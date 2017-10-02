include ../Makefile.inc

EXE = $(patsubst %.c,%,$(wildcard *.c))

all : ${EXE}

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
