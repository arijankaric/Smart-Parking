# Put your stlink folder here so make burn will work.
STLINK= stlink

# Binaries will be generated with this name (.elf, .bin, .hex, etc)
PROJ_NAME = main
BUILD_DIR = Build # at this directory
BUILD_OUTPUT = ./Build/main

# Put your STM32F4 library code directory here
STM_COMMON=sdk
STM32CUBEPROG:= stmcubeprog/bin/STM32_Programmer.sh -vb 1 -q -c port=SWD 

# Put your source files here (or *.c, etc)
SRCS = main.c	#SRCS = Src/main.c 
SRCS += STM32_Init.c	#SRCS += Src/STM32_Init.c
SRCS += fonts1.c	#SRCS += Src/fonts1.c
SRCS += keypad.c	#SRCS += Src/keypad.c
SRCS += oledDisplay.c	#SRCS += Src/oledDisplay.c
SRCS += servo.c	#SRCS += Src/servo.c
SRCS += ultrasound.c	#SRCS += Src/ultrasound.c
SRCS += obstacleSensor.c	#SRCS += Src/obstacleSensor.c
# Reason for not pushing fatfs .c/.h files is they were not really written by us
# Build files are usually not pushed on repo but since this repo is not meant for a client...
# Obviously this build/make also doesn't work without the HAL_Drivers and gcc-arm-none-eabi compiler, stm32_flash.ld script, ...
SRCS += system_stm32f4xx.c Src/stm32f4xx_it.c #SRCS += Src/system_stm32f4xx.c Src/stm32f4xx_it.c
SRCS += usart.c #SRCS += Src/usart.c
SRCS += misc.c	#SRCS += Src/misc.c
SRCS += user_diskio.c	#SRCS += Src/user_diskio.c
SRCS += fatfs.c	#SRCS += Src/fatfs.c
SRCS += fatfs_sd.c	#SRCS += Src/fatfs_sd.c
SRCS += ff_gen_drv.c	#SRCS += Src/ff_gen_drv.c
SRCS += ff.c	#SRCS += Src/ff.c
SRCS += diskio.c	#SRCS += Src/diskio.c
SRCS += ccsbcs.c	#SRCS += Src/ccsbcs.c 


# HAL Driver
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc_ex.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ramfunc.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_usart.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma_ex.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc_ex.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr_ex.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_exti.c 
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_timebase_tim_template.c
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c
SRCS += $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c


# Normally you shouldn't need to change anything below this line!
#######################################################################################
GNUGCC = $(STM_COMMON)/gcc-arm-none-eabi/bin
CC = $(GNUGCC)/arm-none-eabi-gcc
OBJCOPY = $(GNUGCC)/arm-none-eabi-objcopy
SIZE =  $(GNUGCC)/arm-none-eabi-size


CFLAGS  = -g -O2 -Wall -Tstm32_flash.ld -std=c11
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
# important flag is -fsingle-precision-constant which prevents the double precision emulation
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16 -fsingle-precision-constant
CFLAGS += -I. -I ./Inc

# Include files from STM libraries
CFLAGS += -I $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Inc
CFLAGS += -I $(STM_COMMON)/Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
CFLAGS += -I $(STM_COMMON)/Drivers/CMSIS/Include 
CFLAGS += -I $(STM_COMMON)/Drivers/CMSIS/Device/ST/STM32F4xx/Include
#CFLAGS += -I$(STM_COMMON)/Utilities/STM32F4-Discovery
#CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/Include 
#CFLAGS += -I$(STM_COMMON)/Libraries/CMSIS/ST/STM32F4xx/Include
#CFLAGS += -I$(STM_COMMON)/Libraries/STM32F4xx_StdPeriph_Driver/inc

# add startup file to build
SRCS += $(STM_COMMON)/Drivers/startup_stm32f4xx.s
OBJS = $(SRCS:.c=.o)


.PHONY: proj

all: $(BUILD_OUTPUT).elf

$(BUILD_OUTPUT).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -lm -lc -lnosys -o $@ 
	$(CC) $(CFLAGS) -S $< $^
	$(OBJCOPY) -O ihex $(BUILD_OUTPUT).elf $(BUILD_OUTPUT).hex
	$(OBJCOPY) -O binary $(BUILD_OUTPUT).elf $(BUILD_OUTPUT).bin
	$(SIZE) -B  $(BUILD_OUTPUT).elf
	rm -rf *.o *.s
	ls -l $(BUILD_OUTPUT).bin



clean:
	rm -rf *.o $(BUILD_OUTPUT).elf $(BUILD_OUTPUT).hex $(BUILD_OUTPUT).bin *.s
	ls

# Flash the STM32F4
upload: proj
	@$(STM32CUBEPROG) -w $(BUILD_OUTPUT).bin  0x08000000 
	@sleep 1
	@$(STM32CUBEPROG) -hardRst 

