#pragma once
#include <cuda.h>
#include <boost/describe/enum.hpp>
#include <boost/describe.hpp>
#pragma push_macro("BDEE")
#undef BDEE
#define BDEE BOOST_DESCRIBE_ENUM_ENTRY
// CUresult
namespace should_use_BOOST_DESCRIBE_NESTED_ENUM {}
static_assert(std::is_enum<cudaError_enum>::value, "BOOST_DESCRIBE_ENUM should only be used with enums");
BOOST_DESCRIBE_ENUM_BEGIN(cudaError_enum)

BDEE(cudaError_enum, CUDA_SUCCESS)                              /* = 0, */
BDEE(cudaError_enum, CUDA_ERROR_INVALID_VALUE)                  /* = 1, */
BDEE(cudaError_enum, CUDA_ERROR_OUT_OF_MEMORY)                  /* = 2, */
BDEE(cudaError_enum, CUDA_ERROR_NOT_INITIALIZED)                /* = 3, */
BDEE(cudaError_enum, CUDA_ERROR_DEINITIALIZED)                  /* = 4, */
BDEE(cudaError_enum, CUDA_ERROR_PROFILER_DISABLED)              /* = 5, */
BDEE(cudaError_enum, CUDA_ERROR_PROFILER_NOT_INITIALIZED)       /* = 6, */
BDEE(cudaError_enum, CUDA_ERROR_PROFILER_ALREADY_STARTED)       /* = 7, */
BDEE(cudaError_enum, CUDA_ERROR_PROFILER_ALREADY_STOPPED)       /* = 8, */
BDEE(cudaError_enum, CUDA_ERROR_STUB_LIBRARY)                   /* = 34, */
BDEE(cudaError_enum, CUDA_ERROR_DEVICE_UNAVAILABLE)             /* = 46, */
BDEE(cudaError_enum, CUDA_ERROR_NO_DEVICE)                      /* = 100, */
BDEE(cudaError_enum, CUDA_ERROR_INVALID_DEVICE)                 /* = 101, */
BDEE(cudaError_enum, CUDA_ERROR_DEVICE_NOT_LICENSED)            /* = 102, */
BDEE(cudaError_enum, CUDA_ERROR_INVALID_IMAGE)                  /* = 200, */
BDEE(cudaError_enum, CUDA_ERROR_INVALID_CONTEXT)                /* = 201, */
BDEE(cudaError_enum, CUDA_ERROR_CONTEXT_ALREADY_CURRENT)        /* = 202, */
BDEE(cudaError_enum, CUDA_ERROR_MAP_FAILED)                     /* = 205, */
BDEE(cudaError_enum, CUDA_ERROR_UNMAP_FAILED)                   /* = 206, */
BDEE(cudaError_enum, CUDA_ERROR_ARRAY_IS_MAPPED)                /* = 207, */
BDEE(cudaError_enum, CUDA_ERROR_ALREADY_MAPPED)                 /* = 208, */
BDEE(cudaError_enum, CUDA_ERROR_NO_BINARY_FOR_GPU)              /* = 209, */
BDEE(cudaError_enum, CUDA_ERROR_ALREADY_ACQUIRED)               /* = 210, */
BDEE(cudaError_enum, CUDA_ERROR_NOT_MAPPED)                     /* = 211, */
BDEE(cudaError_enum, CUDA_ERROR_NOT_MAPPED_AS_ARRAY)            /* = 212, */
BDEE(cudaError_enum, CUDA_ERROR_NOT_MAPPED_AS_POINTER)          /* = 213, */
BDEE(cudaError_enum, CUDA_ERROR_ECC_UNCORRECTABLE)              /* = 214, */
BDEE(cudaError_enum, CUDA_ERROR_UNSUPPORTED_LIMIT)              /* = 215, */
BDEE(cudaError_enum, CUDA_ERROR_CONTEXT_ALREADY_IN_USE)         /* = 216, */
BDEE(cudaError_enum, CUDA_ERROR_PEER_ACCESS_UNSUPPORTED)        /* = 217, */
BDEE(cudaError_enum, CUDA_ERROR_INVALID_PTX)                    /* = 218, */
BDEE(cudaError_enum, CUDA_ERROR_INVALID_GRAPHICS_CONTEXT)       /* = 219, */
BDEE(cudaError_enum, CUDA_ERROR_NVLINK_UNCORRECTABLE)           /* = 220, */
BDEE(cudaError_enum, CUDA_ERROR_JIT_COMPILER_NOT_FOUND)         /* = 221, */
BDEE(cudaError_enum, CUDA_ERROR_UNSUPPORTED_PTX_VERSION)        /* = 222, */
BDEE(cudaError_enum, CUDA_ERROR_JIT_COMPILATION_DISABLED)       /* = 223, */
BDEE(cudaError_enum, CUDA_ERROR_UNSUPPORTED_EXEC_AFFINITY)      /* = 224, */
BDEE(cudaError_enum, CUDA_ERROR_INVALID_SOURCE)                 /* = 300, */
BDEE(cudaError_enum, CUDA_ERROR_FILE_NOT_FOUND)                 /* = 301, */
BDEE(cudaError_enum, CUDA_ERROR_SHARED_OBJECT_SYMBOL_NOT_FOUND) /* = 302, */
BDEE(cudaError_enum, CUDA_ERROR_SHARED_OBJECT_INIT_FAILED)      /* = 303, */
BDEE(cudaError_enum, CUDA_ERROR_OPERATING_SYSTEM)               /* = 304, */
BDEE(cudaError_enum, CUDA_ERROR_INVALID_HANDLE)                 /* = 400, */
BDEE(cudaError_enum, CUDA_ERROR_ILLEGAL_STATE)                  /* = 401, */
BDEE(cudaError_enum, CUDA_ERROR_NOT_FOUND)                      /* = 500, */
BDEE(cudaError_enum, CUDA_ERROR_NOT_READY)                      /* = 600, */
BDEE(cudaError_enum, CUDA_ERROR_ILLEGAL_ADDRESS)                /* = 700, */
BDEE(cudaError_enum, CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES)        /* = 701, */
BDEE(cudaError_enum, CUDA_ERROR_LAUNCH_TIMEOUT)                 /* = 702, */
BDEE(cudaError_enum, CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING)  /* = 703, */
BDEE(cudaError_enum, CUDA_ERROR_PEER_ACCESS_ALREADY_ENABLED)    /* = 704, */
BDEE(cudaError_enum, CUDA_ERROR_PEER_ACCESS_NOT_ENABLED)        /* = 705, */
BDEE(cudaError_enum, CUDA_ERROR_PRIMARY_CONTEXT_ACTIVE)         /* = 708, */
BDEE(cudaError_enum, CUDA_ERROR_CONTEXT_IS_DESTROYED)           /* = 709, */
BDEE(cudaError_enum, CUDA_ERROR_ASSERT)                         /* = 710, */
BDEE(cudaError_enum, CUDA_ERROR_TOO_MANY_PEERS)                 /* = 711, */
BDEE(cudaError_enum, CUDA_ERROR_HOST_MEMORY_ALREADY_REGISTERED) /* = 712, */
BDEE(cudaError_enum, CUDA_ERROR_HOST_MEMORY_NOT_REGISTERED)     /* = 713, */
BDEE(cudaError_enum, CUDA_ERROR_HARDWARE_STACK_ERROR)           /* = 714, */
BDEE(cudaError_enum, CUDA_ERROR_ILLEGAL_INSTRUCTION)            /* = 715, */
BDEE(cudaError_enum, CUDA_ERROR_MISALIGNED_ADDRESS)             /* = 716, */
BDEE(cudaError_enum, CUDA_ERROR_INVALID_ADDRESS_SPACE)          /* = 717, */
BDEE(cudaError_enum, CUDA_ERROR_INVALID_PC)                     /* = 718, */
BDEE(cudaError_enum, CUDA_ERROR_LAUNCH_FAILED)                  /* = 719, */
BDEE(cudaError_enum, CUDA_ERROR_COOPERATIVE_LAUNCH_TOO_LARGE)   /* = 720, */
BDEE(cudaError_enum, CUDA_ERROR_NOT_PERMITTED)                  /* = 800, */
BDEE(cudaError_enum, CUDA_ERROR_NOT_SUPPORTED)                  /* = 801, */
BDEE(cudaError_enum, CUDA_ERROR_SYSTEM_NOT_READY)               /* = 802, */
BDEE(cudaError_enum, CUDA_ERROR_SYSTEM_DRIVER_MISMATCH)         /* = 803, */
BDEE(cudaError_enum, CUDA_ERROR_COMPAT_NOT_SUPPORTED_ON_DEVICE) /* = 804, */
BDEE(cudaError_enum, CUDA_ERROR_MPS_CONNECTION_FAILED)          /* = 805, */
BDEE(cudaError_enum, CUDA_ERROR_MPS_RPC_FAILURE)                /* = 806, */
BDEE(cudaError_enum, CUDA_ERROR_MPS_SERVER_NOT_READY)           /* = 807, */
BDEE(cudaError_enum, CUDA_ERROR_MPS_MAX_CLIENTS_REACHED)        /* = 808, */
BDEE(cudaError_enum, CUDA_ERROR_MPS_MAX_CONNECTIONS_REACHED)    /* = 809, */
BDEE(cudaError_enum, CUDA_ERROR_MPS_CLIENT_TERMINATED)          /* = 810, */
BDEE(cudaError_enum, CUDA_ERROR_STREAM_CAPTURE_UNSUPPORTED)     /* = 900, */
BDEE(cudaError_enum, CUDA_ERROR_STREAM_CAPTURE_INVALIDATED)     /* = 901, */
BDEE(cudaError_enum, CUDA_ERROR_STREAM_CAPTURE_MERGE)           /* = 902, */
BDEE(cudaError_enum, CUDA_ERROR_STREAM_CAPTURE_UNMATCHED)       /* = 903, */
BDEE(cudaError_enum, CUDA_ERROR_STREAM_CAPTURE_UNJOINED)        /* = 904, */
BDEE(cudaError_enum, CUDA_ERROR_STREAM_CAPTURE_ISOLATION)       /* = 905, */
BDEE(cudaError_enum, CUDA_ERROR_STREAM_CAPTURE_IMPLICIT)        /* = 906, */
BDEE(cudaError_enum, CUDA_ERROR_CAPTURED_EVENT)                 /* = 907, */
BDEE(cudaError_enum, CUDA_ERROR_STREAM_CAPTURE_WRONG_THREAD)    /* = 908, */
BDEE(cudaError_enum, CUDA_ERROR_TIMEOUT)                        /* = 909, */
BDEE(cudaError_enum, CUDA_ERROR_GRAPH_EXEC_UPDATE_FAILURE)      /* = 910, */
BDEE(cudaError_enum, CUDA_ERROR_EXTERNAL_DEVICE)                /* = 911, */
BDEE(cudaError_enum, CUDA_ERROR_INVALID_CLUSTER_SIZE)           /* = 912, */
BDEE(cudaError_enum, CUDA_ERROR_UNKNOWN)                        /* = 999 */

BOOST_DESCRIBE_ENUM_END(cudaError_enum);

#pragma pop_macro("BDEE")