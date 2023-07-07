# Comum
SRC_DIR					:=	./src
OBJ_DIR					:=	./obj
LOG_DIR					:=	./logs
INC_DIR					:=	./includes

CC							:=	c++

NAME						:=	webserv

CFLAGS					:=	-Wall -Werror -Wextra -std=c++98 -I${INC_DIR}
LDFLAGS					:=	-Wall -Werror -Wextra -std=c++98 -g -fsanitize=address -I${INC_DIR}

SRC							:=	$(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)
OBJ							:=	$(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))

all: ${NAME}

${NAME}: ${OBJ}
	@mkdir -p ${LOG_DIR}
	${CC} -o ${NAME} $^ ${LDFLAGS}

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@mkdir -p $(dir $@)
	${CC} -c ${CFLAGS} $< -o $@

run: fclean all
	rm -rf ${LOG_DIR}/*.txt
	./${NAME}

clean:
	rm -rf ${OBJ_DIR}

fclean: clean
	rm -rf ${NAME}

re: fclean all

cppcheck:
	cppcheck . --suppress=missingInclude --enable=all -I./includes > result_cppcheck.txt 2>&1

.PHONY: all run clean fclean re cppcheck
