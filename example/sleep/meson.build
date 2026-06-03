# 三个 target 演示: 调度器空闲钩子 + 低功耗模式
#   main1.cpp -> target1_sleep_idle.elf      (LED blink + idle pmu_to_sleepmode)
#   main2.cpp -> target2_deepsleep_idle.elf  (LED blink + idle pmu_to_deepsleepmode, WKUP wakeup)
#   main3.cpp -> target3_deepsleep_wkup.elf  (10 次闪烁后 pmu_to_deepsleepmode, WKUP wakeup)
foreach name_src : [
    ['target1_sleep_idle.elf',     'main1.cpp'],
    ['target2_deepsleep_idle.elf', 'main2.cpp'],
    ['target3_deepsleep_wkup.elf', 'main3.cpp'],
]
    executable(name_src[0], files(name_src[1]),
        dependencies: dependencies,
        link_args: ['-u', '_SEGGER_RTT'],
        install: false,
    )
endforeach
