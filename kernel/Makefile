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
arch/i386/gdt.o \

OBJS=\
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \
$(ARCHDIR)/set_gdt.o \
$(ARCHDIR)/reload_gdt.o \


LINK_LIST=\
$(LDFLAGS) \
$(ARCHDIR)/crti.o \
$(ARCHDIR)/crtbegin.o \
$(KERNEL_OBJS) \
$(LIBS) \
$(ARCHDIR)/crtend.o \
$(ARCHDIR)/crtn.o \
$(ARCHDIR)/set_gdt.o \
$(ARCHDIR)/reload_gdt.o \

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
