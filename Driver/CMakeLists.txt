cmake_minimum_required(VERSION 3.22)
# Enable CMake support for ASM and C languages
enable_language(C ASM)

file(GLOB_RECURSE GD32F4xx_SPL_SRC
    "GD32F4xx_standard_peripheral/Source/*.c"
    "CMSIS/GD/GD32F4xx/Source/system_gd32f4xx.c"
)


add_library(GD32F4xx_SPL OBJECT
    ${GD32F4xx_SPL_SRC}
)

target_include_directories(GD32F4xx_SPL PUBLIC
    "GD32F4xx_standard_peripheral/Include"
    "CMSIS/"
    "CMSIS/GD/GD32F4xx/Include"
)