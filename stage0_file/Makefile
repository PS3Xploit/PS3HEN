CC			:= ppu-gcc
CXX			:= ppu-g++
LD			:= ppu-ld 
OBJCOPY		:= ppu-objcopy
AR 			:= ppu-ar
STRIP		:= ppu-strip
INCLUDE		=  ../lv2/include ../lv1/include ../debug/include
INCLUDE		+= $(PS3DEV)/ppu/ppu/include
LIBSDIR		=
LIBS 		=
TARGET		= stage0

FIRMWARES_RELEASE = 480C 481C 482C 483C 484C 485C 486C 487C 488C 489C 490C 491C 492C 493C
FIRMWARES_ALL     = 480C 481C 482D 482C 483C 484D 484C 485C 486C 487C 488C 489C 490C 491C 492C 493C

FIRMWARE ?= 493C

ifeq ($(FIRMWARE),480C)
FIRMWARE_DEFINE := -DFIRMWARE_4_80
START_OBJ := start_490.o
else ifeq ($(FIRMWARE),481C)
FIRMWARE_DEFINE := -DFIRMWARE_4_81
START_OBJ := start.o
else ifeq ($(FIRMWARE),482C)
FIRMWARE_DEFINE := -DFIRMWARE_4_82
START_OBJ := start.o
else ifeq ($(FIRMWARE),482D)
FIRMWARE_DEFINE := -DFIRMWARE_4_82DEX
START_OBJ := start_DEX.o
else ifeq ($(FIRMWARE),483C)
FIRMWARE_DEFINE := -DFIRMWARE_4_83
START_OBJ := start.o
else ifeq ($(FIRMWARE),484C)
FIRMWARE_DEFINE := -DFIRMWARE_4_84
START_OBJ := start.o
else ifeq ($(FIRMWARE),484D)
FIRMWARE_DEFINE := -DFIRMWARE_4_84DEX
START_OBJ := start_DEX.o
else ifeq ($(FIRMWARE),485C)
FIRMWARE_DEFINE := -DFIRMWARE_4_85
START_OBJ := start.o
else ifeq ($(FIRMWARE),486C)
FIRMWARE_DEFINE := -DFIRMWARE_4_86
START_OBJ := start.o
else ifeq ($(FIRMWARE),487C)
FIRMWARE_DEFINE := -DFIRMWARE_4_87
START_OBJ := start.o
else ifeq ($(FIRMWARE),488C)
FIRMWARE_DEFINE := -DFIRMWARE_4_88
START_OBJ := start.o
else ifeq ($(FIRMWARE),489C)
FIRMWARE_DEFINE := -DFIRMWARE_4_89
START_OBJ := start.o
else ifeq ($(FIRMWARE),490C)
FIRMWARE_DEFINE := -DFIRMWARE_4_90
START_OBJ := start_490.o
else ifeq ($(FIRMWARE),491C)
FIRMWARE_DEFINE := -DFIRMWARE_4_91
START_OBJ := start.o
else ifeq ($(FIRMWARE),492C)
FIRMWARE_DEFINE := -DFIRMWARE_4_92
START_OBJ := start.o
else ifeq ($(FIRMWARE),493C)
FIRMWARE_DEFINE := -DFIRMWARE_4_93
START_OBJ := start.o
else
$(error Unsupported firmware $(FIRMWARE))
endif

CFLAGS = -ffunction-sections -fdata-sections -Os -m64 -fno-builtin -fno-exceptions \
			   -Os -Wall $(foreach dir,$(INCLUDE),-I$(dir)) \
			   -DLV2 $(FIRMWARE_DEFINE) -ffreestanding
CFLAGS += --std=gnu99
LDFLAGS = -T $(TARGET).ld -nostartfiles -nostdlib -nodefaultlibs -Wl,-static -Wl,-s -L. \
		   $(foreach dir,$(LIBSDIR),-L$(dir)) $(LIBS) -Wl,--gc-sections -Wl

OBJS = $(START_OBJ) unregister_services.o main.o ../lv2/src/libc.o ../lv2/src/memory.o ../lv2/src/io.o ../lv2/src/thread.o ../lv1/src/hvcall.o

OUTPUT = $(TARGET).bin
RESULT_FILE = ../stage0.bin_$(FIRMWARE)

all: release

release:
	@for fw in $(FIRMWARES_RELEASE); do \
		$(MAKE) --no-print-directory clean-objs; \
		$(MAKE) --no-print-directory build-one FIRMWARE=$$fw || exit $$?; \
	done
	@$(MAKE) --no-print-directory clean-objs

all_firmwares:
	@for fw in $(FIRMWARES_ALL); do \
		$(MAKE) --no-print-directory clean-objs; \
		$(MAKE) --no-print-directory build-one FIRMWARE=$$fw || exit $$?; \
	done
	@$(MAKE) --no-print-directory clean-objs

single:
	@$(MAKE) --no-print-directory clean-objs
	@$(MAKE) --no-print-directory build-one FIRMWARE=$(FIRMWARE)
	@$(MAKE) --no-print-directory clean-objs

build-one: $(OUTPUT)
	@mv $(OUTPUT) $(RESULT_FILE)
	@rm -f *.o *.elf *.self *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@

$(TARGET).elf: $(OBJS) $(TARGET).ld
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean-objs:
	@rm -f *.o *.elf *.self *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o

clean:
	rm -f *.o *.elf *.self *.bin *.map ../lv1/src/*.o ../debug/src/*.o ../lv2/src/*.o
