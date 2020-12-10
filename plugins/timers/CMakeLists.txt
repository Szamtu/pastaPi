project(Timers VERSION ${PastaPi_VERSION} LANGUAGES C CXX)

set(SOURCES
    clock/clock.cc
    clock/clock.h

    delta_time/delta_time.cc
    delta_time/delta_time.h

    t_off/t_off.cc
    t_off/t_off.h

    t_on/t_on.cc
    t_on/t_on.h

    t_pulse/t_pulse.cc
    t_pulse/t_pulse.h
    
    sunrise/sunrise.cc
    sunrise/sunrise.h

    timers.cc
)

set(GUI_SOURCES
    clock/clock_node.cc
    clock/clock_node.h
    
    sunrise/sunrise_node.cc
    sunrise/sunrise_node.h
    )

include(PluginBuild)
