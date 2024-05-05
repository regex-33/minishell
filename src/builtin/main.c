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

void	handle_equal_signal(int signum, siginfo_t *info, void *ptr)
{
	(void)signum;
	(void)ptr;
	(void)info;
	rl_redisplay();
}

void	handle_interrupt(int sig)
{
	(void)sig;
	rl_replace_line("", 1);
	ft_printf("\n");
    rl_on_new_line();
	rl_redisplay();
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
	while (1)
	{
		//print_prompt_with_user_details();
		//line = readline("");
		//pwd = ft_path();

		pwd = "minishell >";
		prompt = get_prompt(pwd, "$ ");

	//	printf(COLOR_KHDER_FATH "%s" ANSI_COLOR_RESET "$ ", pwd);
	//	free(pwd);

		line = readline(prompt);
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
		{
			ft_printf("TREE IS NULL\n");
			continue;
		}
		prompt_heredoc(parse_tree);
		//ft_printf("----------- EXECUTION ---------\n");

 		get_status(__exec(parse_tree, &ctx), SET_STATUS);
		next_token(tokens, RESET_TOK);
		if (!parse_tree)
		{
			free(line);
			//ft_printf("TREE IS NULL\n");
			continue;
		}
		clear_btree(parse_tree, NULL);	
		ft_lstclear_libft(&tokens, free);
		free(line);
	}
	return (0);
}
/*
int	main(void)
{
	char	*line;
	char	**args;
	char	*pwd;
			char full_path[200];
	int		i;

	while (1)
	{
		pwd = ft_path();
		printf(ANSI_COLOR_BLUE "%s" ANSI_COLOR_RESET "$ ", pwd);
		free(pwd);
		line = readline("");
		if (line == NULL || line[0] == '\0')
		{
			printf("\n");
			free(line);
			break ;
		}
		if (line[0] != '\0')
		{
			add_history(line);
			args = ft_split(line, ' ');
			snprintf(full_path, sizeof(full_path), "/bin/%s", args[0]);
			if (!select_buildin_commands(args, line))
			{
				execute_command(line);
			}
			free(line);
			i = 0;
			while (args[i])
			{
				free(args[i]);
				i++;
			}
			free(args);
		}
	}
	return (0);
}
*/
