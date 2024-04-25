#include "minishell.h"


char	*ft_which(char *cmd, char **path_dirs)
{
	char	*tmp;
	char	*cmd_pathname;
	int		i;

	if (ft_strchr(cmd, '/'))
	{
		if (!access(cmd, F_OK) && !access(cmd, X_OK))
			return (ft_strdup(cmd));
		return (NULL);
	}
	tmp = ft_strjoin("/", cmd);
	if (!tmp)
		return (NULL);
	i = -1;
	while (path_dirs[++i])
	{
		cmd_pathname = ft_strjoin(path_dirs[i], tmp);
		if (!cmd_pathname)
			return (free(tmp), NULL);
		if (!access(cmd_pathname, F_OK) && !access(cmd_pathname, X_OK))
			return (free(tmp), cmd_pathname);
		free(cmd_pathname);
	}
	return (free(tmp), NULL);
}

char	**get_cmd_args(char	**cmd_args, char **path_dirs)
{
	char	*cmd_pathname;

	if (!cmd_args || !path_dirs)
		return (NULL);
	cmd_pathname = ft_which(cmd_args[0], path_dirs);
	//printf("CMD PATHNAME: %s, cmd : %s\n", cmd_pathname, cmd);
	if (!cmd_pathname)
	{
		if (!ft_strchr(cmd_args[0], '/'))
		{
			ft_putstr_fd(CMD_NOT_FOUND, STDERR_FILENO);
			ft_putendl_fd(cmd_args[0], STDERR_FILENO);
			return (ft_free_arr(cmd_args), NULL);
		}
		return (ft_free_arr(cmd_args), perror("minishell"), NULL);
	}
	free(cmd_args[0]);
	cmd_args[0] = cmd_pathname;
	return (cmd_args);
}

int open_files(t_list *redir_list)
{
	t_list	*redir;
	int				fd;
	t_token

	redir = redir_list->content;
	while (redir)
	{
		if (redir->type == REDIR_IN)
			fd = open(redir->filename, O_RDONLY);
		else if (redir->type == REDIR_OUT)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == REDIR_OUT_APPEND)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
		{
			perror("minishell");
			return -1;
		}
		else
			dup2(fd, redir->fd)
		redir = redir->next;
		if (redir != NULL)
			close(fd);
	}
	return (1);
}


pid_t exec_cmd(t_list *redir_list, char **args, char **env)
{
//  char **cmd_args;
    char **path_dirs;
    pid_t pid;
	int fd;

    path_dirs = grep_paths(env);
    pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        return -1;
    }
    if (pid == 0)
    {
		fd = open_files(redir_list);
		if (fd < 0)
			return (0);
		if (!select_buildin_commands(args))
		{
			// cmd_args = get_cmd_args(args, path_dirs);
			// if (!cmd_args)
			// 	exit(EXIT_FAILURE);
			//printf("CMD ARGS: %s ==> args : %s\n", cmd_args[0], cmd_args[1]);
			execve(args[0], args, NULL);
			perror("Execve failed");
			exit(EXIT_FAILURE);
		}
		exit(EXIT_SUCCESS);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0)
            {
                ft_printf("Command execution failed with status %d\n", exit_status);
				return 0;
            }
        }
        return pid;
    }
}