cmake_minimum_required(VERSION 3.10 FATAL_ERROR)

macro(print)
    foreach(var ${ARGV})
        message(STATUS "${var}:${${var}}")
    endforeach()
endmacro()

if(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux")
    set(Linux ON)
elseif(${CMAKE_HOST_SYSTEM_NAME} MATCHES "Windows")
    set(Windows ON)
    add_compile_definitions(WIN32)
endif()

project(Helper)

option(Helper_BUILD_SHARED "[Helper] build shared libs" ON)
option(Helper_BUILD_EXAMPLE "[Helper] build example" ON)
option(Helper_Enable_CUDA "[Helper] enable cuda" ON)
option(Helper_ThirdParty_Use_Package "[Helper] if super project not set up ThirdParty,Helper will try find_packge in ${ThirParty_ROOT}" "${CMAKE_SOURCE_DIR}/extern")
option(Helper_ThirParty_ROOT "[Helper] root dir of third libs, if dont wanna download again" "${CMAKE_SOURCE_DIR}/extern")
if(MSVC)
    option(Helper_LOG_NO_GL_ENUM_BDEE "[LOG] when using gcc/clang boost/describe for  _GL_Enum with 13xx member is burden ,but msvc just fine " ON)
else()
    option(Helper_LOG_NO_GL_ENUM_BDEE "[LOG] when using gcc/clang boost/describe for  _GL_Enum with 13xx member is burden ,but msvc just fine " OFF)
endif()
option(USE_MOLD_AS_LINKER "use mold as the linker" ON)

message( "\noptions\n ")
print(Helper_BUILD_SHARED Helper_Enable_CUDA Helper_ThirdParty_Use_Package Helper_ThirParty_ROOT Helper_LOG_NO_GL_ENUM_BDEE)
message(" \n")

# # User could custom their setting in there, which is ignored by the .gitignore
if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/custom.cmake)
    message(STATUS "Found customed setting file, now will call it")
    include(${CMAKE_CURRENT_LIST_DIR}/custom.cmake)
endif()

if(Helper_BUILD_SHARED)
    list(APPEND HELPER_PUBLIC_DEFS Helper_EXPORT)
    list(APPEND HELPER_PRIVATE_DEFS Helper_EXPORT_BUILD)
endif()

if(USE_MOLD_AS_LINKER AND NOT GCC)
    set(MOLD_PATH /home/lull/src/CODE/mold/build/release_pack/bin/mold)
    if(EXISTS "${MOLD_PATH}")
        list( APPEND CMAKE_SHARED_LINKER_FLAGS "-fuse-ld=${MOLD_PATH}")
    endif()
endif()

macro(Helper_set_lib lib)
    file(GLOB_RECURSE ${lib}_HEADERS "${CMAKE_CURRENT_LIST_DIR}/include/Helper/${lib}/*")
    file(GLOB_RECURSE ${lib}_SOURCES "${CMAKE_CURRENT_LIST_DIR}/src/${lib}/*")

    if("${${lib}_SOURCES}" STREQUAL "")
        set(${lib}_FORM INTERFACE)
    elseif(Helper_BUILD_SHARED)
        set(${lib}_FORM SHARED)
    else()
        set(${lib}_FORM STATIC)
    endif()

    if("${${lib}_HEADERS}" STREQUAL "")
        message(FATAL_ERROR "${lib} got no INTERFACE header")
    endif()
    add_library(Helper_${lib} ${${lib}_FORM})
    add_library(Helper::${lib} ALIAS Helper_${lib})

    message(STATUS "Helper add ${${lib}_FORM}:${lib}")

    target_include_directories(Helper_${lib} INTERFACE ${CMAKE_CURRENT_LIST_DIR}/include)
    if(NOT ${${lib}_FORM} MATCHES INTERFACE)
        target_sources(Helper_${lib} PRIVATE ${${lib}_SOURCES})
        target_compile_definitions(Helper_${lib} PRIVATE ${HELPER_PRIVATE_DEFS} PUBLIC ${HELPER_PUBLIC_DEFS})
        target_link_libraries(Helper_${lib} PUBLIC Helper_project ${Helper_${lib}_DEPS})
    endif()
endmacro()

set(HELPER_LIBS
    "project"
    color_transform
    fps
    glm
    gtest
    log
    macro_util
    OpenGL
    stb_image
)

# # Third Party
if(NOT TARGET boost_algorithm)
    find_package(Boost NAMES Boost boost HINTS "${ThirParty_ROOT}/Boost" "${ThirParty_ROOT}/boost" REQUIRED)
endif()

if(NOT TARGET spdlog::spdlog)
    find_package(spdlog HINTS "${ThirParty_ROOT}/spglog" REQUIRED)
endif()

if(NOT TARGET glad)
    find_package(glad REQUIRED)
endif()

if(NOT TARGET glfw)
    find_package(glfw REQUIRED)
endif()

configure_file(${CMAKE_CURRENT_LIST_DIR}/src/project.h.in ${CMAKE_CURRENT_LIST_DIR}/include/Helper/project/project.h)


set(Helper_log_DEPS spdlog::spdlog Boost::describe Helper_project)
set(Helper_OpenGL_DEPS glad glfw Helper_log Helper_glm)

foreach(lib ${HELPER_LIBS})
    Helper_set_lib(${lib})
endforeach()

if(Helper_LOG_NO_GL_ENUM_BDEE)
    target_compile_definitions(Helper_log PUBLIC NO_GL_ENUM_BDEE )
endif()

if(Helper_BUILD_EXAMPLE AND EXISTS ${CMAKE_CURRENT_LIST_DIR}/example )
    add_subdirectory(example)
endif()