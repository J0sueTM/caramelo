CC:=cc
AR:=ar

DEBUG:=0

TGT_NAME:=caramelo
TGT_DIR:=target

LIB_DIR:=${TGT_DIR}/lib
LIB=${TGT_DIR}/${TGT_NAME}

SRC_DIR:=src
SRCS=$(shell find ${SRC_DIR} -type f -name *.c)

OBJ_DIR:=${TGT_DIR}/obj
OBJS:=$(patsubst ${SRC_DIR}/%.c, ${OBJ_DIR}/%.o, ${SRCS})

VENDOR_DIR=vendor
VENDORS=log.c

DEP_DIRS=${TGT_DIR} ${LIB_DIR} ${OBJ_DIR}
DEPS=${DEP_DIRS} submodules

all: ${DEPS} ${VENDORS} ${LIB}

${LIB}: ${OBJS}
	${AR} rcs $@ $^

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	${CC} -c $< -o $@

log.c:
	${CC} -c ${VENDOR_DIR}/log.c/src/**.c -o ${OBJ_DIR}/log.c.o
	${AR} rcs ${LIB_DIR}/liblog.c.a ${OBJ_DIR}/log.c.o

submodules:
	git submodule update --init --recursive	

${DEP_DIRS}:
	mkdir -p ${DEP_DIRS}
