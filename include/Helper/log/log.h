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

#if !defined(TRACE_CALL)
#    if !defined(NO_TRACE_CALL)
#        define TRACE_CALL(logger, desc, api) logger->trace("CALL [{}] {} at {}:{}}", desc, #api, __FILE__, __LINE__)
#    else
#        define TRACE_CALL()
#    endif
#endif

#define GL_ERROR_STOP()                                                                                                                    \
    {                                                                                                                                      \
        g_error = glGetError();                                                                                                            \
        if (g_error != 0)                                                                                                                  \
        {                                                                                                                                  \
            GLLogger()->error("gl get error[{}]: at {}:{}", GL_Formater_Enum_Param(g_error), __FILE__, __LINE__);                          \
            throw 1;                                                                                                                       \
        }                                                                                                                                  \
    }

#define GL_API_CALL_RAW(glapi, desc)                                                                                                       \
    do                                                                                                                                     \
    {                                                                                                                                      \
        TRACE_CALL(GLLogger(), desc, glapi);                                                                                               \
        glapi;                                                                                                                             \
        auto errorCode = glGetError();                                                                                                     \
        if (errorCode != 0)                                                                                                                \
        {                                                                                                                                  \
            GLLogger()->error("{} {} [{}] at {}:{}", desc, #glapi, Formater_Enum_Param(errorCode), __FILE__, __LINE__);                    \
        }                                                                                                                                  \
    } while (0)

#define NV_API_CALL_RAW(nvapi, desc)                                                                                                       \
    do                                                                                                                                     \
    {                                                                                                                                      \
        TRACE_CALL(CULogger(), desc, nvapi);                                                                                               \
        auto errorCode = nvapi;                                                                                                            \
        if (errorCode != CUDA_SUCCESS)                                                                                                     \
        {                                                                                                                                  \
            const char *errStr = 0;                                                                                                        \
            cuGetErrorString(errorCode, &errStr);                                                                                          \
            CULogger()->error(                                                                                                             \
                "{} {} [{}:{}] at {}:{}", desc, #nvapi, Formater_Enum_Param(errorCode), errStr ? errStr : "", __FILE__, __LINE__);         \
        }                                                                                                                                  \
    } while (0)

#define NVENC_API_CALL(api_call) NV_API_CALL_RAW(api_call, "ENC")
#define NV_API_CALL(api_call)    NV_API_CALL_RAW(api_call, "NV")

#define RAW_LOG_ASSERT(logger, desc, condi, action)                                                                                        \
    do                                                                                                                                     \
    {                                                                                                                                      \
        auto result = (condi);                                                                                                             \
        if (!result)                                                                                                                       \
            logger->error("{} {} failed at {}:{}", desc, #condi, __FILE__, __LINE__);                                                      \
    } while (0)

Helper_API int CurrentThreadId();
Helper_API int CurrentThreadName();
