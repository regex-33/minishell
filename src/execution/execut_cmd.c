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

void	print_err(char *mid, char *suffix)
{
	ft_putstr_fd("minishell: ", 2);
	if (mid && !suffix)
		ft_putendl_fd(mid, 2);
	if (mid && suffix)
		ft_putstr_fd(mid, 2);
	if (suffix)
		ft_putendl_fd(suffix, 2);
}

int	is_valid_cmd(char *pathname)
{
	struct stat sb;

	if (stat(pathname, &sb))
	{
		if (errno == ENOENT)
			return (print_err(pathname, " : command not found."), 127);
		if (errno == EACCES)
			return (print_err(pathname, " : Permission denied."), 126);
	}
	if ((sb.st_mode & S_IFMT) == S_IFDIR)
		return (print_err(pathname, " : is a directory."), 126);
	if (access(pathname, X_OK))
		return (print_err(pathname, " : Permission denied."), 126);
	return (0);
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


char **expand_filename_here_doc(char *filename, char **env)
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
        freeLinkedList(expanding_list);
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
int handle_heredoc(char **filename, char **env)
{
	int new_fd;
	int old_fd;
	char	**args = NULL;
	char	*line = NULL;
	char	*new_filename = NULL;
	char	*expanded_line = NULL;

	new_filename = random_filename();
	if (!new_filename)
		return (perror("minishell"), -1);
	new_fd = open(new_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (new_fd < 0)
		return (perror("minishell"), -1);
	//printf("OLD FILENAME : %s |  NEW FILENAME: %s\n", *filename, new_filename);

	old_fd = open(*filename, O_RDONLY);
	if (old_fd < 0)
		return (perror("minishell"), -1);
	line = get_next_line(old_fd);
	while (line)
	{
		args =	expand_filename_here_doc(line, env);
		if (!args)
			return (perror("minishell"), -1);
		expanded_line = join_strings(args, 0);
		if (!expanded_line)
			return (perror("minishell"), -1);
		ft_putstr_fd(expanded_line, new_fd);


		free(expanded_line);
		free_array(args);
		free(line);
		line = get_next_line(old_fd);
	}
	close(old_fd);
	close(new_fd);
	unlink(*filename);
	*filename = new_filename;
	return 0;
}

void	restore_redir(t_list *redir_list)
{
	t_redir	*redir;

	while (redir_list)
	{
		redir = redir_list->content;
		if (redir->bak_fd >= 0)
		{
			dup2(redir->bak_fd, redir->fd);
			close(redir->bak_fd);
		}
		redir_list = redir_list->next;
	}
}

int	open_files(t_list *redir_list, char **env)
{
	t_redir	*redir;
	char		**files;
	int	fd;
	//t_token *token;
	
	while (redir_list)
	{
        redir = redir_list->content;
		files = expand_filename_here_doc(redir->filename, env);
		if (!files)
		{
			restore_redir(redir_list);
			perror("minishell");
			return (1);
		}
		if (count_array(files) > 1)
		{
			restore_redir(redir_list);
			ft_putstr_fd("minishell: ", STDERR_FILENO);
			ft_putstr_fd("ambiguous redirect\n", STDERR_FILENO);
			free_array(files);
			return (1);
		}
		if (redir->type == REDIR_IN)
			fd = open(redir->filename, O_RDONLY);
		else if (redir->type == REDIR_OUT)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (redir->type == REDIR_APPEND)
			fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (redir->type == REDIR_HERE)
		{
			if (handle_heredoc(&redir->filename, env))
				return (restore_redir(redir_list), perror("minishell"), 1);
			fd = open(redir->filename, O_RDONLY);
			if (fd < 0)
				return (restore_redir(redir_list), perror("minishell"), 1);
			unlink(redir->filename);
			//printf("FILENAME: %s\n", redir->filename);
		}
		if (fd < 0)
			return (restore_redir(redir_list), perror("minishell"), 1);
		redir->bak_fd = dup(redir->fd);
		if (redir->bak_fd < 0)
			return (restore_redir(redir_list), perror("minishell"), 1);
		dup2(fd, redir->fd);
		close(fd);
        redir_list = redir_list->next;
	}
	return (0);
}

pid_t	exec_piped_cmd(t_btree *tree, t_context *ctx, int pipes[2][2])
{
	pid_t	pid;
	t_cmd	*cmd;
	char	**args;
	t_list	*redir_list;

	pid = fork();
	if (pid < 0)
		return (perror("minishell"), exit(1), -1);
	if (pid == 0)
	{
		close(pipes[OUT_PIPE][READ]);
		if (dup2(pipes[IN_PIPE][READ], STDIN_FILENO) < 0)
			return (close(pipes[OUT_PIPE][WRITE]), exit(1), 0);
		dup2(pipes[OUT_PIPE][WRITE], STDOUT_FILENO);
		close(pipes[OUT_PIPE][WRITE]);
		if (pipes[IN_PIPE][READ] != STDIN_FILENO)
			close(pipes[IN_PIPE][READ]);

		if (tree->type == nt_subcmd)
			redir_list = tree->data;
		else
			redir_list = ((t_cmd *)tree->data)->redir_list;
		if (open_files(redir_list, ctx->env))
			return (exit(1), 0);
		if (tree->type == nt_subcmd)
		{
			if (open_files(redir_list, ctx->env))
				return (exit(1), 0);
			exit(__exec(tree->left, ctx));
		}
		cmd = tree->data;
		args = get_expanded_args(cmd, ctx->env);
		args = get_cmd_args(args, grep_paths(ctx->env));
		if (!args)
			return (exit(1), 0);
		if (execve(args[0], args, ctx->env))
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

pid_t	exec_last_piped_cmd(t_btree *tree, t_context *ctx, int fd[2])
{
	pid_t	pid;
	t_cmd	*cmd;
	t_list	*redir_list;
	char	**args;

	pid = fork();
	if (pid < 0)
		return (perror("minishell"), exit(1), -1);
	if (pid == 0)
	{
		if (dup2(fd[READ], STDIN_FILENO))
			return (close(fd[READ]), exit(1), 0);
		close(fd[READ]);
		if (tree->type == nt_subcmd)
			redir_list = tree->data;
		else
			redir_list = ((t_cmd *)tree->data)->redir_list;
		if (open_files(redir_list, ctx->env))
			return (exit(1), 0);
		if (tree->type == nt_subcmd)
		{
			if (open_files(redir_list, ctx->env))
				return (exit(1), 0);
			exit(__exec(tree->left, ctx));
		}
		cmd = tree->data;
		args = get_expanded_args(cmd, ctx->env);
		args = get_cmd_args(args, grep_paths(ctx->env));
		if (!args)
			return (exit(1), 0);
		if (execve(args[0], args, ctx->env))
			return (perror("minishell"), exit(1), 0);
	}
	else
		return (close(fd[READ]), pid);
	return (-1);
}

int	exec_cmd(t_list *redir_list, char **args, t_context *ctx)
{
	char **cmd_args;
	extern int last_exit_status;
	char **path_dirs;
    pid_t pid;

    path_dirs = grep_paths(ctx->env);
	if (!path_dirs)
		return -1;
	int status = select_buildin_commands(args, redir_list, ctx);
	if (status != -1)
		return (status);
    pid = fork();
    if (pid < 0)
        return (perror("minishell"), 0);
    if (pid == 0)
    {
		if (open_files(redir_list, ctx->env))
			return (exit(EXIT_FAILURE), 0);
		cmd_args = get_cmd_args(args, path_dirs);
		if (!cmd_args)
			exit(EXIT_FAILURE);
		if (execve(cmd_args[0], cmd_args, ctx->env))
			return (perror("minishell"), exit(1), 0);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
            ctx->last_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			ctx->last_status = WTERMSIG(status) + 128;
        return (ctx->last_status);
    }
    return (1);
}

// echo * segv when no file on dir/h
// export
