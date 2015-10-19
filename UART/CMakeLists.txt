#####################################################################
# simple AVR library
#####################################################################
add_avr_library(
        UART
        uart-sync.h
        uart-sync.cpp
        uart-sync-native.h
        uart-sync-native.cpp
        uart-async.h
        uart-async.cpp
        uart-async-native.h
        uart-async-native.cpp

)

