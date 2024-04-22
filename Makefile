# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/04/21 12:11:54 by bchanaa           #+#    #+#              #
#    Updated: 2024/04/21 12:31:27 by bchanaa          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


# COLORS
NOCOL=\033[0m
#RED=\033[1;31m
YEL=\033[1;33m
GRN=\033[1;32m
BLU=\033[1;34m


LYELLOW = \033[38;5;222m
BEIGE = \033[38;5;223m
LGREEN = \033[38;5;155m

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

CFLAGS = -g -Wall -Wextra -Werror -D BUFFER_SIZE=10
CFLAGS += -I ./$(LFT_DIR)/inc -Iinc

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
BUILTIN_SOURCES = main.c get_username_hostname.c ft_change_dir.c ft_unset.c ft_pwd_and_env.c ft_exit.c ft_export.c ft_echo.c
PARSING_SOURCES = util_funcs.c lexer.c real_parser.c binary_tree.c #exec.c
EXECUTION_SOURCES = exec.c parse_command.c
# PIPX_SOURCES = util_funcs.c lexer.c real_parser.c binary_tree.c exec.c


#SRC_FILES = main_bonus.c io_utils_bonus.c command_utils_bonus.c	

#BUILTIN_SRC = $(addprefix $(SRC_PATH)/builtin/, $(BUILTIN_SOURCES))
#PARSING_SRC = $(addprefix $(SRC_PATH)/parsing/, $(PARSING_SOURCES))
#PIPX_SRC = $(addprefix $(SRC_PATH)/pipx/, $(PIPX_SOURCES))

ALL_SOURCES = $(BUILTIN_SOURCES) $(PARSING_SOURCES) $(EXECUTION_SOURCES)
vpath %.c src/builtin/ src/parsing/ src/pipx src/excution
vpath %.h inc
# OBJ_FILES = $(ALL_SOURCES:%.c=%.o)

OBJ_BUILTIN_FILES = $(BUILTIN_SOURCES:%.c=%.o)
OBJ_PARSING_FILES = $(PARSING_SOURCES:%.c=%.o)
OBJ_EXECUTION_FILES = $(EXECUTION_SOURCES:%.c=%.o)

# OBJ_PIPX_FILES = $(PIPX_SOURCES:%.c=%.o)

OBJ_FILES = $(addprefix $(OBJ_PATH)/, $(OBJ_BUILTIN_FILES) $(OBJ_PARSING_FILES) $(OBJ_EXECUTION_FILES))

all: $(NAME)
$(NAME): $(LFT_NAME) $(OBJ_FILES)
	@echo "\n"
# @echo "i am here\n"
	@$(CC) $^ -o $@ $(CFLAGS) -lreadline $(LDFLAGS)  $(LDLIBS) -fsanitize=address
# @echo "i am here\n"
	@echo "\n\n\n   ${BOLD}${CUR}${LYELLOW}MINISHELL COMPILED ✨${DEF}\n"

parser: $(LFT_NAME)
	cc -fsanitize=address -g3 $(PARSING_SRC) -Iinc -Llib/libft -lft

# #is used to redirect both standard output (stdout) and standard error (stderr) to /dev/null
$(LFT_NAME):
	@$(MAKE) all -sC $(LFT_DIR)	> /dev/null 2>&1
	@cp $(LFT) $(LFT_NAME)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH) > /dev/null 2>&1

$(OBJ_PATH)/%.o: %.c | $(OBJ_PATH)
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
