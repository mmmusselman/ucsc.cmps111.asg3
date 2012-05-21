TEXSRC      =   asg3_dd.tex
TEXPDF      =   ${TEXSRC:.tex=.pdf}
LIBFILE     =   libmem.a
CSOURCE     =   memalloc.c
COBJECT     =   ${CSOURCE:.c=.o}

all : ${LIBFILE} ${TEXPDF}

${LIBFILE} : ${COBJECT}
	ar -cvq $@ $<
	
%.o : %.c
	gcc -Wall -c $<

${TEXPDF} : ${TEXSRC}
	pdflatex $<
	
view : ${TEXPDF}
	- evince $<

clean :
	- rm ${COBJECT}
	- rm ${TEXSRC:.tex=.aux}
	- rm ${TEXSRC:.tex=.log}

spotless : clean
	- rm ${LIBFILE}
	- rm ${TEXPDF}

again :
	gmake spotless
	gmake all

