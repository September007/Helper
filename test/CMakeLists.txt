file(GLOB_RECURSE single_files "singles/*.cpp")
print(single_files)


foreach(src ${single_files})
    STRING(REGEX REPLACE ".*/([^/]+)\\..*" "\\1" FILE_NAME ${src})
    STRING(REGEX REPLACE ".*/([^/]+)\\..*" "\\1" FILE_SUFFIX ${src})
    print(FILE_NAME)
    add_executable(${FILE_NAME} ${src})

    if(MSVC)
        target_compile_options(${FILE_NAME} PUBLIC

            # warning C4251: ... needs to have dll-interface to be used by clients of class 'spdlog::spdlog_ex'
            # warning C4275: spdlog\common.h(277): non dll-interface class 'std::exception' used as base for dll-interface class 'spdlog::spdlog_ex'
            /wd4819 /wd4251 /4275)
    endif()
    foreach(HLIB ${HELPER_LIBS})
        if("${FILE_NAME}" MATCHES ".*${HLIB}.*" AND TARGET Helper::${HLIB})
            target_link_libraries(${FILE_NAME} PRIVATE Helper::${HLIB})
            message("${FILE_NAME} matches ${HLIB}")
        endif()
    endforeach()
    # default link
    target_link_libraries(${FILE_NAME} PRIVATE Helper::log)
    
endforeach()

# target_link_libraries(d_type PUBLIC ${D_TYPE_LIBS} ${D_TYPE_EXTRA_DEPS})
target_link_libraries(d_type_hellow_world PUBLIC Helper::glm)
set_property(TARGET color_transform PROPERTY  CXX_STANDARD 20)
