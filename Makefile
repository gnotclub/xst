# st - simple terminal
# See LICENSE file for copyright and license details.
.POSIX:

include config.mk

SRC = st.c x.c boxdraw.c
OBJ = $(SRC:.c=.o)

all: options st

options:
	@echo st build options:
	@echo "CFLAGS  = $(STCFLAGS)"
	@echo "LDFLAGS = $(STLDFLAGS)"
	@echo "CC      = $(CC)"

config.h:
	cp config.def.h config.h

.c.o:
	$(CC) $(STCFLAGS) -c $<

st.o: config.h st.h win.h
x.o: arg.h config.h st.h win.h
boxdraw.o: config.h st.h boxdraw_data.h

$(OBJ): config.h config.mk

st: $(OBJ)
	$(CC) -o xst $(OBJ) $(STLDFLAGS)

clean:
	rm -f xst config.h $(OBJ) st-$(VERSION).tar.gz

dist: clean
	mkdir -p st-$(VERSION)
	cp -R FAQ LEGACY TODO LICENSE Makefile README config.mk\
		config.def.h st.info st.1 arg.h st.h win.h $(SRC)\
		st-$(VERSION)
	tar -cf - st-$(VERSION) | gzip > st-$(VERSION).tar.gz
	rm -rf st-$(VERSION)

install: st
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f xst $(DESTDIR)$(PREFIX)/bin
	chmod 755 $(DESTDIR)$(PREFIX)/bin/xst
	mkdir -p $(DESTDIR)$(MANPREFIX)/man1
	sed "s/VERSION/$(VERSION)/g" < st.1 > $(DESTDIR)$(MANPREFIX)/man1/xst.1
	chmod 644 $(DESTDIR)$(MANPREFIX)/man1/xst.1
	mkdir -p $(DESTDIR)$(PREFIX)/share/terminfo
	env TERMINFO=$(DESTDIR)$(PREFIX)/share/terminfo tic -sx st.info

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/xst
	rm -f $(DESTDIR)$(MANPREFIX)/man1/xst.1
	rm -f $(DESTDIR)$(PREFIX)/share/terminfo/x/xst
	rm -f $(DESTDIR)$(PREFIX)/share/terminfo/x/xst-256color
	rm -f $(DESTDIR)$(PREFIX)/share/terminfo/x/xst-bs
	rm -f $(DESTDIR)$(PREFIX)/share/terminfo/x/xst-bs-256color
	rm -f $(DESTDIR)$(PREFIX)/share/terminfo/x/xst-meta
	rm -f $(DESTDIR)$(PREFIX)/share/terminfo/x/xst-meta-256color
	rm -f $(DESTDIR)$(PREFIX)/share/terminfo/x/xst-mono

.PHONY: all options clean dist install uninstall
