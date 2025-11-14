PROJECT_NAME=myos.kernel
BUILD_DIR=../build
OBJ_DIR=$(BUILD_DIR)/objs

TARGET=$(BUILD_DIR)/$(PROJECT_NAME)

DEFAULT_HOST!=../default-host.sh
HOST?=DEFAULT_HOST
HOSTARCH!=../target-triplet-to-arch.sh $(HOST)

NASM?=nasm
NASMFLAGS?=-f elf32

CFLAGS?=-O2 -g
CPPFLAGS?=
LDFLAGS?=
LIBS?=

DESTDIR?=
PREFIX?=/usr/local
EXEC_PREFIX?=$(PREFIX)
BOOTDIR?=$(EXEC_PREFIX)/boot
INCLUDEDIR?=$(PREFIX)/include

CFLAGS:=$(CFLAGS) -ffreestanding -Wall -Wextra
CPPFLAGS:=$(CPPFLAGS) -D__is_kernel -Iinclude
LDFLAGS:=$(LDFLAGS)
LIBS:=$(LIBS) -nostdlib -lk -lgcc

ARCHDIR=arch/$(HOSTARCH)

include $(ARCHDIR)/make.config

CFLAGS:=$(CFLAGS) $(KERNEL_ARCH_CFLAGS)
CPPFLAGS:=$(CPPFLAGS) $(KERNEL_ARCH_CPPFLAGS)
LDFLAGS:=$(LDFLAGS) $(KERNEL_ARCH_LDFLAGS)
LIBS:=$(LIBS) $(KERNEL_ARCH_LIBS)

KERNEL_OBJS=\
$(KERNEL_ARCH_OBJS) \
kernel/kernel.o \
$(ARCHDIR)/gdt.o \
$(ARCHDIR)/c_idt.o \
$(ARCHDIR)/exception_handlers.o \
$(ARCHDIR)/pic_handler.o \
$(ARCHDIR)/global.o \
$(ARCHDIR)/ata_ide.o \
$(ARCHDIR)/c_default_interrupts.o \
$(ARCHDIR)/default_interrupts.o \

OJBS_START = \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \

OJBS_END = \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \
$(ARCHDIR)/set_gdt.o \
$(ARCHDIR)/reload_gdt.o \
$(ARCHDIR)/idt.o \
$(ARCHDIR)/pic.o \
$(ARCHDIR)/pic_asm.o \
$(ARCHDIR)/test_nasm.o \
$(ARCHDIR)/test_nasm_asm.o \

OBJS=\
$(OJBS_START) \
$(KERNEL_OBJS) \
$(OJBS_END) \


LINK_LIST=\
$(LDFLAGS) \
$(OJBS_START) \
$(KERNEL_OBJS) \
$(LIBS) \
$(OJBS_END) \

OBJS := $(patsubst %.o, $(OBJ_DIR)/%.o,$(OBJS))
LINK_LIST := $(patsubst %.o, $(OBJ_DIR)/%.o,$(LINK_LIST))

$(foreach dir,$(sort $(dir $(OBJS))),$(shell mkdir -p $(dir)))

.PHONY: all clean install install-headers install-kernel
.SUFFIXES: .o .c .S

all: $(TARGET) 

$(TARGET): $(OBJS) $(ARCHDIR)/linker.ld 
	$(CC) -T $(ARCHDIR)/linker.ld -o $@ $(CFLAGS) $(LINK_LIST)
	grub-file --is-x86-multiboot $(TARGET) 

$(OBJ_DIR)/$(ARCHDIR)/crtbegin.o $(OBJ_DIR)/$(ARCHDIR)/crtend.o:
	OBJ=`$(CC) $(CFLAGS) $(LDFLAGS) -print-file-name=$(@F)` && cp "$$OBJ" $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) -MD -c $< -o $@ -std=gnu11 $(CFLAGS) $(CPPFLAGS)

$(OBJ_DIR)/%.o: %.S | $(OBJ_DIR)
	$(NASM) $(NASMFLAGS) -o $@ $<

$(OBJ_DIR)/%.o: %.s | $(OBJ_DIR)
	$(NASM) $(NASMFLAGS) -o $@ $<

$(BUILD_DIR):
	mkdir $(BUILD_DIR)

$(OBJ_DIR): | $(BUILD_DIR)
	mkdir $(OBJ_DIR)

clean:
	rm -rf $(BUILD_DIR)

install: install-headers install-kernel

install-headers:
	mkdir -p $(DESTDIR)$(INCLUDEDIR)
	cp -R --preserve=timestamps include/. $(DESTDIR)$(INCLUDEDIR)/.

install-kernel: $(TARGET) 
	mkdir -p $(DESTDIR)$(BOOTDIR)
	cp $(TARGET) $(DESTDIR)$(BOOTDIR)

-include $(OBJS:.o=.d)
