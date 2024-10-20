CC:=cc
AR:=ar

TGT_NAME:=caramelo
TGT_DIR:=target

LIB_DIR:=${TGT_DIR}/lib
LIB=${TGT_DIR}/${LIB_NAME}

OBJ_DIR:=${TGT_DIR}/obj

VENDOR_DIR=vendor
VENDORS=log.c

DEP_DIRS=${TGT_DIR} ${LIB_DIR} ${OBJ_DIR}
DEPS=${DEP_DIRS} submodules

all: ${DEPS} ${VENDORS} ${LIB}

${LIB}:
	tree target

log.c:
	${CC} -c ${VENDOR_DIR}/log.c/src/**.c -o ${LIB_DIR}/log.c.o
	${AR}	rcs ${OBJ_DIR}/liblog.c.a ${LIB_DIR}/log.c.o

submodules:
	git submodule update --init --recursive	

${DEP_DIRS}:
	mkdir -p ${DEP_DIRS}
