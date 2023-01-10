#pragma once

#ifndef Helper_API_DESC
#    if defined(Helper_EXPORT)
#        if defined(WIN32) || defined(__CYGWIN__)
#            if defined(Helper_EXPORT_BUILD)
// #                pragma message("[pragma message]Helper_API_DESC: Export For Win")
#                if defined(__GNUC__)
#                    define Helper_API_DESC __attribute__((dllexport)) extern
#                else
#                    define Helper_API_DESC __declspec(dllexport)
#                endif
#            else
// #                pragma message("[pragma message]Helper_API_DESC: Importing For Win")
#                if defined(__GNUC__)
#                    define Helper_API_DESC __attribute__((dllimport)) extern
#                else
#                    define Helper_API_DESC __declspec(dllimport)
#                endif
#            endif
#        elif defined(__GNUC__) && defined(Helper_API_DESC_EXPORT_BUILD)
#            define Helper_API_DESC __attribute__((visibility("default")))
#        else
#            define Helper_API_DESC
#        endif
#    else
#        define Helper_API_DESC extern
#    endif

#    define Helper_API_VAR extern Helper_API_DESC
#    define Helper_API_CLS Helper_API_DESC
#    define Helper_API     /*extern "C"*/ Helper_API_DESC // some interface acctullay is CPP interface 
#endif