# Makefile for gcc for PIC32

# mipsel-elf-gcc & others should be in out PATH
# P32M_TOOLCHAIN_PATH can be provided by the PATH,
# or when running the job, ie. P32M_TOOLCHAIN_PATH=../.. make
# PIC32PROG_PATH is optional

# These 5 need to be provided.
# ARCH, FAMILY, and some other things are computed out of these.
# Makes for one change, instead of 10.
TARGET = main
MCU = 32MM0256GPM028
OPTIMIZATION = -O2
CRYSTALFREQUENCY = 8000000L
MIN_HEAP_SIZE = _min_heap_size=2048
MIN_STACK_SIZE = _min_stack_size=2048

P32M_TOOLCHAIN_PATH = /opt/pic32/pic32-parts-free
PIC32PROG_PATH = /opt/pic32/progyon

# Provide your source directories
BUILD_DIR = build
C_SRC_DIR = src $(P32M_TOOLCHAIN_PATH)/include/support/interrupt $(P32M_TOOLCHAIN_PATH)/include/support/syscalls
C_SRC_DIR += src/system src/drivers src/peripherals src/usb src/prog_interface src/cartridge
INCLUDE_DIR = inc
ASM_DIR = src/asm

# Compiler definitions. Don't touch.
CC = mipsel-elf-gcc
COPY = mipsel-elf-objcopy
SIZE = mipsel-elf-size

# The rest from here should automatically discern the needed scipts


MCU_XC = __$(strip $(MCU))__	# Convert to form used by xc.h. 
MCU_ASM_FILE = p$(MCU).S		# Definitions have a p prefix, and .S suffix

# Extract information about the family automatically.
ifeq ($(findstring 32MM, $(MCU)), 32MM)
FAMILY = __PIC32MM__
ARCH = -march=m14k -mmicromips -EL -msoft-float
LDSCRIPT_USED = elf32pic32mm.ld
endif
ifeq ($(findstring 32MX, $(MCU)), 32MX)
FAMILY = __PIC32MX__
ARCH = -march=m4k -EL -msoft-float
LDSCRIPT_USED = elf32pic32mx.ld
endif


LDSCRIPTS = -T$(P32M_TOOLCHAIN_PATH)/proc/$(MCU)/procdefs.ld -T$(P32M_TOOLCHAIN_PATH)/ldscripts/$(LDSCRIPT_USED)
STARTUPSCRIPTS = $(P32M_TOOLCHAIN_PATH)/libpic32/startup/crt0.S $(P32M_TOOLCHAIN_PATH)/libpic32/startup/general-exception.S
INTERRUPTSCRIPT = $(P32M_TOOLCHAIN_PATH)/include/support/interrupt/interrupt.S	# The new isrwrapper/interrupt system
# Added build dir to LDFLAGS, so xc.h can find processor.o
#LDFLAGS =  $(STARTUPSCRIPTS) $(INTERRUPTSCRIPT) $(LDSCRIPTS) -Wl,-undefined,dynamic_lookup -L build -lc -lm -lgcc $(DEBUG_L)
LDFLAGS =  $(STARTUPSCRIPTS) $(INTERRUPTSCRIPT) $(LDSCRIPTS) -Wl,-undefined,dynamic_lookup -L build -lc -lm -lgcc $(DEBUG_L) -lnosys
#LDFLAGS =  $(STARTUPSCRIPTS) $(INTERRUPTSCRIPT) $(LDSCRIPTS) -Wl,-undefined,dynamic_lookup -L build -lc -lm -lgcc $(DEBUG_L) -lnosys -u _printf_float

INCLUDES = -I$(INCLUDE_DIR) -I$(INCLUDE_DIR)/system -I$(INCLUDE_DIR)/drivers -I$(INCLUDE_DIR)/peripherals \
-I$(INCLUDE_DIR)/usb -I$(P32M_TOOLCHAIN_PATH)/include -I $(P32M_TOOLCHAIN_PATH)/include/support/interrupt \
-I$(INCLUDE_DIR)/prog_interface -I$(P32M_TOOLCHAIN_PATH)/include/sys \
-I$(INCLUDE_DIR)/cartridge
C_SOURCES = $(foreach dir, $(C_SRC_DIR), $(wildcard $(dir)/*.c)) 
ASM_SOURCES = $(foreach dir, $(ASM_DIR), $(wildcard $(dir)/*.s)) 
MCU_ASM = $(P32M_TOOLCHAIN_PATH)/proc/$(MCU)/$(MCU_ASM_FILE) # TODO BETTER


# Get files in folders & subfolders, build to BUILD_DIR
OBJ = $(addprefix $(BUILD_DIR)/, $(notdir $(C_SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(C_SOURCES)))
OBJ += $(addprefix $(BUILD_DIR)/, $(notdir $(ASM_SOURCES:.s=.o)))
vpath %.s $(sort $(dir $(ASM_SOURCES)))


CFLAGS = $(ARCH) -nostdlib $(OPTIMIZATION) -D $(FAMILY) -D $(MCU_XC) -D USB_USE_INTERRUPTS\
 -Wl,-defsym,$(MIN_HEAP_SIZE) -Wl,-defsym,$(MIN_STACK_SIZE) -Wl,-Map=$(BUILD_DIR)/output.map\
-Wall -ffunction-sections -fdata-sections -Wl,--gc-section -fdollars-in-identifiers $(DEBUG_C) #-Werror 
#

all: $(TARGET).hex
	
# Flash, debug print, skip verify (personal preference)
flash:
	$(PIC32PROG_PATH)/progyon $(BUILD_DIR)/$(TARGET).hex -D -S -s 4000

flash_icsp:
	$(PIC32PROG_PATH)/progyon $(BUILD_DIR)/$(TARGET).hex -D -S -i ICSP -s 4000


# For debug, recursively call the makefile again, with the DEBUG varible
# set to -g, so Debug symbols get built, and -D to define DEBUG_BUILD
# (useful to automate some cases with #ifdef later)
debug: 
	$(MAKE) $(MAKEFILE) DEBUG_C="-g -D DEBUG_BUILD" DEBUG_L="-Wl,--defsym=_DEBUGGER=1"

clean: 
	rm -f $(BUILD_DIR)/*.hex
	rm -f $(BUILD_DIR)/*.elf
	rm -f $(BUILD_DIR)/*.o
	rm -f $(BUILD_DIR)/*.map
	rm -f $(BUILD_DIR)/*.s

# Make objects from C
# $< evaluates to FILE.c
# $@ evaluates to FILE.o 
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(info $($@))
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Same for assembly files
$(BUILD_DIR)/%.o: %.s | $(BUILD_DIR)
	$(info $($@))
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Same for other assembly files, with upprecase letter
$(BUILD_DIR)/%.o: %.S | $(BUILD_DIR)
	$(info $($@))
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@


# Take for example p32mx250f128.s, and compile into processor.o (defines register locations)
# Input file should be p32mx ... .s (MCU_ASM)
# $@ evaluates to processor.o
processor.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $(MCU_ASM) -o $(BUILD_DIR)/$@

# target : prerequisites
# Compile .c files into .o, and processor.o (because name changes)
# Then link
$(TARGET).elf: $(OBJ) processor.o
	$(CC) $(CFLAGS) $(OBJ) $(BUILD_DIR)/processor.o $(LDFLAGS) $(INCLUDES) -o $(BUILD_DIR)/$@

# Make .elf
# LINK
# Convert .elf to .hex
# Show used space :)
$(TARGET).hex: $(TARGET).elf
	$(COPY) -O ihex $(BUILD_DIR)/$< $(BUILD_DIR)/$(TARGET).hex
	$(SIZE) $(BUILD_DIR)/$(TARGET).elf

# Create build directory. 
$(BUILD_DIR):
	mkdir $(BUILD_DIR)

