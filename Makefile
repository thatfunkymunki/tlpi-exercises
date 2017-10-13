include ../Makefile.inc


LIB = $(patsubst lib%.c, lib%,$(wildcard lib*.c))

EXE = $(filter-out $(LIB) ,$(patsubst %.c,%,$(wildcard *.c)))


all : ${EXE} 

clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}
	@ echo ${TLPI_LIB}
	@ echo ${LIB}

${EXE} :  ${TLPI_LIB}  		# True as a rough approximation
