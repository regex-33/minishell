
# COLORS
NOCOL=\033[0m
RED=\033[1;31m
YEL=\033[1;33m
ORG=\033[0;33m
GRN=\033[1;32m
DGRAY=\033[1;30m
BLU=\033[1;34m

ORANGE = \033[38;5;216m
LBLUE = \033[38;5;153m
LYELLOW = \033[38;5;222m
BEIGE = \033[38;5;223m
LGREEN = \033[38;5;155m

DEF = \033[0m
BOLD = \033[1m
CUR = \033[3m
UL = \033[4m
UP = \033[A

# NAMES
NAME = minishell

# MAKE
MAKE = make

# COMPILER
CC = gcc

LFT_NAME = libft.a

# PATHS
INC_PATH = inc
SRC_PATH = src
OBJ_PATH = obj
LIB_PATH = lib

LFT_DIR = $(LIB_PATH)/libft

# LIBS
LFT = $(LFT_DIR)/$(LFT_NAME)

CFLAGS = -g -Wall -Wextra -Werror -lreadline -D BUFFER_SIZE=10 
CFLAGS += -I ./$(LFT_DIR)/inc

LDFLAGS = -L ./
LDLIBS = -lft  -O3

# ANIMATION
SRCS_COUNT = 0
SRCS_TOT = ${shell find ./src/ -type f -name '*.c' | wc -l}
SRCS_PRCT = ${shell expr 100 \* ${SRCS_COUNT} / ${SRCS_TOT}}
#determine the length of the progress bar.
BAR =  ${shell expr 23 \* ${SRCS_COUNT} / ${SRCS_TOT}}


ifdef DEBUG
	CFLAGS += -fsanitize=address -g3
endif

# SOURCES
SRC_FILES =	main.c ft_change_dir.c ft_unset.c ft_pwd_and_env.c ft_exit.c ft_export.c ft_echo.c
#SRC_FILES = main_bonus.c io_utils_bonus.c command_utils_bonus.c	

SRC = $(addprefix $(SRC_PATH)/, $(SRC_FILES))

OBJ_FILES = $(SRC_FILES:%.c=%.o)

OBJ = $(addprefix $(OBJ_PATH)/, $(OBJ_FILES))


all: $(NAME)
$(NAME): $(LFT_NAME) $(LMLX_NAME) $(OBJ)
	@echo "\n"
	@$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS) $(LDLIBS)
	@echo "\n\n\n   ${BOLD}${CUR}${LYELLOW}MINISHELL COMPILED ✨${DEF}\n"

# #is used to redirect both standard output (stdout) and standard error (stderr) to /dev/null
$(LFT_NAME):
	@$(MAKE) all -sC $(LFT_DIR)	> /dev/null 2>&1
	@cp $(LFT) $(LFT_NAME)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH) > /dev/null 2>&1

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
#The eval command allows you to perform dynamic evaluation and assignment within the Makefile.
	@${eval SRCS_COUNT = ${shell expr ${SRCS_COUNT} + 1}}
	@$(CC) $(CFLAGS) -c $< -o $@
	@echo ""
	@echo " ${BOLD}${CUR}${BEIGE}-> Compiling ${DEF}${BOLD}${LYELLOW}[MINISHELL]${DEF}"
	@printf " ${BEIGE}   [${LGREEN}%-23.${BAR}s${BEIGE}] [%d/%d (%d%%)]${DEF}" "***********************" ${SRCS_COUNT} ${SRCS_TOT} ${SRCS_PRCT}
#@echo "${UP}${UP}${UP}": Uses ANSI escape sequences (${UP}) to move the cursor up three lines
	@echo "${UP}${UP}${UP}"

debug:
	@echo "\n   ${BOLD}${UL}${MAGENTA}DEBUGGING MODE${DEF}"
	@${MAKE} DEBUG=1

clean:
	@$(MAKE) clean -sC $(LFT_DIR) > /dev/null 2>&1
	@rm -rf $(LFT_NAME)
	@rm -rf $(OBJ_PATH)

fclean: clean
	@$(MAKE) fclean -sC $(LFT_DIR) > /dev/null 2>&1
	@rm -rf $(NAME)

re: fclean all

norm:
	@printf "\n${GRN}##########${YEL} NORMINETTE ${GRN}##########${NOCOL}\n"
	@printf "\n${GRN}LIBFT:${BLU}\n\n"
	@norminette $(LFT_DIR)/srcs
	@printf "\n${GRN}MINISHELL:${BLU}\n\n"
	@norminette $(INC_PATH)
	@norminette $(SRC_PATH)
	@printf "${NOCOL}"

.PHONY: all clean fclean re norm