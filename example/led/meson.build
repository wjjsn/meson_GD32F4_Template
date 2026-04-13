executable('led.elf', files(
    '../../lib/CMSIS/GD/GD32F4xx/Source/GCC/startup_gd32f4xx.s',
    'src/main.cpp',
),
    dependencies: [spl_dep, cmsis_dep, hal_dep],
    include_directories: include_directories(
    'inc',
),
    c_args: ['-DGD32F4xx'],
    cpp_args: ['-DGD32F4xx'],
    link_args: [
        '-T', meson.source_root() / 'gd32f4xx_flash.ld',
        '-Wl,-Map=led.map'
    ],
    install: false,
)