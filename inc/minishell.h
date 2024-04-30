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
# include "types.h"

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

int	get_last_exit_status();

char		*extract_after_dollar(const char *str, char quots);
char		*extract_substring(const char *str);
extern int	ft_change_dir(char **path, t_context *ctx);

/* ft_echo */

int         ft_echo(char **args, int fd);
//void		ft_echo_print(char *temp, int start_index, char **env);

/* ft_exit */

char		*skip_chars(const char *str);
bool		is_numeric(const char *str);
int			ft_exit(const char *exit_code);

/* ft_export */

int			ft_export(char **variable, char ***env, int fd);
int	update_existing_variable(char **variable, char ***env_ptr, int *env_count, int add_to_value);
int	add_new_variable(char **variable, char ***env_ptr, int env_count, int index);
int	handle_equal_sign(char **env, char *str, int to_equal, int *env_count_local);
int	handle_plus_sign(char **env, char *str, int name_len, int *env_count_local);

/* ft_export_etc    */

int	check_variable_name(char *variable);
int	ft_sort_export_cmd(char **environ_exp, int fd, int i);
void	ft_print_free(char **variable, int fd);
int	is_valid_identifier(const char *variable);

/* ft_unset */

int			ft_unset(char **variable, char ***env);

/* ft_pwd and ft_env */

int		ft_pwd(int fd, t_context *ctx);
int ft_env(char **env, int fd);
char		*get_value(char *name, char **env);

/* ft_get_username and get_hostname */

char	*get_user_host_name(char *str);
void    print_prompt_with_user_details(void);

/* main */

char	**grep_paths(char **env);
int			get_last_exit_status(void);
char	*ft_path(void);
int	select_buildin_commands(char **args, t_list *redir_list, t_context *ctx);

#endif
