# Comum
SRC_DIR					:=	./src
OBJ_DIR					:=	./obj
LOG_DIR					:=	./logs
INC_DIR					:=	./includes
TEST_DIR				:=	./tst

CC							:=	c++

NAME						:=	webserv
TEST_NAME				:=	test_webserv

MAIN_SRC				:= ${SRC_DIR}/main.cpp
TEST_SRC				:= ${SRC_DIR}/test/catch.cpp

SRC							:= $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)
SRC							:= $(filter-out $(TEST_SRC), $(SRC))
OBJ							:=	$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
CFLAGS					:=	-Wall -Werror -Wextra -std=c++98 -I${INC_DIR}
LDFLAGS					:=	-Wall -Werror -Wextra -std=c++98 -g -fsanitize=address -I${INC_DIR}

TEST_SRC				:= $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)
TEST_SRC				:= $(filter-out $(MAIN_SRC), $(TEST_SRC))
TEST_OBJ				:=	$(patsubst $(SRC_DIR)/%.cpp,${TEST_DIR}/$(OBJ_DIR)/%.o,$(TEST_SRC))
TEST_CFLAGS			:=	-I${INC_DIR}
TEST_LDFLAGS		:=	-g -fsanitize=address -I${INC_DIR}

all: ${NAME}

${NAME}: ${OBJ}
	@mkdir -p ${LOG_DIR}
	${CC} -o ${NAME} $^ ${LDFLAGS}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@mkdir -p $(dir $@)
	${CC} -c ${CFLAGS} $< -o $@

test: ${TEST_NAME}

${TEST_NAME}: ${TEST_OBJ}
	@mkdir -p ${LOG_DIR}
	${CC} -o ${TEST_NAME} $^ ${TEST_LDFLAGS}

${TEST_DIR}/${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@mkdir -p $(dir $@)
	${CC} -c ${TEST_CFLAGS} $< -o $@

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

re: fclean all

install:
	sudo apt-get install cppcheck

cppcheck:
	cppcheck . --suppress=missingInclude --enable=all -I./includes > result_cppcheck.txt 2>&1

.PHONY: all run clean fclean re install cppcheck
