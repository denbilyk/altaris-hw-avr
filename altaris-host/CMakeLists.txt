#####################################################################
# simple AVR executable
#####################################################################
add_avr_executable(
        altaris-host
        altaris-host.cpp
        altaris-host.h
        esp-network.cpp
        esp-network.h
)

#####################################################################
# libraries to link to
#####################################################################
target_link_libraries(
        altaris-host-${AVR_MCU}.elf
        hw-${AVR_MCU}
        UART-${AVR_MCU}
        utils-${AVR_MCU}
        wdt_power-${AVR_MCU}
        rf24-${AVR_MCU}
        softuart-${AVR_MCU}
        rf24-ard-${AVR_MCU}
)
