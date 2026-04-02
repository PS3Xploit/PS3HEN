CC			:= ppu-gcc
CXX			:= ppu-g++
LD			:= ppu-ld 
OBJCOPY		:= ppu-objcopy
OBJDUMP		:= ppu-objdump
AR 			:= ppu-ar
STRIP		:= ppu-strip
INCLUDE		=  ../lv2/include ../lv1/include ../cryptcode/include ../debug/include
INCLUDE		+= $(PS3DEV)/ppu/ppu/include
LIBSDIR     =
LIBS        =
OUTPUT		= stage2.bin

FIRMWARES_RELEASE = 480C 481C 482C 483C 484C 485C 486C 487C 488C 489C 490C 491C 492C 493C
FIRMWARES_DEBUG   = 480C 481C 482D 482C 483C 484D 484C 485C 486C 487C 488C 489C 490C 491C 492C 493C

FIRMWARE   ?= 493C
BUILD_TYPE ?= release

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
			   -Os -Wall -Wno-strict-aliasing -Wno-multichar $(foreach dir,$(INCLUDE),-I$(dir)) \
			   -DLV2 $(FIRMWARE_DEFINE) -DCFW -ffreestanding 
CFLAGS += --std=gnu99

ifeq ($(BUILD_TYPE), debug)
CFLAGS += -DDEBUG -DTEST 
endif

ifeq ($(BUILD_TYPE), ps2_debug)
CFLAGS += -DDEBUG -DPS2EMU_DEBUG -DTEST
endif

ifeq ($(BUILD_TYPE), test)
CFLAGS += -DTEST
endif

#CFLAGS += -DPSN_SUPPORT


#LDFLAGS= -T stage2.ld -nostartfiles -nostdlib -nodefaultlibs -Wl,-static -Wl,-s -L. $(foreach dir,$(LIBSDIR),-L$(dir)) $(LIBS) \
#	-Wl,--gc-sections -Wl,-Map=stage2.map
LDFLAGS= -T stage2.ld -nostartfiles -nostdlib -nodefaultlibs -Wl,-static -L. $(foreach dir,$(LIBSDIR),-L$(dir)) $(LIBS) \
	 -Wl,--gc-sections -Wl,-Map=stage2.map

OBJS = $(START_OBJ) psp.o psp_s.o ps3mapi_core.o main.o crypto.o modulespatch.o config.o mappath.o storage_ext.o region.o cobra.o sm_ext.o \
	 make_rif.o homebrew_blocker.o \
	../lv2/src/usb.o ../lv2/src/patch.o ../lv2/src/interrupt.o ../lv2/src/interrupt_c.o ../lv2/src/io.o ../lv2/src/libc.o \
	../lv2/src/libc_c.o ../lv2/src/memory.o ../lv2/src/memory_c.o ../lv2/src/thread.o ../lv2/src/thread_c.o ../lv2/src/process.o \
	../lv2/src/synchronization.o ../lv2/src/modules.o ../lv2/src/modules_c.o ../lv2/src/storage.o ../lv2/src/object.o \
	../lv2/src/security.o ../lv2/src/time.o ../lv2/src/hid.o ../lv2/src/pad.o ../lv2/src/syscall.o \
	../lv1/src/hvcall.o ../lv1/src/stor.o ../lv1/src/device.o ../lv1/src/mm.o ../lv1/src/patch.o

ifeq ($(BUILD_TYPE), release)
OBJS += laboratory.o
endif

ifeq ($(BUILD_TYPE), debug)
OBJS += laboratory.o ../debug/src/debug.o ../debug/src/printf.o ../debug/src/debug_util.o
endif

ifeq ($(BUILD_TYPE), ps2_debug)
OBJS += laboratory.o ../debug/src/debug.o ../debug/src/printf.o ../debug/src/debug_util.o
endif

ifeq ($(BUILD_TYPE), debug)
OUTPUT_SUFFIX := _debug
else ifeq ($(BUILD_TYPE), ps2_debug)
OUTPUT_SUFFIX := _debug
else
OUTPUT_SUFFIX :=
endif

RESULT_FILE = ../stage2.bin$(OUTPUT_SUFFIX)_$(FIRMWARE)

all: release debug

release:
	@for fw in $(FIRMWARES_RELEASE); do \
		$(MAKE) --no-print-directory clean-objs; \
		$(MAKE) --no-print-directory build-one FIRMWARE=$$fw BUILD_TYPE=release || exit $$?; \
	done
	@$(MAKE) --no-print-directory clean-objs

debug:
	@for fw in $(FIRMWARES_DEBUG); do \
		$(MAKE) --no-print-directory clean-objs; \
		$(MAKE) --no-print-directory build-one FIRMWARE=$$fw BUILD_TYPE=debug || exit $$?; \
	done
	@$(MAKE) --no-print-directory clean-objs

single:
	@$(MAKE) --no-print-directory clean-objs
	@$(MAKE) --no-print-directory build-one FIRMWARE=$(FIRMWARE) BUILD_TYPE=$(BUILD_TYPE)
	@$(MAKE) --no-print-directory clean-objs

build-one: $(OUTPUT)
	@mv $(OUTPUT) $(RESULT_FILE)
	@rm -f *.o *.elf *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o

objdump: $(OUTPUT)
	$(OBJDUMP) -D -EB -b binary -m powerpc:common64 $(OUTPUT)	

%.bin: %.elf
	$(OBJCOPY) -O binary $< $@	

stage2.elf: $(OBJS) stage2.ld
	$(CC) $(LDFLAGS) -o $@ $(OBJS)

%.o: %.S
	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean-objs:
	@rm -f *.o *.elf *.bin *.release *.debug *.cex *.dex *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o

clean:
	rm -f *.o *.elf *.bin *.release *.debug *.cex *.dex *.map *.lzma ../lv2/src/*.o ../lv1/src/*.o ../debug/src/*.o
