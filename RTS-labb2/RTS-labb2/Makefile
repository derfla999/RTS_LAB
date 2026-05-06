.PHONY: all clean console

# app name fallback
SPACE := $(null) $(null)
APP_NAME = $(notdir $(subst $(SPACE),-,$(CURDIR)))

# output files
TARGET_ELF = $(BUILD_DIR)/$(APP_NAME).elf
TARGET_S19 = $(BUILD_DIR)/$(APP_NAME).s19
TARGET_LST = $(BUILD_DIR)/$(APP_NAME).lst

# linker script
LINKER_SCRIPT = dev/md407-ram.x

# source directories
BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
SRC_DIRS = src lib/tinytimber/src lib/md407/src
INC_DIRS = $(SRC_DIRS:src=inc)
OBJ_DIRS = $(addprefix $(OBJ_DIR)/, $(SRC_DIRS))

# source files
SRCS := $(foreach d, $(SRC_DIRS), $(wildcard $(d)/*.c) $(wildcard $(d)/*.s))
OBJS := $(SRCS:%=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

# toolchain paths
CC = "$(GCC_PATH)arm-none-eabi-gcc"
OBJCOPY = "$(GCC_PATH)arm-none-eabi-objcopy"
OBJDUMP = "$(GCC_PATH)arm-none-eabi-objdump"

# compiler, linker, and generator flags
CFLAGS += -g \
          -O0 \
          -Wall \
          -MMD \
          -mcpu=cortex-m4 \
          -mfpu=fpv4-sp-d16 \
          -mthumb \
          -mfloat-abi=hard \
          -D STM32F40_41xxx \
          $(addprefix -I, $(INC_DIRS))
LDFLAGS += -nostartfiles \
           -specs=nano.specs \
           -mcpu=cortex-m4 \
           -mfpu=fpv4-sp-d16 \
           -mthumb \
           -mfloat-abi=hard \
           -T $(LINKER_SCRIPT)
OBJCOPY_FLAGS += -S -O srec
OBJDUMP_FLAGS += -S

# compiler, standard, local, and math libraries
LDLIBS += -lgcc -lc_nano -lm

# directory commands
ifeq ($(OS), Windows_NT)
  MKDIR = powershell mkdir -Force
  RM = powershell rm -Force
else
  MKDIR = mkdir -p
endif

# main target
all: $(TARGET_S19) $(TARGET_LST)

# generate s-records
$(TARGET_S19): $(TARGET_ELF)
	$(OBJCOPY) $(OBJCOPY_FLAGS) $< $@

# link object files into executable
$(TARGET_ELF): $(OBJS)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

# display assembler mnemonics
$(TARGET_LST): $(TARGET_ELF)
	$(OBJDUMP) $(OBJDUMP_FLAGS) $< > $@

# create folders for object files
$(OBJ_DIRS):
	$(MKDIR) $@

# compile source files
$(OBJ_DIR)/%.o: % | $(OBJ_DIRS)
	$(CC) $(CFLAGS) -c $< -o $@

# remove build directory
clean:
	$(RM) -r $(BUILD_DIR)

# communicate with MD407 (non-IDE users on Linux/MacOS)
console: $(TARGET_S19)
	bin/$@ $<

-include $(DEPS)
