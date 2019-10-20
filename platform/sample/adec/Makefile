# Project name

PROJ = ak_adec_sample

LIB_DEP := plat_common  plat_mem plat_thread pthread  plat_log plat_ao  plat_osal plat_drv mpi_adec 

LIB_DEP += akaudiofilter akaudiocodec

LIBS = $(addprefix -l, $(LIB_DEP))

TARGET = ak_adec_sample

TOPDIR 	?= $(shell pwd)/../..

# Flags

INCLUDE    =	-I. \
		-I../../include_inner \
		-I../../include \
		-I../../src/include \
		-I../include \

#Locations

SRCDIR += .

CSRCS += $(foreach d, $(SRCDIR), $(wildcard $d/*.c))

COBJS += $(patsubst %.c, %.o, $(CSRCS))

LIB_DIR = $(TOPDIR)/lib
RTSP_LIB_DIR := $(TOPDIR)/lib/rtsp_lib

LD_FLAGS = -Xlinker "-(" $(LIBS) -Xlinker "-)" -L$(LIB_DIR) -L$(RTSP_LIB_DIR)

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
