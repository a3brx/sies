# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = main.c interrupts.c timer.c kbc.c keyboard.c mouse.c graphics.c draw.c arrow.c chess.c game.c utils.c rtc.c menu.c serial.c

# additional compilation flags
# "-Wall -Wextra -Werror -I . -std=c11 -Wno-unused-parameter" are already set
CFLAGS += -pedantic -DPROJ

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
