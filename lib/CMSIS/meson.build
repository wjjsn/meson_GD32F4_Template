cmsis_files = files(
    'GD/GD32F4xx/Source/system_gd32f4xx.c',
)

cmsis_inc = include_directories(
    'GD/GD32F4xx/Include',
    '../GD32F4xx_standard_peripheral/Include',
)

cmsis_lib = static_library('GD32F4xx_CMSIS', cmsis_files,
    include_directories: cmsis_inc,
    build_by_default: true,
)

cmsis_dep = declare_dependency(
    include_directories: cmsis_inc,
    link_with: cmsis_lib,
)