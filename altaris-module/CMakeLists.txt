#####################################################################
# simple AVR executable
#####################################################################
add_avr_executable(
        altaris-module
        altaris-module.cpp
        altaris-module.h
)

#####################################################################
# libraries to link to
#####################################################################
target_link_libraries(
        altaris-module-${AVR_MCU}.elf
        UART-${AVR_MCU}
        utils-${AVR_MCU}
        wdt_power-${AVR_MCU}
        #SPI-${AVR_MCU}
)
