project('GD32F4xx_Template', 'c', 'cpp',
    version: '1.0.0',
    default_options: [
        'warning_level=everything',
        'werror=false',
        'layout=mirror',
    ]
)

add_project_arguments('-DGD32F4xx', language: 'c')

spl_files = files(
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_adc.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_can.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_crc.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_ctc.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_dac.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_dci.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_dma.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_enet.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_exmc.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_exti.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_fmc.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_fwdgt.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_gpio.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_i2c.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_ipa.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_iref.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_misc.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_pmu.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_rcu.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_rtc.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_sdio.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_spi.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_syscfg.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_timer.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_tli.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_trng.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_usart.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_wwdgt.c',
    'Driver/GD32F4xx_standard_peripheral/Source/gd32f4xx_dbg.c',
    'Driver/CMSIS/GD/GD32F4xx/Source/system_gd32f4xx.c',
)

spl_inc = include_directories(
    'Driver/GD32F4xx_standard_peripheral/Include',
    'Driver/CMSIS',
    'Driver/CMSIS/GD/GD32F4xx/Include',
)

spl_lib = static_library('GD32F4xx_SPL', spl_files,
    include_directories: spl_inc,
)

source_files = files(
    'Driver/CMSIS/GD/GD32F4xx/Source/GCC/startup_gd32f4xx.s',
    'Core/Src/main.c',
    'Core/Src/syscalls.c',
    'Core/Src/gpio.c',
    'Core/Src/led.c',
)

app_inc = include_directories(
    'Core/Inc',
    'Driver/GD32F4xx_standard_peripheral/Include',
    'Driver/CMSIS',
    'Driver/CMSIS/GD/GD32F4xx/Include',
)

executable('GD32F4xx_Template.elf', source_files,
    link_with: spl_lib,
    include_directories: app_inc,
    link_args: [
        '-T', meson.source_root() / 'gd32f4xx_flash.ld',
        '--specs=nano.specs',
        '-Wl,-Map=GD32F4xx_Template.map',
        '-Wl,--gc-sections',
        '-Wl,--print-memory-usage',
    ],
    install: false,
)
