EXECUTABLES = helloWorld_Consumer helloWorld_ConsumerForever helloWorld_Producer

all: ${EXECUTABLES}


CCNX_HOME=/usr/local/ccnx
PARC_HOME=/usr/local/parc

INCLUDE_DIR_FLAGS=-I. -I${CCNX_HOME}/include -I${PARC_HOME}/include
LINK_DIR_FLAGS=-L${CCNX_HOME}/lib -L${PARC_HOME}/lib
CCNX_LIB_FLAGS=-lccnx_api_portal -lccnx_api_notify -lrta -lccnx_api_control -lccnx_common
PARC_LIB_FLAGS=-lparc -llongbow -llongbow-ansiterm
DEP_LIB_FLAGS=-lcrypto -lm -levent

CFLAGS=${INCLUDE_DIR_FLAGS} ${LINK_DIR_FLAGS} ${CCNX_LIB_FLAGS} ${PARC_LIB_FLAGS} ${DEP_LIB_FLAGS} 

CC=gcc -O2 -std=c99

helloWorld_Consumer: helloWorld_Consumer.c config.h
	${CC} helloWorld_Consumer.c ${CFLAGS} -o helloWorld_Consumer 

helloWorld_ConsumerForever: helloWorld_ConsumerForever.c config.h
	${CC} helloWorld_ConsumerForever.c ${CFLAGS} -o helloWorld_ConsumerForever 

helloWorld_Producer: helloWorld_Producer.c config.h
	${CC} helloWorld_Producer.c ${CFLAGS} -o helloWorld_Producer 

clean:
	rm -rf ${EXECUTABLES}
