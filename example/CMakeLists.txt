
link_libraries(Helper::log)
set(CMAKE_CXX_STANDARD 17)
file(GLOB_RECURSE singles_files "${CMAKE_CURRENT_LIST_DIR}/singles/*.cpp")
print(singles_files)
foreach(src ${singles_files})
    STRING(REGEX REPLACE ".*/([^/]+)\\..*" "\\1" FILE_NAME ${src})
    add_executable(Helper_Example_${FILE_NAME} ${src})
    if(TARGET Helper::${FILE_NAME})
        target_link_libraries(Helper_Example_${FILE_NAME} PRIVATE Helper::${FILE_NAME})
    endif()
endforeach()

