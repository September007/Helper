#include <Helper/OpenGL/GLObjects.h>
#include <Helper/log/log.h>

#define INT2VOIDP(i) (void *)(uintptr_t)(i)

static std::mutex g_OpenGL_Mutex;

VertexBuffer *VertexBuffer::create(float *vertices, uint32_t size)
{
    return new OpenGLVertexBuffer(vertices, size);
}

IndexBuffer *IndexBuffer::create(uint32_t *indices, uint32_t count)
{
    return new OpenGLIndexBuffer(indices, count);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float *vertices, uint32_t size)
{
    glGenBuffers(1, &m_rendererId);
    bind();

    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
}
// xucl: note there we change the mean of count to sizeof buffer
OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t *indices, uint32_t count)
    : m_count(count / sizeof(uint32_t))
{
    glGenBuffers(1, &m_rendererId);
    bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count, indices, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    glDeleteBuffers(1, &m_rendererId);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    glDeleteBuffers(1, &m_rendererId);
}

void OpenGLVertexBuffer::bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, m_rendererId);
}

void OpenGLVertexBuffer::unbind() const
{
    GLint c;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &c);
    if (c != m_rendererId)
        GLLogger()->warn("Unbinding GL_ARRAY_BUFFER[current:{},this:{}] but it's not current BUFFER OBJECT, this maybe misuse of "
                         "unbinding or Thread-traffic",
            c, m_rendererId);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void OpenGLIndexBuffer::bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_rendererId);
}

void OpenGLIndexBuffer::unbind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

uint32_t OpenGLIndexBuffer::getCount() const
{
    return m_count;
}
GLenum Shader2OpenGLType(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Float:
    case ShaderDataType::Float2:
    case ShaderDataType::Float3:
    case ShaderDataType::Float4:
        return GL_FLOAT;
    case ShaderDataType::Int:
    case ShaderDataType::Int2:
    case ShaderDataType::Int3:
    case ShaderDataType::Int4:
        return GL_INT;
    case ShaderDataType::Mat3:
    case ShaderDataType::Mat4:
        return GL_FLOAT;
    case ShaderDataType::Bool:
        return GL_BOOL;
    default:
        GLLogger()->error("Unsupported Shader data type");
        return 0;
    }
}

VertexArray *VertexArray::create()
{
    return new OpenGLVertexArray();
}

OpenGLVertexArray::OpenGLVertexArray()
{
    glGenVertexArrays(1, &m_rendererId);
}

// xucl : auto delete
OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &m_rendererId);
};

void OpenGLVertexArray::bind() const
{
    glBindVertexArray(m_rendererId);
}

void OpenGLVertexArray::unbind() const
{
    glBindVertexArray(0);
}

void OpenGLVertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer> &vbo)
{
    if (vbo->getLayout().getElements().size() == 0)
    {
        GLLogger()->error("Buffer Layout not set!");
        return;
    }

    glBindVertexArray(m_rendererId);
    vbo->bind();

    const auto &layout = vbo->getLayout();

    uint32_t index = 0;
    for (const auto &element : layout)
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, element.getComponentCount(), Shader2OpenGLType(element.getType()),
            element.isNormalized() ? GL_TRUE : GL_FALSE, layout.getStride(), INT2VOIDP(element.getOffset()));
        index++;
    }
    m_vertexBuffers.push_back(vbo);
    glBindVertexArray(0);
}

void OpenGLVertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer> &ibo)
{
    glBindVertexArray(m_rendererId);
    ibo->bind();
    m_indexBuffer = ibo;
    glBindVertexArray(0);
}

std::mutex &GetGLobalOpenGLMutex()
{
    return g_OpenGL_Mutex;
}

GLFWwindow *CreateGLContext(GLFWwindow *share, std::string title, int64_t width, int64_t height)
{
    std::unique_lock<std::mutex> lock(GetGLobalOpenGLMutex());
    auto                         logger = GLLogger();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow((int)width, (int)height, title.data(), NULL, share);
    if (window != 0)
        logger->debug("GLFWwindow create window[{},share:{}] Success", intptr_t(window), intptr_t(share));
    else
        logger->error("GLFWwindow create window[{},share:{}] Failed", intptr_t(window), intptr_t(share));
    return window;
}

OpenGLContext::OpenGLContext(GLFWwindow *windowHandle)
    : m_windowHandle(windowHandle)
{
    if (!windowHandle)
    {
        GLLogger()->critical("Could not create OpenGL Context: Handle is NULL");
        exit(1);
    }
}

OpenGLContext::~OpenGLContext() = default;

void OpenGLContext::init()
{
    glfwMakeContextCurrent(m_windowHandle);
    // xucl load this only one time
    static ScopeObject one_time_init{[]
        {
            int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
            if (!version)
            {
                GLLogger()->critical("Could not initialize GLAD");
                exit(1);
            }
        },
        [] {}};
    CurrentContext() = this;

    GLLogger()->info("OpenGL Vendor: {}", (char *)glGetString(GL_VENDOR));
    GLLogger()->info("OpenGL Device: {}", (char *)glGetString(GL_RENDERER));
    GLLogger()->info("OpenGL Version: {}", (char *)glGetString(GL_VERSION));
}
void OpenGLContext::swapBuffers()
{
    glfwSwapBuffers(m_windowHandle);
}