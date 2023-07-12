# Components
LOG						:= ./components/log
LOG_INC_DIR				:= ${LOG}/includes
LOG_LIB_DIR				:= ${LOG}
LOG_INCLUDES			:= -I${LOG_INC_DIR}
LOG_LIB					:= -L${LOG_LIB_DIR} -llog

# Comum
SRC_DIR					:= ./src
OBJ_DIR					:= ./obj
LOG_DIR					:= ./logs
INC_DIR					:= ./includes
TEST_DIR				:= ./testObj

CC						:= c++
INCLUDES				:= -I${INC_DIR} ${LOG_INCLUDES}
LIBS					:= ${LOG_LIB}

NAME					:= webserv
TEST_NAME				:= test_webserv

SRC						:= $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)

OBJ						:= $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
CFLAGS					:= -Wall -Werror -Wextra -std=c++98 ${INCLUDES}
LDFLAGS					:= -Wall -Werror -Wextra -std=c++98 -g -fsanitize=address ${INCLUDES} ${LIBS}

TEST_OBJ				:= $(patsubst $(SRC_DIR)/%.cpp,${TEST_DIR}/$(OBJ_DIR)/%.o,$(SRC))
TEST_CFLAGS				:= -pthread ${INCLUDES}
TEST_LDFLAGS			:= -lgtest -lgtest_main -pthread -g -fsanitize=address ${INCLUDES} ${LIBS}

all: make_components ${NAME}

${NAME}: ${OBJ}
	@mkdir -p ${LOG_DIR}
	${CC} -o ${NAME} $^ ${LDFLAGS}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@mkdir -p $(dir $@)
	${CC} -c ${CFLAGS} $< -o $@

test: make_components ${TEST_NAME}

${TEST_NAME}: ${TEST_OBJ}
	@mkdir -p ${LOG_DIR}
	${CC} -o ${TEST_NAME} $^ ${TEST_LDFLAGS}

${TEST_DIR}/${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@mkdir -p $(dir $@)
	${CC} -c ${TEST_CFLAGS} -DTEST_MODE=1 $< -o $@

run: fclean all
	rm -rf ${LOG_DIR}/*.txt
	./${NAME}

test_run: fclean test
	rm -rf ${LOG_DIR}/*.txt
	./${TEST_NAME}

clean:
	rm -rf ${OBJ_DIR}
	rm -rf ${TEST_DIR}

fclean: clean
	rm -rf ${NAME}
	rm -rf ${TEST_NAME}
	rm -rf ${LOG_DIR}
	make fclean --directory="./components/log"

re: fclean all

make_components:
	make --directory="./components/log"

install:
	rm -rf googletest;
	sudo apt-get update -y;
	sudo apt-get upgrade -y;
	sudo apt-get update -y;
	sudo apt-get install cppcheck -y;
	git clone https://github.com/google/googletest.git;
	cd googletest && mkdir -p build && cd build && cmake .. && sudo make && sudo make install
	rm -rf googletest;

cppcheck:
	cppcheck ${SRC_DIR} -i./src/test --suppress=missingInclude --enable=all -I./includes > result_cppcheck.txt 2>&1

.PHONY: all run clean fclean re make_components install cppcheck
