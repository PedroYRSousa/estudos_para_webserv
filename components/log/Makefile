NAME					:= liblog.a

CC						:= c++

SRC_DIR					:= ./src
OBJ_DIR					:= ./obj
INC_DIR					:= ./includes

SRC						:= $(wildcard $(SRC_DIR)/**/*.cpp $(SRC_DIR)/*.cpp)
OBJ						:= $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC))
CFLAGS					:= -Wall -Werror -Wextra -std=c++98 -g -fsanitize=address -I${INC_DIR}

all: ${NAME}

${NAME}: ${OBJ}
	ar rcs ${NAME} $^

${OBJ_DIR}/%.o: ${SRC_DIR}/%.cpp
	@mkdir -p $(dir $@)
	${CC} -c $< -o $@ ${CFLAGS}

clean:
	rm -rf ${OBJ_DIR}

fclean: clean
	rm -rf ${NAME}

re: fclean all

cppcheck:
	cppcheck ${SRC_DIR} --suppress=missingInclude --enable=all -I./includes > result_cppcheck.txt 2>&1

.PHONY: all clean fclean re
