#include "../inc/minishell.h"

int	main(void)
{
	char	*line;
	char	**args;
	pid_t	pid;
	int		status;

	while (1)
	{
		line = readline("Minishell> ");
		if (line == NULL)
		{
			printf("\n");
			ft_exit();
		}
		if (line[0] != '\0')
		{
			add_history(line);
			args = ft_split(line, ' ');
			char full_path[200];
			snprintf(full_path, sizeof(full_path), "/bin/%s", args[0]);
            if (!strcmp(args[0], "cd"))
				ft_change_dir(args[1]);
			else if (!strcmp(args[0], "pwd"))
				ft_pwd();
			else if (!strcmp(args[0], "exit"))
				ft_exit();
            else if (!strcmp(args[0], "echo"))
                ft_echo(args, line);
            else if (!strcmp(args[0], "env"))
                ft_env();
            else if (!strcmp(args[0], "export"))
                ft_export(args[1]);
            else if (!strcmp(args[0], "get"))
				get_value(args[1]);
            else if (!strcmp(args[0], "unset"))
				ft_unset(args[1]);
			else
			{
				pid = fork();
				if (pid == 0)
				{
					execve(full_path, args, NULL);
					perror("execve failed");
					exit(EXIT_FAILURE);
				}
				else if (pid < 0)
					perror("fork failed");
				else
					waitpid(pid, &status, 0);
			}
            int i = 0;
			while (args[i])
			{
				free(args[i]);
                i++;
			}
		}
		free(line);
	}
	return (0);
}
