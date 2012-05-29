TEXSRC      =   asg3_dd.tex
TEXPDF      =   ${TEXSRC:.tex=.pdf}
LIBFILE     =   libmem.a
CSOURCES    =   memalloc.c buddy.c
CHEADERS    =   buddy.h
COBJECTS    =   ${CSOURCES:.c=.o}
TESTSRCS    =   buddy_test.c
TESTEXES    =   ${TESTSRCS:.c=}
TARFILE     =   proj3.tar

all : ${LIBFILE} ${TEXPDF} ${TESTEXES}

${TESTEXES} : ${TESTSRCS} ${LIBFILE}
	gcc -o $@ $< -L. -lmem

${LIBFILE} : ${COBJECTS}
	ar -cvr $@ ${COBJECTS}
	
%.o : %.c
	gcc -Wall -c $<

${TEXPDF} : ${TEXSRC}
	pdflatex $<
	
view : ${TEXPDF}
	- evince $<

clean :
	- rm ${COBJECTS}
	- rm ${TEXSRC:.tex=.aux}
	- rm ${TEXSRC:.tex=.log}

spotless : clean
	- rm ${LIBFILE}
	- rm ${TEXPDF}
	- rm ${TESTEXES}

again :
	gmake spotless
	gmake all

pack :
	tar cf ${TARFILE} ${TEXPDF} ${CSOURCES} ${CHEADERS} README Makefile
	gzip ${TARFILE} 
	
