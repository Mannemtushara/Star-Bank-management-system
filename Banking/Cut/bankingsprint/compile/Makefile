SRCDIR=../src
OBJDIR=../obj
BINDIR=../bin
INCDIR=../include
CFLAGS=gcc -c -I${INCDIR}
all:banking.o finalexe
banking.o:
	${CFLAGS} ${SRCDIR}/banking.c -o ${OBJDIR}/banking.o
finalexe:
	gcc ${OBJDIR}/banking.o -o ${BINDIR}/main.exe


clean:
	rm ${OBJDIR}banking.o
