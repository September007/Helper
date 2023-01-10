#pragma once
#include <Helper/project/symbol.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

/* HELPER_LOG_NO_DESCRIBE: if you dont have boost/describe so define it */
/* note: things like GL_INVALID_PARAM is just macro-defined int-value   */
#ifndef HELPER_LOG_NO_DESCRIBE
#    include <boost/describe.hpp>
#    include <Helper/log/cuda_enum.h>
#    include <Helper/log/GL_enum.h>
#    include <Helper/log/log_formater.h>
#    define Formater_Enum_Param(e)    e
#    define GL_Formater_Enum_Param(e) _GL_Enum(e)
#else
#    define Formater_Enum_Param(e)    int(e)
#    define GL_Formater_Enum_Param(e) e
#endif

Helper_API std::shared_ptr<spdlog::sinks::sink> &DefaultSink(std::shared_ptr<spdlog::sinks::sink> sink = nullptr);
Helper_API std::shared_ptr<spdlog::logger> &DefaultLogger(std::shared_ptr<spdlog::logger> logger = nullptr);
Helper_API std::shared_ptr<spdlog::logger> GLLogger(std::shared_ptr<spdlog::logger> new_logger = nullptr);
Helper_API std::shared_ptr<spdlog::logger> CULogger(std::shared_ptr<spdlog::logger> new_logger = nullptr);

Helper_API_VAR int g_error;
#define GL_ERROR_STOP()                                                                                                                    \
    {                                                                                                                                      \
        g_error = glGetError();                                                                                                            \
        if (g_error != 0)                                                                                                                  \
        {                                                                                                                                  \
            GLLogger()->error("gl get error[{}]: at {}:{}", GL_Formater_Enum_Param(g_error), __FILE__, __LINE__);                                        \
            throw 1;                                                                                                                       \
        }                                                                                                                                  \
    }

#define NVENC_API_CALL(nvencAPI)                                                                                                           \
    do                                                                                                                                     \
    {                                                                                                                                      \
        auto errorCode = nvencAPI;                                                                                                         \
        if (errorCode != CUDA_SUCCESS)                                                                                                     \
        {                                                                                                                                  \
            CULogger()->error("{} err:[{}] at {}:{}", #nvencAPI, Formater_Enum_Param(errorCode), __FILE__, __LINE__);                      \
        }                                                                                                                                  \
    } while (0)

Helper_API int CurrentThreadId();
Helper_API int CurrentThreadName();
