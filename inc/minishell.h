#ifndef MINISHELL_H 
# define MINISHELL_H

# include <math.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
#include <readline/history.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <unistd.h>
#include "../lib/libft/includes/ft_printf.h"
#include "../lib/libft/includes/libft.h"
#include "../lib/libft/includes/get_next_line.h"

void	ft_change_dir(const char *path);
void	ft_exit(void);
void	ft_pwd(void);
void	ft_echo(char **args, char *str);
void	ft_env(void);
int	ft_export(char *variable);
void    get_value(char *name);
int	ft_unset(char *name);


typedef struct s_env
{
    char **environ;
}              t_env;

#endif
