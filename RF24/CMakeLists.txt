#####################################################################
# simple AVR library
#####################################################################
add_avr_library(
        rf24
        RF24.h
        RF24.cpp
        nRF24L01.h
        hardware.cpp
        ../hardware/pins.cpp
        ../hardware/pins.h
)

