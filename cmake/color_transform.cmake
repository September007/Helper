

set_property(TARGET Helper_color_transform PROPERTY 20)

if(MSVC)
    target_compile_options(Helper_color_transform INTERFACE /c++20)
else()
    target_compile_options(Helper_color_transform INTERFACE -std=c++20)
endif()