#pragma once
#include <Helper/project/symbol.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

Helper_API std::shared_ptr<spdlog::sinks::sink> &DefaultSink(std::shared_ptr<spdlog::sinks::sink> sink = nullptr);
Helper_API std::shared_ptr<spdlog::logger> &DefaultLogger(std::shared_ptr<spdlog::logger> logger = nullptr);
Helper_API std::shared_ptr<spdlog::logger> GLLogger(std::shared_ptr<spdlog::logger> new_logger = nullptr);

Helper_API_VAR int g_error;
#define GL_ERROR_STOP()                                                                                                                    \
    {                                                                                                                                      \
        g_error = glGetError();                                                                                                            \
        if (g_error != 0)                                                                                                                  \
            GLLogger()->error("gl get error: at %s:%d", g_error, __FILE__, __LINE__);                                                      \
    }

Helper_API int CurrentThreadId();
Helper_API int CurrentThreadName();
