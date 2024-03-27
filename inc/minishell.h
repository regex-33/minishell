#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/includes/ft_printf.h"
# include "../lib/libft/includes/get_next_line.h"
# include "../lib/libft/includes/libft.h"
# include "pipex_bonus.h"
# include <math.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stddef.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <time.h>
# include <unistd.h>

# define ANSI_COLOR_BLUE "\x1b[34m"
# define ANSI_COLOR_RESET "\x1b[0m"

/* change directory */

char		*extract_after_dollar(const char *str, char quots);
char		*extract_substring(const char *str);
void		ft_change_dir(const char *path);

/* ft_echo */

void		ft_echo(char **args, char *str);
void		ft_echo_print(char *temp, int start_index);

/* ft_exit */

char		*skip_chars(const char *str);
bool		is_numeric(const char *str);
void		ft_exit(const char *exit_code);

/* ft_export */

int			is_valid_identifier(const char *variable);
int			ft_export(char *variable);

/* ft_unset */

int			ft_unset(char *variable);

/* ft_pwd and ft_env */

void		ft_pwd(void);
void		ft_env(void);
char		*get_value(char *name);

/* main */

int			get_last_exit_status(void);


#endif
