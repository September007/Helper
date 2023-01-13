/* OpenGL Objects */
/* Context;Shader;Program;VBO;VEO;Layout */
#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Helper/log/log.h>
#include <Helper/macro_util/macro_util.h>
#include <Helper/OpenGL/GLUtils.h>
#include <string>
#include <map>
#include <memory>
#include <stdexcept>

#ifndef STRICT_
#    define STRICT_ true
#endif

/* global mutex for OpenGL */
Helper_API std::mutex &GetGLobalOpenGLMutex();

/* thread-safe create GLFW Window,pass 'share' window to share object */
Helper_API GLFWwindow *CreateGLContext(GLFWwindow *share, std::string title, int64_t width, int64_t height);

enum class ShaderDataType
{
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Int,
    Int2,
    Int3,
    Int4,
    Mat3,
    Mat4,
    Bool
};
struct BufferElement
{
    BufferElement(ShaderDataType type, std::string name, bool normalized = false)
        : m_type(type)
        , m_name(name)
        , m_offset(0)
        , m_size(getSize(type))
        , m_componentCount(getComponentCount(type))
        , m_normalized(normalized)
    {}

    void setOffset(uint32_t offset)
    {
        this->m_offset = offset;
    }

    uint32_t getSize() const
    {
        return m_size;
    }

    uint32_t getComponentCount() const
    {
        return m_componentCount;
    }

    uint32_t getOffset() const
    {
        return m_offset;
    }

    ShaderDataType getType() const
    {
        return m_type;
    }

    bool isNormalized() const
    {
        return m_normalized;
    }

    auto getName() const
    {
        return m_name;
    }

private:
    ShaderDataType m_type;
    std::string m_name;
    uint32_t m_offset;
    uint32_t m_size;
    uint32_t m_componentCount;
    bool m_normalized;

private:
    inline uint32_t getSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
            return 4;
        case ShaderDataType::Float2:
            return 4 * 2;
        case ShaderDataType::Float3:
            return 4 * 3;
        case ShaderDataType::Float4:
            return 4 * 4;
        case ShaderDataType::Int:
            return 4;
        case ShaderDataType::Int2:
            return 4 * 2;
        case ShaderDataType::Int3:
            return 4 * 3;
        case ShaderDataType::Int4:
            return 4 * 4;
        case ShaderDataType::Mat3:
            return 4 * 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4 * 4;
        case ShaderDataType::Bool:
            return 1;
        default:
            GLLogger()->warn("Unsupported Shader data type");
            return 0;
        }
    }

    inline uint32_t getComponentCount(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
            return 1;
        case ShaderDataType::Float2:
            return 2;
        case ShaderDataType::Float3:
            return 3;
        case ShaderDataType::Float4:
            return 4;
        case ShaderDataType::Int:
            return 1;
        case ShaderDataType::Int2:
            return 2;
        case ShaderDataType::Int3:
            return 3;
        case ShaderDataType::Int4:
            return 4;
        case ShaderDataType::Mat3:
            return 3 * 3;
        case ShaderDataType::Mat4:
            return 4 * 4;
        case ShaderDataType::Bool:
            return 1;
        default:
            GLLogger()->warn("Unsupported Shader data type");
            return 0;
        }
    }
};

class BufferLayout
{
public:
    BufferLayout() = default;

    BufferLayout(std::initializer_list<BufferElement> elements)
        : m_elements(elements)
    {
        m_stride = 0;
        for (auto &element : this->m_elements)
        {
            element.setOffset(m_stride);
            m_stride += element.getSize();
        }
    }

    inline uint32_t getStride() const
    {
        return m_stride;
    }

    inline const std::vector<BufferElement> &getElements() const
    {
        return m_elements;
    }

    std::vector<BufferElement>::iterator begin()
    {
        return m_elements.begin();
    }
    std::vector<BufferElement>::iterator end()
    {
        return m_elements.end();
    }
    std::vector<BufferElement>::const_iterator begin() const
    {
        return m_elements.begin();
    }
    std::vector<BufferElement>::const_iterator end() const
    {
        return m_elements.end();
    }

private:
    std::vector<BufferElement> m_elements;
    uint32_t m_stride;
};

class Helper_API_CLS VertexBuffer
{
public:
    VertexBuffer() = default;
    virtual ~VertexBuffer() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;
    virtual ScopeObject temp_bind(){
        return ScopeObject([p=this]{
            p->bind();
        },[p=this]{
            p->unbind();
        });
    }
    inline void setLayout(BufferLayout layout)
    {
        this->m_layout = layout;
    }

    inline const BufferLayout &getLayout()
    {
        return m_layout;
    }

    static VertexBuffer *create(float *vertices, uint32_t size);

protected:
    BufferLayout m_layout;
};

class Helper_API_CLS IndexBuffer
{
public:
    IndexBuffer() = default;
    virtual ~IndexBuffer() = default;

    virtual void bind() const = 0;
    virtual void unbind() const = 0;

    virtual uint32_t getCount() const = 0;

    static IndexBuffer *create(uint32_t *indices, uint32_t count);
};
class Helper_API_CLS OpenGLVertexBuffer : public VertexBuffer
{
public:
    OpenGLVertexBuffer(float *vertices, uint32_t size);
    virtual ~OpenGLVertexBuffer();

    virtual void bind() const override;
    virtual void unbind() const override;

private:
    uint32_t m_rendererId;
};

class Helper_API_CLS OpenGLIndexBuffer : public IndexBuffer
{
public:
    OpenGLIndexBuffer(uint32_t *indices, uint32_t count);
    virtual ~OpenGLIndexBuffer();

    virtual void bind() const override;
    virtual void unbind() const override;

    uint32_t getCount() const override;

private:
    uint32_t m_rendererId;
    uint32_t m_count;
};

class Helper_API_CLS VertexArray
{
public:
    VertexArray() = default;
    virtual ~VertexArray() = default;

    virtual void        bind() const = 0;
    virtual void        unbind() const = 0;
    virtual ScopeObject temp_bind()
    {
        return ScopeObject{[p = this]
            {
                p->bind();
            },
            [p = this]
            {
                p->unbind();
            }};
    }
    virtual void addVertexBuffer(const std::shared_ptr<VertexBuffer> &vbo) = 0;
    virtual void setIndexBuffer(const std::shared_ptr<IndexBuffer> &ibo) = 0;

    virtual const std::vector<std::shared_ptr<VertexBuffer>> &getVertexBuffers() const = 0;
    virtual const std::shared_ptr<IndexBuffer> &getIndexBuffer() const = 0;

    static VertexArray *create();
};

class Helper_API_CLS GraphicsContext
{
public:
    GraphicsContext() = default;
    virtual ~GraphicsContext() = default;

    virtual void init() = 0;
    virtual void swapBuffers() = 0;
};

class Helper_API_CLS OpenGLVertexArray : public VertexArray
{
public:
    OpenGLVertexArray();
    virtual ~OpenGLVertexArray();

    void bind() const override;
    void unbind() const override;

    void addVertexBuffer(const std::shared_ptr<VertexBuffer> &vbo) override;
    void setIndexBuffer(const std::shared_ptr<IndexBuffer> &ibo) override;

    const std::vector<std::shared_ptr<VertexBuffer>> &getVertexBuffers() const override
    {
        return m_vertexBuffers;
    }
    const std::shared_ptr<IndexBuffer> &getIndexBuffer() const override
    {
        return m_indexBuffer;
    }

private:
    uint32_t m_rendererId;

    std::vector<std::shared_ptr<VertexBuffer>> m_vertexBuffers;
    std::shared_ptr<IndexBuffer> m_indexBuffer;
};

class Helper_API_CLS OpenGLContext : public GraphicsContext
{
public:
    OpenGLContext(GLFWwindow *windowHandle);
    ~OpenGLContext();

    void init() override;
    void swapBuffers() override;
    static OpenGLContext *&CurrentContext()
    {
        static OpenGLContext *cur = nullptr;
        return cur;
    }

    auto GetHandle()
    {
        return m_windowHandle;
    }

private:
    GLFWwindow *m_windowHandle;
};

/************************************************************************/

template<bool strict_ = STRICT_>
struct ShaderObject
{

    std::shared_ptr<GLint> shaderHandler = nullptr;
    // save state info which queried last time, the construction func would set this as compile info
    std::string tempInfo;
    ShaderObject(GLint shader_type, std::string src)
    {
        shaderHandler = std::shared_ptr<GLint>(new GLint(glCreateShader(shader_type)),
            [](GLint *hand)
            {
                if (*hand)
                    glDeleteShader(*hand);
                delete hand;
            });
        const char *const srcs[] = {src.c_str()};
        glShaderSource(*shaderHandler, 1, srcs, NULL);
        glCompileShader(*shaderHandler);
        int success;
        // get compile info
        int len;
        char infoLog[512];
        glGetShaderiv(*shaderHandler, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(*shaderHandler, 512, &len, infoLog);
            tempInfo = {infoLog, infoLog + len};

            if constexpr (strict_)
            {
                GLLogger()->error("shader compile failed:\n\t{}", tempInfo);
            }
        }
    }
    auto Attach(GLint targetProgram) const -> void
    {
        return glAttachShader(targetProgram, *shaderHandler);
    }
    auto getInfo(GLenum pname)
    {
        constexpr int tempLength = 1024;
        char temp[tempLength];
        GLint params = GL_FALSE;
        glGetShaderiv(*shaderHandler, pname, &params);
        GLsizei len;
        glGetShaderInfoLog(*shaderHandler, tempLength, &len, temp);
        tempInfo = {temp, temp + len};
        return params;
    }
};

namespace Helper {
template<typename... MultiShaderObj>
inline GLint CreateProgram(MultiShaderObj... mso);
}
using GLHandle = std::shared_ptr<GLint>;
template<bool strict_ = STRICT_>
struct ProgramObject
{
    GLHandle program = nullptr;
    // save state info recently queried
    std::string tempInfo;
    ProgramObject(GLint program = 0)
    {
        reset(program);
    }
    ProgramObject(const std::string &vsFileName, const std::string &fsFileName)
    {
        reset(Helper::CreateProgram(
            ShaderObject(GL_VERTEX_SHADER, readFile(vsFileName)), ShaderObject(GL_FRAGMENT_SHADER, readFile(fsFileName))));
    }
    void reset(GLint program)
    {
        this->program.reset(new GLint(program), [](GLint *p) { glDeleteProgram(*p); });
        if (program == 0)
            return;
        // check program link status
        auto ret = getInfo(GL_LINK_STATUS);

        if (ret != GL_TRUE)
        {
            GLLogger()->critical("program link failed:\n\t{}", tempInfo);
            if constexpr (strict_)
                throw std::runtime_error("program link failed:\n\t" + tempInfo);
        }
    }
    DebugArea(static inline std::atomic<int> use_cnt = 0;);
    void use()
    {
        DebugArea(ASSERT(use_cnt.load() == 0));
        glUseProgram(getProgram());
        DebugArea(use_cnt++;);
    }
    void unuse()
    {
        DebugArea(ASSERT(use_cnt == 1));
        glUseProgram(0);
        DebugArea(use_cnt--);
    }
    [[nodiscard]] auto temp_use()
    {
        // here could report something
        return ScopeObject([this] { this->use(); }, [this] { this->unuse(); });
    }
    GLint getProgram() const
    {
        return *program;
    }
    auto getInfo(GLenum pname)
    {
        if (getProgram() == 0)
            return GL_FALSE;
        constexpr int tempLength = 1024;
        char temp[tempLength];
        GLint params = GL_FALSE;
        glGetProgramiv(getProgram(), pname, &params);
        GLsizei len;
        glGetProgramInfoLog(getProgram(), tempLength, &len, temp);
        tempInfo = {temp, temp + len};
        return params;
    }
    auto getAttributes() const
    {
        return ::getAttributes(getProgram());
    }
    auto getUniforms() const
    {
        std::map<std::string, AttributeInfo> ret;
        int count = 0;
        glGetProgramiv(getProgram(), GL_ACTIVE_UNIFORMS, &count);
        constexpr int name_length = 256;
        char cache_name[name_length];
        GLint used_cache_name_length = 0;
        for (int i = 0; i < count; ++i)
        {
            AttributeInfo info;
            glGetActiveUniform(getProgram(), GLint(i), name_length, &used_cache_name_length, &info.size, &info.type, cache_name);
            info.name = std::string(cache_name, cache_name + used_cache_name_length);
            info.position = glGetUniformLocation(getProgram(), info.name.c_str());
            ret[info.name] = info;
        }
        return ret;
    }
    // uniform setter
    void setBool(const std::string &name, bool value) const
    {
        DebugArea(if (!checkExist(getUniforms(), name)) return);
        GLLogger()->trace("{:15} {:<20} {:>10}", "Setbool", name, value);
        glUniform1i(glGetUniformLocation(getProgram(), name.c_str()), (int)value);
    }
    void setInt(const std::string &name, int value) const
    {
        DebugArea(if (!checkExist(getUniforms(), name)) return;);
        GLLogger()->trace("{:15} {:<20} {:>10}", "SetInt", name, value);
        glUniform1i(glGetUniformLocation(getProgram(), name.c_str()), value);
    }
    void setFloat(const std::string &name, float value) const
    {
        DebugArea(if (!checkExist(getUniforms(), name)) return);
        GLLogger()->trace("{:15} {:<20} {:>10}", "SetFloat", name, value);
        glUniform1f(glGetUniformLocation(getProgram(), name.c_str()), value);
    }
    int getUniformLocation(const std::string &name)
    {
        return glGetUniformLocation(getProgram(), name.c_str());
    }
    // this is compaticable of Objects

    // use this for location binding
    void prepareVBO(VertexBuffer &vbo)
    {
        auto layout = vbo.getLayout();
        std::vector<std::pair<std::string, GLint>> locas;
        int pos = 0;
        for (auto &e : layout)
        {
            locas.emplace_back(e.getName(), pos++);
        }
        bindAttributesLocations(getProgram(), locas);
    }
    inline bool checkExist(std::map<std::string, AttributeInfo> map, const std::string &name) const
    {
        auto ex = (map.find(name) != map.end());
        if (!ex)
        {
            GLLogger()->trace("{:15} {:<20} {:>10}", "NoSet-notFound", name, "");
        }
        return ex;
    }
};

namespace {
template<bool isBegin, typename FirstShader, typename... RestShaders>
GLint _CreateProgram(GLint program, FirstShader fs, RestShaders... rest)
{
    static_assert(std::is_same_v<FirstShader, ShaderObject<>>, "只接受ShaderObject类型参数");

    if constexpr (isBegin)
    {
        program = glCreateProgram();
    }
    fs.Attach(program);

    if constexpr (sizeof...(RestShaders) == 0)
        return program;
    else
        return _CreateProgram<false>(program, rest...);
}

template<int isBegin>
GLint _CreateProgram(GLint program)
{
    return program;
}
} // namespace

namespace Helper {
template<typename... MultiShaderObj>
inline GLint CreateProgram(MultiShaderObj... mso)
{
    auto program = _CreateProgram<true>(0, std::forward<MultiShaderObj>(mso)...);
    glLinkProgram(program);
    return program;
}

} // namespace Helper

class TextureObject
{
public:
    std::shared_ptr<GLuint> obj;
    GLuint targetTexture;
    int width, height, nChannels;
    TextureObject() = default;
    TextureObject(GLuint obj_, GLuint targetTex)
        : targetTexture(targetTex)
    {
        obj.reset(new GLuint(obj_),
            [](GLuint *p)
            {
                if (p)
                    glDeleteTextures(1, p);
                delete p;
            });
    }
    auto GetTexture() const
    {
        return *obj;
    }
    void SetWHN(int w, int h, int n)
    {
        width = w;
        height = h;
        nChannels = n;
    }
    void bind(GLuint target)
    {
        GL_ERROR_STOP();

        // glBindTexture(target,*obj);
        GL_ERROR_STOP();
    }
};
namespace Helper {
// possible param: GL_TEXTURE1 'void*' 100 100 GL_RGBA8 GL_RGB GL_UNSIGNED_BYTE
inline auto CreateTexture(GLuint textureTarget, void *data, int width, int height, GLint internalFormat ,GLenum format,GLenum type)
{
    GLuint texture;
    glActiveTexture(textureTarget); // binding textureTarget->GL_TEXTURE_2D
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // binding GL_TEXTURE_2D->texture, so there is textureTarget->texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    auto ret = TextureObject(texture, textureTarget);

    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, type, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    ret.SetWHN(width, height, -1);
    return ret;
}
struct stb_pic_data
{
    void *data = nullptr;
    int w, h, n;
    static auto create_stb_pic_data(const std::string &filePath)
    {
        std::shared_ptr<stb_pic_data> ret(new stb_pic_data(),
            [](stb_pic_data *p)
            {
                if (p)
                    stbi_image_free(p->data);
            });
        ret->data = stbi_load(filePath.c_str(), &ret->w, &ret->h, &ret->n, 0);
        return ret;
    }

private:
    stb_pic_data() {}
    ~stb_pic_data() {}
};
/* auto load file,then pass file-data, width,height */
inline auto CreateTexture(GLuint textureTarget, const std::string &dataFile,GLint internalFormat,GLenum format,GLenum type)
{
    static ScopeObject setting = {[] { stbi_set_flip_vertically_on_load(true); }, nullptr};
    auto pic_data = stb_pic_data::create_stb_pic_data(dataFile);
    auto ret = CreateTexture(textureTarget, pic_data->data, pic_data->w, pic_data->h,internalFormat, format,type );
    ret.nChannels=pic_data->n;
    return ret;
}
inline auto CreateTextureByData(GLuint textureTarget, void *data, uint32_t width, uint32_t height, GLenum innerFormat, GLenum format,GLenum type)
{

    GLuint texture;
    glActiveTexture(textureTarget); // binding textureTarget->GL_TEXTURE_2D
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // binding GL_TEXTURE_2D->texture, so there is textureTarget->texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    auto ret = TextureObject(texture, textureTarget);

    glTexImage2D(GL_TEXTURE_2D, 0, innerFormat, width, height, 0, format, type, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    ret.SetWHN(width, height, -1);
    return ret;
};
} // namespace Helper
