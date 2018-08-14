# xst - simple terminal fork with xresources support and other patches
# See LICENSE file for copyright and license details.

VERSION = 0.7

# paths
PREFIX = /usr/local
MANPREFIX = ${PREFIX}/share/man

X11INC = /usr/X11R6/include
X11LIB = /usr/X11R6/lib

# includes and libs
INCS = -I. -I/usr/include -I${X11INC} \
       `pkg-config --cflags fontconfig` \
       `pkg-config --cflags freetype2`
LIBS = -L/usr/lib -lc -L${X11LIB} -lm -lrt -lX11 -lutil -lXext -lXft -lXrender \
       `pkg-config --libs fontconfig`  \
       `pkg-config --libs freetype2`

# flags
CPPFLAGS = -DVERSION=\"${VERSION}\" -D_XOPEN_SOURCE=600
CFLAGS += -g -std=c99 -pedantic -Wall -Wvariadic-macros -Os ${INCS} ${CPPFLAGS}
LDFLAGS += -g ${LIBS}

# compiler and linker
# CC = cc

SRC = src/xst.c
OBJ = ${SRC:.c=.o}

all: options xst

options:
	@echo xst build options:
	@echo "CFLAGS   = ${CFLAGS}"
	@echo "LDFLAGS  = ${LDFLAGS}"
	@echo "CC       = ${CC}"

.c.o:
	@echo CC $<
	@${CC} -o $@ -c ${CFLAGS} $<

${OBJ}: src/config.h

xst: ${OBJ}
	@echo CC -o $@
	@${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	@echo cleaning
	@rm -f xst ${OBJ} xst-${VERSION}.tar.gz

dist: clean
	@echo creating dist tarball
	@mkdir -p xst-${VERSION}
	@cp -R LICENSE Makefile README doc/xst.info doc/xst.1 src/arg.h ${SRC} xst-${VERSION}
	@tar -cf xst-${VERSION}.tar xst-${VERSION}
	@gzip xst-${VERSION}.tar
	@rm -rf xst-${VERSION}

install: all
	@echo installing executable file to ${DESTDIR}${PREFIX}/bin
	@mkdir -p ${DESTDIR}${PREFIX}/bin
	@cp -f xst ${DESTDIR}${PREFIX}/bin
	@chmod 755 ${DESTDIR}${PREFIX}/bin/xst
	@echo installing manual page to ${DESTDIR}${MANPREFIX}/man1
	@mkdir -p ${DESTDIR}${MANPREFIX}/man1
	@sed "s/VERSION/${VERSION}/g" < doc/xst.1 > ${DESTDIR}${MANPREFIX}/man1/xst.1
	@chmod 644 ${DESTDIR}${MANPREFIX}/man1/xst.1
	@echo Please see the README file regarding the terminfo entry of xst.
	@mkdir -p ${DESTDIR}/${PREFIX}/share/terminfo
	@tic -o ${DESTDIR}/${PREFIX}/share/terminfo -sx doc/xst.info 

uninstall:
	@echo removing executable file from ${DESTDIR}${PREFIX}/bin
	@rm -f ${DESTDIR}${PREFIX}/bin/xst
	@echo removing manual page from ${DESTDIR}${MANPREFIX}/man1
	@rm -f ${DESTDIR}${MANPREFIX}/man1/xst.1

.PHONY: all options clean dist install uninstall
