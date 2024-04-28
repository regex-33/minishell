#include "minishell.h"


char	*ft_which(char *cmd, char **path_dirs)
{
	char	*tmp; char	*cmd_pathname;
	int		i;

	if (ft_strchr(cmd, '/'))
	{ if (!access(cmd, F_OK) && !access(cmd, X_OK))
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


char **expand_filename(char *filename, char **env)
{
	char **files = NULL;
	t_list *expanding_list = NULL;

	if (!expand_arg_list(&expanding_list, filename, env))
	{
		perror("minishell");
		return NULL;
	}
	if (expanding_list)
    {
        files = ft_list_to_array(expanding_list);
        if (!files)
        {
            freeLinkedList(expanding_list);
            return NULL;
        }
        //freeLinkedList(expanding_list);
		//printArray(args);
        //free_array(args);
		return files;
    }
	return NULL;
}
int count_array(char **array)
{
	int i;

	i = 0;
	if (!array || !*array)
		return 0;
	while (array[i])
		i++;
	return i;
}

int open_files(t_list *redir_list, char ***env)
{
	t_redir	*redir;
	int				fd;
	char		**files;
	fd = 1;
	//t_token *token;
	
	if (!redir_list)
		return (fd);
	(void)fd;
	printf("I am in open_files\n");
	while (redir_list)
	{
        redir = redir_list->content;
		files = expand_filename(redir->filename, *env);
		if (!files)
		{
			perror("minishell");
			return -1;
		}
		if (count_array(files) > 1)
		{
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd("ambiguous redirect\n", STDERR_FILENO);
			free_array(files);
			return -1;
		}
		printArray(files);
		/*
        redir = redir_list->content;
		if (redir->type == REDIR_IN)
			fd = open(redir->filename, O_RDONLY);
		else if (redir->type == REDIR_OUT)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == REDIR_APPEND)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == REDIR_HERE)
		{
			fd = open(redir->filename, O_RDONLY);
			unlink(redir->filename);
		}
		if (fd < 0)
		{
			perror("minishell");
			return -1;
		}
		else
			dup2(fd, redir->fd);
		//if (redir != NULL)
		close(fd);
		*/
        redir_list = redir_list->next;
	}
	return (fd);
}

pid_t	exec_piped_cmd(t_btree *leaf, char ***env, int pipes[2][2])
{
	pid_t	pid;
	t_cmd	*cmd;
	char	**args;

	pid = fork();
	if (pid < 0)
		return (perror("minishell"), exit(1), -1);
	if (pid == 0)
	{
		cmd = leaf->data;
		close(pipes[OUT_PIPE][READ]);
		if (dup2(pipes[IN_PIPE][READ], STDIN_FILENO) < 0)
			return (close(pipes[OUT_PIPE][WRITE]), exit(1), 0);
		dup2(pipes[OUT_PIPE][WRITE], STDOUT_FILENO);

		if (pipes[IN_PIPE][READ] != STDIN_FILENO)
			close(pipes[IN_PIPE][READ]);

		close(pipes[OUT_PIPE][WRITE]);
		args = get_expanded_args(cmd, *env);
		args = get_cmd_args(args, grep_paths(*env));
		if (!args)
			return (perror("minishell:"), 0);
		if (execve(args[0], args, *env))
			return (perror("minishell"), exit(1), 0);
	}
	else
	{
		if (pipes[IN_PIPE][READ] != STDIN_FILENO)
			close(pipes[IN_PIPE][READ]);
		return (close(pipes[OUT_PIPE][WRITE]), pid);
	}
	return (-1);
}

pid_t	exec_last_piped_cmd(t_btree *leaf, char ***env, int fd[2])
{
	pid_t	pid;
	t_cmd	*cmd;
	char	**args;

	pid = fork();
	if (pid < 0)
		return (perror("minishell"), exit(1), -1);
	if (pid == 0)
	{
		cmd = leaf->data;
		if (dup2(fd[READ], STDIN_FILENO))
			return (close(fd[READ]), exit(1), 0);
		close(fd[READ]);
		args = get_expanded_args(cmd, *env);
		args = get_cmd_args(args, grep_paths(*env));
		if (!args)
			return (perror("minishell:"), 0);
		if (execve(args[0], args, *env))
			return (perror("minishell"), exit(1), 0);
	}
	else
	{
		return (close(fd[READ]), pid);
	}
	return (-1);
}

pid_t exec_cmd(t_list *redir_list, char **args, char ***env)
{
	char **cmd_args;
	extern char **environ;
	extern int last_exit_status;
	char **path_dirs;
    pid_t pid;
	int fd;

    //printf("i am in exec_cmd\n ");
    path_dirs = grep_paths(*env);
	if (!path_dirs)
		return -1;
	int status = select_buildin_commands(args, redir_list, env);
	if (status != -1)
		return (status);
    pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        return -1;
    }
    if (pid == 0)
    {
		fd = open_files(redir_list, env);
		if (fd < 0)
			return (0);
		cmd_args = get_cmd_args(args, path_dirs);
		if (!cmd_args)
			exit(EXIT_FAILURE);
		//printf("CMD ARGS: %s ==> args : %s\n", cmd_args[0], cmd_args[1]);
		execve(cmd_args[0], cmd_args, *env);
		perror("Execve failed");
		exit(EXIT_FAILURE);
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
                //ft_printf("Command execution failed with status %d\n", exit_status);
				last_exit_status = exit_status;
                return (exit_status);
            }
        }
		else if (WIFSIGNALED(status))
		{
			last_exit_status = WTERMSIG(status) + 128;
			return (last_exit_status);
		}
        return pid;
    }
    return pid;
}

// echo * segv when no file on dir/h
// export
