
MPICC  = mpicc
CFLAGS = -O

all:	greetings

clean:
	rm -f *.[oadD] *.[mM][oO][dD] greetings

greetings:	greetings.c
	${MPICC} ${CFLAGS} -o greetings greetings.c

