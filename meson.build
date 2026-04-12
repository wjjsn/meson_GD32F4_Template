project('GD32F4xx_Template', 'c', 'cpp',
    version: '1.0.0',
    default_options: [
        'warning_level=everything',
        'werror=false',
        'layout=mirror',
    ]
)

add_project_arguments('-DGD32F4xx', language: 'c')

subdir('lib/CMSIS')
subdir('lib/GD32F4xx_standard_peripheral')

source_files = files(
    'lib/CMSIS/GD/GD32F4xx/Source/GCC/startup_gd32f4xx.s',
    'app/Src/main.c',
    'app/Src/syscalls.c',
    'app/Src/gpio.c',
    'app/Src/led.c',
)

app_inc = include_directories(
    'app/Inc',
)

executable('GD32F4xx_Template.elf', source_files,
    dependencies: [spl_dep, cmsis_dep],
    include_directories: app_inc,
    link_args: [
        '-T', meson.source_root() / 'gd32f4xx_flash.ld',
        '-Wl,-Map=GD32F4xx_Template.map'
    ],
    install: false,
)
