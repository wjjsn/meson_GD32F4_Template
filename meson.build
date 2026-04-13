project('GD32F4xx_Template', 'c', 'cpp',
    version: '1.0.0',
    default_options: [
        'warning_level=everything',
        'werror=false',
        'layout=mirror',
        'cpp_std=c++23'
    ]
)


subdir('lib/CMSIS')
subdir('lib/GD32F4xx_standard_peripheral')

sub = subproject('HAL')
hal_dep = sub.get_variable('hal_dep')

subdir('example')
