project('GD32F4xx_Template', 'c', 'cpp',
    version: '1.0.0',
    default_options: [
        'warning_level=3',
        'werror=false',
        'layout=mirror',
        'cpp_std=c++23'
    ]
)
dependencies = [
  dependency('HAL'),
  dependency('gd32f4xx_spl'),
  dependency('CherryRB'),
  dependency('Hardware'),
  dependency('SEGGER_RTT'),
  dependency('template_schedule')
]


subdir('example')
