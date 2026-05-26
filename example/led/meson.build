executable('led.elf', files(
    'src/main.cpp',
),
    dependencies: dependencies,
    include_directories: include_directories(
    'inc',
),
    link_args: [
        '-Wl,-Map=led.map'
    ],
    install: false,
)