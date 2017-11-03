#
#	Makefile for Kernel language on 64-bit ABE runtime
#

#CFLAGS=	-ansi -O3 -DNDEBUG -DDBUG_OFF
#CFLAGS=	-ansi -O -DNDEBUG -DDBUG_OFF
#CFLAGS=	-ansi -g
#CFLAGS=	-ansi -pedantic
CFLAGS=	-ansi -pedantic -Wall

LIB=	libabe.a
LHDRS=	actor.h emit.h atom.h gc.h cons.h sbuf.h dbug.h types.h
LOBJS=	actor.o emit.o atom.o gc.o cons.o sbuf.o dbug.o

LIBS=	$(LIB) -lm

PROGS=	abe kernel
JUNK=	*.exe *.stackdump *.dbg core *~

all: $(LIB) $(PROGS)

clean:
	rm -f $(LIB) $(PROGS) $(JUNK) *.o

test: $(PROGS)
	rm -f *.dbg
	./abe -t -#d:t
#	./abe -t -#d:t:o,abe.dbg
#	./kernel -t -#d:t:o,kernel.dbg

$(LIB): $(LOBJS)
	$(AR) rv $(LIB) $(LOBJS)

$(LOBJS): $(LHDRS)

abe: abe.o sample.o $(LIB) Makefile
	$(CC) $(CFLAGS) -o $@ abe.o sample.o $(LIBS)

kernel: kernel.o $(LIB) Makefile
	$(CC) $(CFLAGS) -o $@ kernel.o $(LIBS)

.c.o:
	$(CC) $(CFLAGS) -c $<
