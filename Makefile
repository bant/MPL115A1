#
# Makefile:
#	Copyright (c) 2013 BANT
#################################################################################

DESTDIR=/usr
PREFIX=/local

#DEBUG	= -g -O0
DEBUG	= -O2
CC	= gcc
INCLUDE	= -I$(DESTDIR)$(PREFIX)/include
CFLAGS	= $(DEBUG) -Wall $(INCLUDE) -Winline -pipe

LDFLAGS	= -L$(DESTDIR)$(PREFIX)/lib
LIBS    = -lwiringPi -lwiringPiDev -lpthread -lm

# May not need to  alter anything below this line
###############################################################################

SRC	=	MPL115A1.c

OBJ	=	$(SRC:.c=.o)

all:		MPL115A1

MPL115A1:	$(OBJ)
	@echo [Link]
	@$(CC) -o $@ $(OBJ) $(LDFLAGS) $(LIBS)
	
.c.o:
	@echo [Compile] $<
	@$(CC) -c $(CFLAGS) $< -o $@

.PHONEY:	clean
clean:
	@echo "[Clean]"
	@rm -f $(OBJ) MPL115A1 *~ core tags *.bak

.PHONEY:	tags
tags:	$(SRC)
	@echo [ctags]
	@ctags $(SRC)

.PHONEY:	install
install:
	@echo "[Install]"
	@cp MPL115A1 		$(DESTDIR)$(PREFIX)/bin
	@chown root.root	$(DESTDIR)$(PREFIX)/bin/MPL115A1
	@chmod 4755		$(DESTDIR)$(PREFIX)/bin/MPL115A1

.PHONEY:	uninstall
uninstall:
	@echo "[UnInstall]"
	@rm -f $(DESTDIR)$(PREFIX)/bin/MPL115A1

.PHONEY:	depend
depend:
	makedepend -Y $(SRC)

# DO NOT DELETE

MPL115A1.o: MPL115A1.h
