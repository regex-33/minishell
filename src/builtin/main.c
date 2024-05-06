#include "../../inc/minishell.h"

int	get_status(int new_status, int flags)
{
	static int	status;

	if (flags == SET_STATUS)
		status = new_status;
	return (status);
}

// Set: get_status(your_status, SET_STATUS);
// Get: get_status(0, 0);

int	is_builtin(char *cmd_name)
{
	if (!ft_strcmp(cmd_name, "cd"))
		return (1);
	else if (!ft_strcmp(cmd_name, "pwd"))
		return (1);
	else if (!ft_strcmp(cmd_name, "exit"))
		return (1);
	else if (!ft_strcmp(cmd_name, "echo"))
		return (1);
	else if (!ft_strcmp(cmd_name, "env"))
		return (1);
	else if (!ft_strcmp(cmd_name, "export"))
		return (1);
	else if (!ft_strcmp(cmd_name, "unset"))
		return (1);
	return (0);
}

int	select_buildin_commands(char **args, t_list *redir_list, t_context *ctx)
{
	int	status;

	status = 1;
	if (!*args || !is_builtin(args[0]))
		return (-1);
	if (redirect(redir_list, ctx))
		return (1);
	if (!ft_strcmp(args[0], "cd"))
		status = ft_change_dir(++args, ctx); // return status code
	else if (!ft_strcmp(args[0], "pwd"))
		status = ft_pwd(1, ctx);
	else if (!ft_strcmp(args[0], "exit"))
		status = ft_exit(args);
	else if (!ft_strcmp(args[0], "echo"))
		status = ft_echo(args, 1);
	else if (!ft_strcmp(args[0], "env"))
		status = ft_env(ctx, 1);
	else if (!ft_strcmp(args[0], "export"))
		status = ft_export(args, &ctx->env, 1, &ctx->unset_path);
	else if (!ft_strcmp(args[0], "unset"))
		status = ft_unset(args, &ctx->env, &ctx->unset_path);
	reset_redir(redir_list, 1);
	return (status);
}


char	*get_prompt(char *str, char *suffix)
{
	char	*prompt;
	size_t	size;

	if (!str)
		return (NULL);
	size = ft_strlen(str) + 1;
//	size += ft_strlen(COLOR_KHDER_FATH);
//	size += ft_strlen(ANSI_COLOR_RESET);
	if (suffix)
		size += ft_strlen(suffix);
	prompt = ft_calloc(size, sizeof(char));
	if (!prompt)
		return (NULL);
//	ft_strlcat(prompt, COLOR_KHDER_FATH, size);
	ft_strlcat(prompt, str, size);
//	ft_strlcat(prompt, ANSI_COLOR_RESET, size);
	if (suffix)
		ft_strlcat(prompt, suffix, size);
	return (prompt);
}
/* new things */

char	**grep_paths(char **env)
{
	char	*path_env;
	char	**path_dirs;

	//printArray(env);
	path_env = get_value("PATH", env);
	if (!path_env)
		path_env = "";
	//printf("path_env : %s\n", path_env);
	path_dirs = ft_split(path_env, ':');
	if (!path_dirs)
		return (perror(COMMAND_NOT_FOUND), NULL);
	return (path_dirs);
}

char **creat_temp_env(t_context *ctx)
{
	char	**temp_env;

	ctx->unset_path = 1;
	temp_env = malloc(sizeof(char *) * 4 + 1);
	if (!temp_env)
		return (perror("minishell: malloc error\n"), NULL);
	temp_env[4] = NULL;
	temp_env[0] = ft_strdup(FIRST_ENV);
	if (!temp_env[0])
		return (perror("minishell: malloc error\n"), NULL);
	temp_env[1] = ft_strdup(SECOND_ENV);
	if (!temp_env[1])
		return (perror("minishell: malloc error\n"), NULL);
	temp_env[2] = ft_strdup(THIRD_ENV);
	if (!temp_env[2])
		return (perror("minishell: malloc error\n"), NULL);
	temp_env[3] = ft_strdup(TEMP_PATH);
	if (!temp_env[3])
		return (perror("minishell: malloc error\n"), NULL);
	return temp_env;
}
	
	
char	**ft_creat_env(t_context *ctx)
{
	char **env;
	extern char **environ;
	int	env_count;
	int i;

	i = 0;
	env_count = 0;
	if (!*environ)
		return (creat_temp_env(ctx));
	while (environ[env_count])
		env_count++;
	env = malloc(sizeof(char *) * (env_count + 1));	
	if (!env)
	{
		printf("env malloc error\n");
		return NULL;
	}
	while (environ[i])
	{
		env[i] = ft_strdup(environ[i]);
		if (!env[i])
		{
			perror("minishell: malloc error\n");	
			return NULL;
		}
		i++;
	}
	env[i] = NULL;
	return env;
}

int init_context(t_context *ctx)
{
	ctx->env = NULL;
	ctx->unset_path = 0;
	ctx->env = ft_creat_env(ctx);
	if (ctx->env == NULL)
	{
		while(ctx->env && *(ctx->env))
		{
			free(*(ctx->env));
			ctx->env++;
		}
		return (ft_printf("env	is NULL\n"), 1);
	}
	ctx->last_pwd = get_value("PWD", ctx->env);
	if (!ctx->last_pwd)
		ctx->last_pwd = ft_strdup("");
	return 0;
}

int	my_putchar(int c)
{
	return (write(1, &c, 1));
}

int	get_state(int new_state, int flags)
{
	static int	state;

	if (flags == SET_STATE)
		state = new_state;
	return (state);
}

void	init_terminal(void)
{
	struct termios	old_term;
	struct termios	new_term;

	if (!isatty(STDIN_FILENO))
		return ;
	tcgetattr(STDIN_FILENO, &old_term);
	//ft_memcpy(&new_term, &old_term, sizeof(struct termios));
	new_term = old_term;
	new_term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
}
void	handle_interrupt(int sig)
{
	(void)sig;
	// if (!isatty(STDIN_FILENO)) // 	return ;
	// tcgetattr(STDIN_FILENO, &old_term);
	// ft_memcpy(&new_term, &old_term, sizeof(struct termios));
	// new_term.c_lflag &= ~(ICANON | ECHO);
	// tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
	// write(1, "\033[6n", 4);
	// int x, y;
	// read_cursor_pos(&x, &y);
	// tcsetattr(STDIN_FILENO, TCSANOW, &old_term);
	// rl_replace_line("", 1);
	// ft_printf("\n");
    // rl_on_new_line();
	// rl_redisplay();
	//tputs(tgetstr("ce", NULL), 1, my_putchar);
	get_status(1, SET_STATUS);
	rl_replace_line("", 1);
	tputs(tgetstr("do", NULL), 1, my_putchar);
	if (get_state(0, 0) == ON_PROMPT)
	{
		rl_on_new_line();
		rl_redisplay();
	}
	//tputs(tgetstr("cr", NULL), 1, my_putchar);
	// tputs(tgetstr("ce", NULL), 1, my_putchar);
}

void	handle_quit(int sig)
{
	(void)sig;
	if (get_state(0, 0) == ON_EXEC)
		ft_putendl_fd("Quit: 3", 1);
	else
		rl_redisplay();
	//get_status(131, SET_STATUS);
}

int	main(void)
{
	char	*line;
	char	*pwd;
	char	*prompt;
	t_list	*tokens;
	t_context	ctx;
	t_btree	*parse_tree;
	struct sigaction	saint;

	if (init_context(&ctx))
		return 1;
	//printf("path : %d\n", ctx.unset_path);
	saint.sa_handler = handle_interrupt;
	sigemptyset(&saint.sa_mask);
	sigaction(SIGINT, &saint, NULL);
	signal(SIGQUIT, handle_quit);
	get_state(ON_PROMPT, SET_STATE);
	init_terminal();
	while (1)
	{
		//print_prompt_with_user_details();
		//line = readline("");
		//pwd = ft_path();


		get_state(ON_PROMPT, SET_STATE);
		pwd = "minishell >";
		prompt = get_prompt(pwd, "$ ");

	//	printf(COLOR_KHDER_FATH "%s" ANSI_COLOR_RESET "$ ", pwd);
	//	free(pwd);

		line = readline(prompt);
		get_state(ON_EXEC, SET_STATE);
		if (!line)
			exit(0);
		tokens = lexer(line);	
		if (!tokens)
		{
			free(line);
			continue ;
		}
		add_history(line);
		parse_tree = parse(tokens);
		next_token(tokens, RESET_TOK);
		if (!parse_tree)
			continue;
		if (prompt_heredoc(parse_tree) < 0)
			continue;
		//ft_printf("----------- EXECUTION ---------\n");
 		get_status(__exec(parse_tree, &ctx), SET_STATUS);
		clear_btree(parse_tree, NULL);	
		ft_lstclear_libft(&tokens, free);
		free(line);
		init_terminal();
	}
	return (0);
}
