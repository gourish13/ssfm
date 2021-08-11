# ssfm - Simply Stupid File Manager
# Hava a look at the LICENSE file for copyright and license details.

include config.mk

SRC = ssfm.c
OBJ = $(SRC:.c=.o)

all: options ssfm

options:
	@echo Building ssfm with these options:
	@echo "LDFLAGS = $(SSFMLDFLAGS)"
	@echo "CC = $(CC)"

config.h:
	cp -f config.def.h config.h

.c.o:
	$(CC) -c $<

ssfm.o: config.h ssfm.h

$(OBJ): config.h config.mk

ssfm: $(OBJ)
	$(CC) -o $@ $(OBJ) $(SSFMLDFLAGS)

clean:
	rm -f ssfm
	rm -f *.o
	rm config.h

install: ssfm
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f ssfm $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/ssfm

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/ssfm

.PHONY: all options clean install uninstall
