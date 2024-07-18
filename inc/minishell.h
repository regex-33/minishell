#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/includes/ft_printf.h"
# include "../lib/libft/includes/get_next_line.h"
# include "../lib/libft/includes/libft.h"
# include "execution.h"
# include "expanding.h"
# include "parser.h"
# include "pipex_bonus.h"
# include "types.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <math.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/utsname.h>
# include <sys/wait.h>
# include <term.h>
# include <termios.h>
# include <time.h>
# include <unistd.h>

# define ANSI_COLOR_RED "\x1b[31m"
# define ANSI_COLOR_GREEN "\x1b[32m"
# define ANSI_COLOR_YELLOW "\x1b[33m"
# define ANSI_COLOR_BLUE "\x1b[34m"
# define ANSI_COLOR_MAGENTA "\x1b[35m"
# define ANSI_COLOR_CYAN "\x1b[36m"
# define ANSI_COLOR_RESET "\x1b[0m"

# define SUFF_LEN 32
# define BASE_HERE_FILENAME "/tmp/minsh-"
# define HERE_PROMPT "heredoc> "
# define HERE_MAX_RETRY 5

# define CUSTOM_COLOR_BOLD_BLUE 1000
# define CUSTOM_COLOR_BOLD_CYAN 1001

// Use ANSI escape sequences for custom colors
# define CUSTOM_COLOR_BOLD_BLUE_ANSI "\x1b[1;94m"
# define CUSTOM_COLOR_BOLD_CYAN_ANSI "\x1b[1;96m"

/* COLORS */

# define COLOR_RESET "\033[0m"
# define COLOR_BOLD_RED "\033[1;31m"
# define COLOR_BOLD_BLUE "\033[1;34m"
# define COLOR_BOLD_WHITE "\033[1;37m"
# define COLOR_KHDER_FATH "\033[1;32m"

# define ANSI_COLOR_RESET "\x1b[0m"
# define ANSI_COLOR_BOLD_RED "\x1b[1;31m"
# define ANSI_COLOR_BOLD_BLUE "\x1b[1;34m"
# define ANSI_COLOR_BOLD_WHITE "\x1b[1;37m"
# define ANSI_COLOR_KHDER_FATH "\x1b[1;32m"

# define ANSI_COLOR_BLUE "\x1b[34m"
# define ANSI_COLOR_RESET "\x1b[0m"

# define MAX_USERNAME_LENGTH 256
# define MAX_HOSTNAME_LENGTH 256
# define HOSTNAME_FILE "/proc/sys/kernel/hostname"
# define COMMAND_NOT_FOUND "COMMAND NOT FOUND"
# define PWD_ERROR "minishell: pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n"

/*  temp env    */

# define FIRST_ENV "PWD=/Users/yachtata/mini"
# define SECOND_ENV "SHLVL=1"
# define THIRD_ENV "_=/usr/bin/env"
# define FOURTH_ENV "OLDPWD"

# define SET_STATE 1
# define ON_PROMPT 0
# define ON_EXEC 1

# define TEMP_PATH "PATH=/usr/gnu/bin:/usr/local/bin:/bin:/usr/bin:."

/*  status  */

# define SET_STATUS 1

/*      buildin utils       */

int		get_status(int new_status, int flags);
int		get_state(int new_state, int flags);
int		is_builtin(char *cmd_name);
char	**grep_paths(t_context *ctx);
int		parse_existing_variable(char *str, int flag);
int		init_context(t_context *ctx);

/*      environment         */

int		handle_shell_level(char *str, char **env, int *i);
char	**creat_temp_env(t_context *ctx);
char	**ft_creat_env(t_context *ctx);

/*      ft_cd_etc           */

int		is_directory(const char *path);
int		check_permission(const char *path);
int		check_existence(const char *path);
void	init_array(char **array, int size);

/* change directory */

int		home_path(t_context *ctx);
int		ft_change_dir(char **path, t_context *ctx);
int		excute_success(t_context *ctx);
int		excute_failed(char **args);

/* ft_echo */

int		ft_echo(char **args, int fd);

/* ft_exit */

bool	is_numeric(const char *str);
int		ft_exit(char **exit_code, t_context *ctx);

/* ft_export etc */

int		is_valid_identifier(const char *variable);
void	ft_print(char **variable, int fd, int unset_path);
int		ft_sort_export_cmd(char **environ_exp, int fd, int unset_path);
int		check_variable_name(char *variable, int *unset_path);

/*   ft_export    */

int		ft_export(char **variable, char ***env, int fd, int *unset_path);
int		update_existing_variable(char **variable, char ***env_ptr,
			int *env_count, int add_to_value);
int		add_new_variable(char **variable, char ***env_ptr, int env_count,
			int index);
int		handle_equal(char **env, char *str, int to_equal, int *env_count_local);
int		handle_plus(char **env, char *str, int name_len, int *env_count_local);

/* ft_unset */

int		get_env_count(char **env);
int		ft_unset(char **variable, char ***env, int *unset_path);
char	**allocate_new_environ(int env_count);
int		copy_variable_to_new_environment(char *env_var, char **new_environ,
			int *j);
int		copy_env_except_variable(char **env, char **new_environ, char *variable,
			int *j);

/* ft_pwd and ft_env */

int		ft_pwd(int fd, t_context *ctx);
int		ft_env(t_context *ctx, int fd);
char	*get_value(char *name, char **env);
int		ft_handle_no_variables(char ***env_ptr, int fd, int unset_path);

/* ft_get_username and get_hostname */

char	*get_user_host_name(char *str);

char	*get_prompt_with_user_details(int is_prompt);
/* main */

int		get_status(int new_status, int flags);
char	*ft_path(void);
int		select_buildin_commands(char **args, t_list *redir_list,
			t_context *ctx);

/*      signals     */

void	handle_interrupt(int sig);
void	handle_quit(int sig);

/*  */

int		isquote(char c);

#endif
