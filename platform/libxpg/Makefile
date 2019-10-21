# Project name

PROJ = libxpg.a

LIB_DEP = 

LIBS = $(addprefix -l,$(LIB_DEP))

TARGET = libxpg.a

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

AR=ar
ARFLAGS=rc

#build rule
.PHONY: all target install clean

all: $(COBJS)
	$(AR) $(ARFLAGS) $(TARGET) $(COBJS)

install:
	@cp -f $(TARGET) $(ROOTFS_BIN_DIR)

clean : 
	$(RM) $(COBJS) $(TARGET)


# Rules
include $(TOPDIR)/rules.mk
