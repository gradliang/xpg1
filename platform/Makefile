# rootfs global info

WORKDIR:=$(shell pwd)

ROOTFS_BIN_DIR?=$(WORKDIR)/rootfs/rootfs/usr/bin
ROOTFS_LIB_DIR?=$(WORKDIR)/rootfs/rootfs/usr/lib
ROOTFS_SBIN_DIR?=$(WORKDIR)/rootfs/rootfs/usr/sbin

MAKE:=make

export WORKDIR ROOTFS_BIN_DIR ROOTFS_LIB_DIR ROOTFS_SBIN_DIR MAKE

LIB_DIR:=$(WORKDIR)/lib
INC_DIR:=$(WORKDIR)/include
INC_INNER_DIR:=$(WORKDIR)/include_inner

export LIB_DIR INC_DIR INC_INNER_DIR

# rootfs dir MUST be placed at first

dir-y := rootfs
dir-y += sample

all:
	@for i in $(dir-y); \
	do \
		$(MAKE) -C $$i; \
		echo ""; \
		if [ $$? -ne 0 ]; then exit 1; fi \
	done

install:
	@for i in $(dir-y); \
	do \
		$(MAKE) -C $$i install; \
		echo ""; \
	done
	@cp -r lib/* $(ROOTFS_LIB_DIR)
	@cp ./rootfs/scripts/main.sh.rgb ./rootfs/rootfs/usr/sbin/main.sh
	@cp ./rootfs/app.sh ./rootfs/rootfs/etc/jffs2/
image:
	@for i in $(dir-y); \
	do \
		$(MAKE) -C $$i image; \
	done
	@cp rootfs/usr.* /mnt/hgfs/burntool-2
	@cp rootfs/root.s* /mnt/hgfs/burntool-2

clean:
	@for i in $(dir-y); \
	do \
		$(MAKE) -C $$i clean; \
		echo ""; \
	done

.PHONY: all install image clean
