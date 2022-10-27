SRCDIR=../src
OBJDIR=../obj
BINDIR=../bin
INCDIR=../include
CFLAGS=gcc -c -I${INCDIR}
all:bank.o finalexe
bank.o:
	${CFLAGS} ${SRCDIR}/bank.c -o ${OBJDIR}/bank.o
finalexe:
	gcc ${OBJDIR}/bank.o -o ${BINDIR}/main.exe


clean:
	rm ${OBJDIR}bank.o
