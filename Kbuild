
CONFIG_SAMPLE_CHAR_DRIVER = m
CONFIG_SAMPLE_CHAR_DEVICE = m
# For debug
CONFIG_SAMPLE_CHAR_DEBUG = y

ifeq ($(CONFIG_SAMPLE_CHAR_DEBUG),y)
  DBGFLAGS = -O1 -g -DCONFIG_SAMPLE_CHAR_DEBUG
endif

EXTRA_CFLAGS += $(DBGFLAGS)
EXTRA_CFLAGS += -I$(LDDINC)


# Sample character device
sample-char-dev-objs := char-device.o char-utils.o
obj-$(CONFIG_SAMPLE_CHAR_DEVICE) += sample-char-dev.o
