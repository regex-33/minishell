#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/includes/ft_printf.h"
# include "../lib/libft/includes/get_next_line.h"
# include "../lib/libft/includes/libft.h"
# include "parser.h"
# include "pipex_bonus.h"
# include "expanding.h"
# include "execution.h"
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
#include <sys/types.h>
#include <sys/utsname.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>
#include <dirent.h>


/* COLORS */

#define COLOR_RESET "\033[0m"
#define COLOR_BOLD_RED "\033[1;31m"
#define COLOR_BOLD_BLUE "\033[1;34m"
#define COLOR_BOLD_WHITE "\033[1;37m"
#define COLOR_KHDER_FATH "\033[1;32m"


# define ANSI_COLOR_BLUE "\x1b[34m"
# define ANSI_COLOR_RESET "\x1b[0m"

#define MAX_USERNAME_LENGTH 256
#define MAX_HOSTNAME_LENGTH 256
#define HOSTNAME_FILE "/proc/sys/kernel/hostname"
#define COMMAND_NOT_FOUND "COMMAND NOT FOUND"

/* change directory */
typedef struct s_context
{
    char    **env;
    int     last_status;
}          t_context;

char		*extract_after_dollar(const char *str, char quots);
char		*extract_substring(const char *str);
void		ft_change_dir(const char *path, char **env);

/* ft_echo */

void		ft_echo(char **args);
//void		ft_echo_print(char *temp, int start_index, char **env);

/* ft_exit */

char		*skip_chars(const char *str);
bool		is_numeric(const char *str);
void		ft_exit(const char *exit_code);

/* ft_export */

int			is_valid_identifier(const char *variable);
int			ft_export(char **variable, char ***env);

/* ft_unset */

int			ft_unset(char *variable, char **env);

/* ft_pwd and ft_env */

void		ft_pwd(void);
void		ft_env(char **env);
char		*get_value(char *name, char **env);

/* ft_get_username and get_hostname */

char	*get_user_host_name(char *str);
void    print_prompt_with_user_details(void);

/* main */

char	**grep_paths(char **env);
int			get_last_exit_status(void);
char	*ft_path(void);
int	select_buildin_commands(char **args, char **env);

#endif
