
SHELL	= /bin/sh
PACKAGE = ulsnake
VERSION = 0.1
DATE	= May2013

all:
	@echo "\nCompiling..."
	@gcc ulsnake.c levels.c -Wall -o ulsnake -lncurses

install:	all
	@cp ulsnake /usr/local/bin
	@chmod 755 /usr/local/bin/ulsnake
	@echo "Installed. Type 'ulsnake' to start the game. \n"

uninstall:
	@rm -f /usr/local/bin/ulsnake

clean:	uninstall
	@rm -f ulsnake
	@echo "\nDone!\n"
