#include <Helper/log/log.h>

static std::shared_ptr<spdlog::sinks::sink> g_default_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
static std::shared_ptr<spdlog::logger> g_GL_logger = std::make_shared<spdlog::logger>("GL", DefaultSink());
static std::shared_ptr<spdlog::logger> g_CU_logger = std::make_shared<spdlog::logger>("CU", DefaultSink());
int g_error =0;
int g_cuda_error;

std::shared_ptr<spdlog::sinks::sink> &DefaultSink(std::shared_ptr<spdlog::sinks::sink> sink)
{
    if (sink != nullptr)
        g_default_sink = sink;
    return g_default_sink;
}

static std::shared_ptr<spdlog::logger> g_default_logger = spdlog::default_logger();

std::shared_ptr<spdlog::logger> &DefaultLogger(std::shared_ptr<spdlog::logger> logger)
{
    if (logger != nullptr)
        g_default_logger = logger;
    return g_default_logger;
}

std::shared_ptr<spdlog::logger> GLLogger(std::shared_ptr<spdlog::logger> new_logger)
{
    if (new_logger != nullptr)
        g_GL_logger = new_logger;
    return g_GL_logger;
}

std::shared_ptr<spdlog::logger> CULogger(std::shared_ptr<spdlog::logger> new_logger)
{
    if (new_logger != nullptr)
        g_CU_logger = new_logger;
    return g_CU_logger;
}