# name of the program (Minix service)
PROG=proj

# source code files to be compiled
SRCS = main.c piece.c board.c chess.c

# additional compilation flags
CPPFLAGS += -pedantic

# list of library dependencies (for Lab 2, only LCF library)
DPADD += ${LIBLCF}
LDADD += -llcf

# include LCOM's makefile that does all the "heavy lifting"
.include <minix.lcom.mk>
