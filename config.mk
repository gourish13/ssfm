# ssfm version
VERSION = 0.1.0

PREFIX = /usr

# Libs
LIBS = -lncurses -lmenu -pthread

# Flags
SSFMLDFLAGS = $(LIBS) $(LDLIBS)

# Compiler and Linker
CC = c99
