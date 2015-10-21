#####################################################################
# simple AVR executable
#####################################################################
add_avr_executable(
        module-cfg-2
        module-cfg.cpp
)

#####################################################################
# libraries to link to
#####################################################################
target_link_libraries(
        module-cfg-2-${AVR_MCU}.elf
        altaris-module-core-${AVR_MCU}
        hw-${AVR_MCU}
        UART-${AVR_MCU}
        utils-${AVR_MCU}
        wdt_power-${AVR_MCU}
        rf24-${AVR_MCU}
        dht11-${AVR_MCU}
        ds1820-${AVR_MCU}
)
