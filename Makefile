# Variables
CC = gcc
CFLAGS = -Wall -O2
TARGET = tpSwipe
INSTALL_PATH = /usr/local/bin

# 1. Detect which library is present
HAS_ELOGIND := $(shell pkg-config --exists libelogind && echo yes)
HAS_SYSTEMD := $(shell pkg-config --exists libsystemd && echo yes)

# 2. Set flags based on detection
ifeq ($(HAS_ELOGIND),yes)
    LOGIN_LIBS = $(shell pkg-config --libs libelogind)
    LOGIN_CFLAGS = $(shell pkg-config --cflags libelogind) -DUSE_ELOGIND
else ifeq ($(HAS_SYSTEMD),yes)
    LOGIN_LIBS = $(shell pkg-config --libs libsystemd)
    LOGIN_CFLAGS = $(shell pkg-config --cflags libsystemd)
else
    $(error "Neither libelogind nor libsystemd found. Please install one.")
endif

# 3. Final Build Variables
LIBS = -linput -ludev $(LOGIN_LIBS) -lm
FULL_CFLAGS = $(CFLAGS) $(LOGIN_CFLAGS)

all: $(TARGET)

$(TARGET): swipe.c config.h
	$(CC) $(FULL_CFLAGS) swipe.c $(LIBS) -o $(TARGET)

clean:
	rm -f $(TARGET)

install: $(TARGET)
	sudo install -m 755 $(TARGET) $(INSTALL_PATH)/$(TARGET)

uninstall:
	sudo rm -f $(INSTALL_PATH)/$(TARGET)

.PHONY: all clean install uninstall

