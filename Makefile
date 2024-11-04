
TGT_NAME:=fossdaw
TGT_DIR:=$(CURDIR)/target
TGT:=${TGT_DIR}/${TGT_NAME}

LIB_DIR:=${TGT_DIR}/lib
LIBS=liblog.c libX11

VENDOR_DIR=$(CURDIR)/vendor
VENDOR_LIBS=$(patsubst %, ${LIB_DIR}/%.a, ${LIBS})
VENDORS=log.c x11
SUBMODULES=$(patsubst %, ${VENDOR_DIR}/%, ${VENDORS})

SRC_DIR:=$(CURDIR)/src
SRCS=$(shell find ${SRC_DIR} -type f -name *.c)

OBJ_DIR:=${TGT_DIR}/obj
OBJS:=$(patsubst ${SRC_DIR}/%.c, ${OBJ_DIR}/%.o, ${SRCS})

TEST_SRC_DIR:=$(CURDIR)/tests
TEST_SRCS=$(shell find ${TEST_SRC_DIR} -type f -name *.c ! -name all.c)

TEST_TGT_DIR:=${TGT_DIR}/tests
TEST_TGTS=$(patsubst ${TEST_SRC_DIR}/%.c, ${TEST_TGT_DIR}/%, ${TEST_SRCS})

DEP_DIRS=${TGT_DIR} ${LIB_DIR} ${OBJ_DIR} ${TEST_TGT_DIR}
DEPS=${DEP_DIRS} ${SUBMODULES}

CC_FLAGS=-Wall -L${LIB_DIR} -llog -lX11 -lGL
ifeq ($(DEBUG),true)
	CC_FLAGS+=-g
else
	CC_FLAGS+=-O3
endif

all: ${DEPS} ${VENDOR_LIBS} ${TGT}

run: all
	${TGT}

${TGT}: ${OBJS}
	@echo === building $(notdir $(basename $@)) ===
	$(CC) $^ -o $@ ${CC_FLAGS}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.c
	@echo === compiling $(notdir $(basename $@)) ===
	@mkdir -p $(dir $@)
	$(CC) -c $^ -o $@ ${CC_FLAGS}

${LIB_DIR}/liblog.c.a:
	@echo === building vendor log.c ===
	$(CC) -c ${VENDOR_DIR}/log.c/src/log.c -o ${OBJ_DIR}/log.c.o
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

${EX_TGT_DIR}/%: ${EX_SRC_DIR}/%.c ${LIB}
	@echo === building example $(notdir $@) ===
	$(CC) -o $@ $< -lfossdaw ${CC_FLAGS}

tests: ${DEP_DIRS} ${TEST_TGTS}

${TEST_TGT_DIR}/%: ${TEST_SRC_DIR}/%.c ${LIB}
	@echo === building test $(notdir $@) ===
	mkdir -p $(dir $@)
	$(CC) -o $@ $< ${VENDOR_DIR}/munit/munit.c -lfossdaw ${CC_FLAGS}

GLOBAL_TEST=${TEST_TGT_DIR}/all
all-tests: ${DEP_DIRS} ${TEST_TGTS} ${GLOBAL_TEST}
	./target/tests/all --log-visible debug

${GLOBAL_TEST}: ${TEST_SRC_DIR}/all.c ${TEST_SRC_DIRS}
	@echo === building global test ===
	$(CC) -o $@ $^ ${VENDOR_DIR}/munit/munit.c -lfossdaw ${CC_FLAGS}

${DEP_DIRS}:
	@echo === creating dirs ===
	mkdir -p ${DEP_DIRS}

clean:
	@echo === cleaning ===
	rm -rf ${DEP_DIRS}

.PHONY: clean
