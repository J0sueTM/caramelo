CC:=cc
AR:=ar

TGT_NAME:=caramelo
TGT_DIR:=$(CURDIR)/target

LIB_DIR:=${TGT_DIR}/lib
LIB=${LIB_DIR}/lib${TGT_NAME}.a

LIBS=liblog.c libX11

VENDOR_DIR=$(CURDIR)/vendor
VENDOR_LIBS=$(patsubst %, ${LIB_DIR}/%.a, ${LIBS})
VENDORS=log.c x11
SUBMODULES=$(patsubst %, ${VENDOR_DIR}/%, ${VENDORS})

SRC_DIR:=$(CURDIR)/src
SRCS=$(shell find ${SRC_DIR} -type f -name *.c)

OBJ_DIR:=${TGT_DIR}/obj
OBJS:=$(patsubst ${SRC_DIR}/%.c, ${OBJ_DIR}/%.o, ${SRCS})

EX_SRC_DIR:=$(CURDIR)/examples
EX_SRCS=$(shell find ${EX_SRC_DIR} -type f -name *.c)

EX_TGT_DIR:=${TGT_DIR}/examples
EX_TGTS=$(patsubst ${EX_SRC_DIR}/%.c, ${EX_TGT_DIR}/%, ${EX_SRCS})

DEP_DIRS=${TGT_DIR} ${LIB_DIR} ${OBJ_DIR} ${EX_TGT_DIR}
DEPS=${DEP_DIRS} ${SUBMODULES}

CC_FLAGS=-Wall -L${LIB_DIR} -llog -lX11 -lGL
ifeq ($(DEBUG),true)
	CC_FLAGS+=-g
endif

all: ${DEPS} ${VENDOR_LIBS} ${LIB}

${LIB}: ${OBJS}
	@echo === building ${TGT_NAME} ===
	${AR} rcs $@ $^

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	@echo === compiling $(notdir $@) ===
	@mkdir -p $(dir $@)
	${CC} -c $< -o $@ ${CC_FLAGS}

${LIB_DIR}/liblog.c.a:
	@echo === building vendor log.c ===
	${CC} -c ${VENDOR_DIR}/log.c/src/log.c -o ${OBJ_DIR}/log.c.o
	${AR} rcs $@ ${OBJ_DIR}/log.c.o
	cp $@ ${LIB_DIR}/liblog.a

${LIB_DIR}/libX%.a:
	@cd ${VENDOR_DIR}/x11; \
  X11_LIBS=$$(find ./src -type f -name lib*.a); \
	if [ -z "$${X11_LIBS}" ]; then \
		echo === building vendor x11 ===; \
    ./autogen.sh && $(MAKE); \
	fi; \
	if [ -z "$(shell find ${LIB_DIR} -type f -name lib*.a)" ]; then \
		echo === copying vendor x11 ===; \
		for lib in $${X11_LIBS}; do \
			echo ...$${lib}; \
			cp $${lib} ${LIB_DIR}; \
		done; \
	fi

${SUBMODULES}: 
	@echo === cloning submodules ===
	git submodule update --init --recursive

examples: ${DEP_DIRS} ${EX_TGTS}

${EX_TGT_DIR}/%: ${EX_SRC_DIR}/%.c
	@echo === building example $(notdir $@) ===
	${CC} -o $@ $< -lcaramelo ${CC_FLAGS}

${DEP_DIRS}:
	@echo === creating dirs ===
	mkdir -p ${DEP_DIRS}

clean:
	@echo === cleaning ===
	rm -rf ${DEP_DIRS}

.PHONY: clean
