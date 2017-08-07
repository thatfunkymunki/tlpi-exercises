include ../Makefile.inc

GEN_EXE = 

LINUX_EXE = test \
						tee\
						cp\
						append\
						atomic_append\
						dup

EXE = ${GEN_EXE} ${LINUX_EXE}

all : ${EXE}

allgen : ${GEN_EXE}


clean : 
	${RM} ${EXE} *.o

showall :
	@ echo ${EXE}

${EXE} : ${TLPI_LIB}		# True as a rough approximation
