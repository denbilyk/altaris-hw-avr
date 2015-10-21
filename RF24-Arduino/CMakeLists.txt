#####################################################################
# simple AVR library
#####################################################################
add_avr_library(
        rf24-ard
        RF24-arduino.h
        RF24-arduino.cpp
        nRF24L01-arduino.h
        ../hardware/pins.cpp
        ../hardware/pins.h
)

