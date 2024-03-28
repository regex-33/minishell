#include "../inc/minishell.h"
#include <errno.h>

int		last_exit_status = 0;

void	execute_command(char *command)
{
	pid_t	pid;
	int		status;
	char	*args[] = {"/bin/sh", "-c", command, NULL};

	pid = fork();
	if (pid < 0)
	{
		perror("error fork");
		exit(EXIT_FAILURE);
	}
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
int	select_buildin_commands(char **args, char *line)
{
	if (!ft_strcmp(args[0], "cd"))
		ft_change_dir(args[1]);
	else if (!ft_strcmp(args[0], "pwd"))
		ft_pwd();
	else if (!ft_strcmp(args[0], "exit"))
		ft_exit(args[1]);
	else if (!ft_strcmp(args[0], "echo"))
		ft_echo(args, line);
	else if (!ft_strcmp(args[0], "env"))
		ft_env();
	else if (!ft_strcmp(args[0], "export"))
		ft_export(args[1]);
	else if (!ft_strcmp(args[0], "get"))
		get_value(args[1]);
	else if (!ft_strcmp(args[0], "unset"))
		ft_unset(args[1]);
	else
		return (0);
	return (1);
}


int	main(void)
{
	char	*line;
	char	**args;
	char	full_path[200];
	int		i;

	while (1)
	{
		print_prompt_with_user_details();

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