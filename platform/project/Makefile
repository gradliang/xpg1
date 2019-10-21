# Project name

PROJ = project1
TARGET = project1

LIB_DEP = #plat_ao plat_common plat_log plat_mem plat_thread pthread akaudiofilter plat_osal plat_drv 

LIBS = $(addprefix -l,$(LIB_DEP))
LIBS += -lxpg 

TOPDIR 	?= $(shell pwd)/..

# Flags

INCLUDE    =	-I. \
		-I../include_inner \
		-I../include \
		-Iinclude \

#Locations

SRCDIR += source

CSRCS += $(foreach d, $(SRCDIR), $(wildcard $d/*.c))

COBJS += $(patsubst %.c, %.o, $(CSRCS))

LIB_DIR = $(TOPDIR)/lib

LD_FLAGS = $(LIBS) -L$(LIB_DIR)

#build rule
.PHONY: all target install clean

all: $(COBJS)
	$(CC) $(COBJS) $(LD_FLAGS) -o $(TARGET)

install:
	@cp -f $(TARGET) $(ROOTFS_BIN_DIR)

clean : 
	$(RM) $(COBJS) $(TARGET)


# Rules
include $(TOPDIR)/rules.mk
