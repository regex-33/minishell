#include "../../inc/minishell.h"

int		last_exit_status = 0;

void	execute_command(char *command) {
	pid_t	pid;
	int		status;
	char	*args[] = {"/bin/sh", "-c", command, NULL};

	pid = fork();
	if (pid < 0)
	{
		perror("error fork");
		exit(EXIT_FAILURE); }
	else if (pid == 0)
	{
		execv("/bin/sh", args);
		perror("execve failed");
		exit(EXIT_FAILURE);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			last_exit_status = WEXITSTATUS(status);
	}
}

int	get_last_exit_status(void)
{
	return (last_exit_status);
}
int	select_buildin_commands(char **args, char ***env)
{
	if (!ft_strcmp(args[0], "cd"))
		ft_change_dir(args[1], *env);
	else if (!ft_strcmp(args[0], "pwd"))
		ft_pwd();
	else if (!ft_strcmp(args[0], "exit"))
		ft_exit(args[1]);
	else if (!ft_strcmp(args[0], "echo"))
		ft_echo(args);
	else if (!ft_strcmp(args[0], "env"))
		ft_env(*env);
	else if (!ft_strcmp(args[0], "export"))
		ft_export(args, env);
	else if (!ft_strcmp(args[0], "get"))
		get_value(args[1], *env);
	else if (!ft_strcmp(args[0], "unset"))
		ft_unset(args[1], env);
	else
		return (0);
	return (1);
}


char	*get_prompt(char *str, char *suffix)
{
	char	*prompt;
	size_t	size;

	if (!str)
		return (NULL);
	size = ft_strlen(str) + 1;
	size += ft_strlen(COLOR_KHDER_FATH);
	size += ft_strlen(ANSI_COLOR_RESET);
	if (suffix)
		size += ft_strlen(suffix);
	prompt = ft_calloc(size, sizeof(char));
	if (!prompt)
		return (NULL);
	ft_strlcat(prompt, COLOR_KHDER_FATH, size);
	ft_strlcat(prompt, str, size);
	ft_strlcat(prompt, ANSI_COLOR_RESET, size);
	if (suffix)
		ft_strlcat(prompt, suffix, size);
	return (prompt);
}
/* new things */

char	**grep_paths(char **env)
{
	char	*path_env;
	char	**path_dirs;

	path_env = get_value("PATH", env);
	if (!path_env)
		path_env = "";
	path_dirs = ft_split(path_env, ':');
	if (!path_dirs)
		return (perror(COMMAND_NOT_FOUND), NULL);
	return (path_dirs);
}

char	**ft_creat_env(void)
{
	char **env;
	extern char **environ;
	int	env_count;
	int i;

	i = 0;
	env_count = 0;
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

int	main(void)
{
	char	*line;
	char	*pwd;
	char	*prompt;
	t_list	*tokens;
	t_btree	*parse_tree;
	char	**env = NULL;

	env = ft_creat_env();
	if (env == NULL)
	{
		while(env && *env)
		{
			free(*env);
			env++;
		}
		printf("env	is NULL\n");
		return 1;
	}
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
		tokens = lexer(line);	
		if (!tokens)
		{

			free(line);
			continue ;
		}
		add_history(line);
		parse_tree = parse(tokens);

		//ft_printf("--------- TREE v2 -------\n");
 		//print_tree(parse_tree, 0, nt_undefined);
 		//ft_printf("--------- COMMAND ----------\n");
 		//ft_printf("%s\n", line);
		//ft_printf("----------- EXECUTION ---------\n");

 		__exec(parse_tree, &env);
		next_token(tokens, RESET_TOK);
		if (!parse_tree)
		{
			free(line);
			ft_printf("TREE IS NULL\n");
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
