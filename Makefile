include ../Makefile.inc

GEN_EXE = 

LINUX_EXE = test \
						tee\
						cp\
						append\
						atomic_append\
						dup\
						5-5\
						readvwritev\
						malloc\
						6-2\
						setunsetenv\
						free_and_sbrk

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}


clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
