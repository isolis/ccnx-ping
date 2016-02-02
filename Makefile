EXECUTABLES = ccnx-ping ccnx-ping-server

all: ${EXECUTABLES}

CCNX_HOME ?= /usr/local/ccnx
LIBEVENT_HOME ?= /usr

INCLUDE_DIR_FLAGS=-I. -I${CCNX_HOME}/include 
LINK_DIR_FLAGS=-L${CCNX_HOME}/lib 
CCNX_LIB_FLAGS=-lccnx_api_portal -lccnx_api_notify -lccnx_transport_rta -lccnx_api_control -lccnx_common
PARC_LIB_FLAGS=-lparc -llongbow -llongbow-ansiterm
DEP_LIB_FLAGS=-lcrypto -lm -L${LIBEVENT_HOME}/lib -levent

CFLAGS=${INCLUDE_DIR_FLAGS} ${LINK_DIR_FLAGS} ${CCNX_LIB_FLAGS} ${PARC_LIB_FLAGS} ${DEP_LIB_FLAGS} 

CC=gcc -O2 -std=c99

ccnx-ping: ccnx-ping.c config.h
	${CC} ccnx-ping.c ${CFLAGS} -o ccnx-ping 

ccnx-ping-server: ccnx-ping-server.c config.h
	${CC} ccnx-ping-server.c ${CFLAGS} -o ccnx-ping-server 

clean:
	rm -rf ${EXECUTABLES}
