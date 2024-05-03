#include "minishell.h"

char	*ft_which(char *cmd, char **path_dirs)
{
	char	*tmp;
	char	*cmd_pathname;
	int		i;

	if (!path_dirs)
		return (NULL);
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

int	check_cmd_name(char *pathname)
{
	struct stat	sb;

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

int	init_command(t_prexec *pexec, t_context *ctx, char **args)
{
	char	**path_dirs;

	pexec->err = 0;
	pexec->cmd_name = NULL;
	if (*(args[0]) == 0)
	{
		return (0);
	}
	path_dirs = grep_paths(ctx->env);
	if (ft_strchr(args[0], '/'))
	{
		pexec->err = check_cmd_name(args[0]);
		if (pexec->err)
			return (pexec->err);
		pexec->cmd_name = ft_strdup(args[0]);
	}
	else
	{
		pexec->cmd_name = ft_which(args[0], path_dirs);
		if (!pexec->cmd_name)
		{
			pexec->err = 127;
			print_err(args[0], " : command not found");
		}
	}
	pexec->args = args;
	return (pexec->err);
}

char	**get_cmd_args(char **cmd_args, char **path_dirs)
{
	char	*cmd_pathname;

	if (!cmd_args || !path_dirs)
		return (NULL);
	cmd_pathname = ft_which(cmd_args[0], path_dirs);
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

char	**expand_filename_here_doc(char *filename, t_context *ctx)
{
	char	**files;
	t_list	*expanding_list;

	files = NULL;
	expanding_list = NULL;
	if (!expand_arg_list(&expanding_list, filename, ctx))
		return (NULL);
	if (expanding_list)
	{
		files = ft_list_to_array(expanding_list);
		if (!files)
			return (freeLinkedList(expanding_list), NULL);
		return (freeLinkedList(expanding_list), files);
	}
	return (NULL);
}

int	count_array(char **array)
{
	int	i;

	i = 0;
	if (!array || !*array)
		return (0);
	while (array[i])
		i++;
	return (i);
}

int	read_and_expand_heredoc(int old_fd, int new_fd, t_context *ctx)
{
	char	**args;
	char	*line;
	char	*expanded_line;

	args = NULL;
	line = NULL;
	expanded_line = NULL;
	line = get_next_line(old_fd);
	while (line)
	{
		args = expand_filename_here_doc(line, ctx);
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
	return (0);
}

int	handle_heredoc(char **filename, t_context *ctx)
{
	int		new_fd;
	int		old_fd;
	char	*new_filename;

	new_filename = NULL;
	new_filename = random_filename();
	if (!new_filename)
		return (perror("minishell"), -1);
	new_fd = open(new_filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (new_fd < 0)
		return (perror("minishell"), -1);
	old_fd = open(*filename, O_RDONLY);
	if (old_fd < 0)
		return (perror("minishell"), -1);
	if (read_and_expand_heredoc(old_fd, new_fd, ctx) == -1)
		return (-1);
	close(old_fd);
	close(new_fd);
	unlink(*filename);
	*filename = new_filename;
	return (0);
}

// void	restore_redir(t_list *redir_list)
// {
// 	t_redir	*redir;
// 
// 	while (redir_list)
// 	{
// 		redir = redir_list->content;
// 		if (redir->bak_fd >= 0)
// 		{
// 			dup2(redir->bak_fd, redir->fd); // bak = 4, bak = 5
// 			close(redir->bak_fd);
// 			ft_printf("restoring bak: %d to %d", redir->bak_fd, redir->fd);
// 		}
// 		else
// 			ft_printf("no restore\n");
// 		redir_list = redir_list->next;
// 	}
// }

void	reset_redir(t_list *redir_list, int restore)
{
	t_redir	*redir;

	if (!redir_list)
		return ;
	if (redir_list->next)
		reset_redir(redir_list->next, restore);
	redir = redir_list->content;
	if (redir->bak_fd >= 0)
	{
		if (restore)
			dup2(redir->bak_fd, redir->fd);
		close(redir->bak_fd);
	}
}

int open_file(t_redir *redir, t_context *ctx, t_list *redir_list)
{
	int fd;

	fd = 1;
	if (redir->type == REDIR_IN)
		fd = open(redir->filename, O_RDONLY);
	else if (redir->type == REDIR_OUT)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(redir->filename, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->type == REDIR_HERE)
	{
		if (handle_heredoc(&redir->filename, ctx))
			return (reset_redir(redir_list, 1), -1);
		fd = open(redir->filename, O_RDONLY);
		if (fd < 0)
			return (reset_redir(redir_list, 1), -1);
		unlink(redir->filename);
	}
	if (fd < 0)
		return (reset_redir(redir_list, 1), ft_putstr_fd("minishell: ", 2), perror(redir->filename), -1);
	return (fd);
}

void	print_fd_err(int fd)
{
	ft_putstr_fd("minishell: ", 2);
	if (fd < 0)
		ft_putstr_fd("file descriptor out of range", 2);
	else
		ft_putnbr_fd(fd, 2);
	ft_putstr_fd(": ", 2);
	perror(NULL);
}

int	redirect(t_list *redir_list, t_context *ctx)
{
	t_redir	*redir;
	char	**files;
	int		fd;

	while (redir_list)
	{
		redir = redir_list->content;
		files = expand_filename_here_doc(redir->filename, ctx);
		if (!files)
			return (reset_redir(redir_list, 1), perror("minishell"), 1);
		if (count_array(files) > 1)
			return (reset_redir(redir_list, 1), ft_putstr_fd("minishell: ",
					STDERR_FILENO), ft_putstr_fd("ambiguous redirect\n",
					STDERR_FILENO), free_array(files), 1);
		fd = open_file(redir, ctx, redir_list);
		if (fd < 0)
			return (1);
		redir->bak_fd = dup(redir->fd);
		if (redir->bak_fd < 0)
			return (reset_redir(redir_list, 1), print_fd_err(redir->fd), 1);
		dup2(fd, redir->fd);
		close(fd);
		redir_list = redir_list->next;
	}
	return (0);
}


pid_t	exec_piped_cmd(t_btree *tree, t_context *ctx, int pipes[2][2])
{
	pid_t		pid;
	t_cmd		*cmd;
	t_list		*redir_list;
	t_prexec	pexec;
	int			status;

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
		if (tree->type == nt_subcmd)
		{
			if (redirect(redir_list, ctx))
				return (exit(1), 0);
			reset_redir(redir_list, 0);
			exit(__exec(tree->left, ctx));
		}
		cmd = tree->data;
		if (!cmd->cmd_args)
			return (exit(0), 0);
		pexec.args = get_expanded_args(cmd, ctx);
		if (!pexec.args)
			return (perror("minishell"), exit(1), 0);
		status = select_buildin_commands(pexec.args, cmd->redir_list, ctx);
		if (status != -1)
			return (exit(status), 0);
		if (redirect(redir_list, ctx))
			return (exit(1), 0);
		reset_redir(redir_list, 0);
		if (init_command(&pexec, ctx, pexec.args))
			return (exit(pexec.err), 0);
		if (execve(pexec.cmd_name, pexec.args, ctx->env))
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
	pid_t		pid;
	t_cmd		*cmd;
	t_list		*redir_list;
	t_prexec	pexec;
	int			status;

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
		if (tree->type == nt_subcmd)
		{
			if (redirect(redir_list, ctx))
				return (exit(1), 0);
			exit(__exec(tree->left, ctx));
		}
		cmd = tree->data;
		if (!cmd->cmd_args)
			return (exit(0), 0);
		pexec.args = get_expanded_args(cmd, ctx);
		if (!pexec.args)
			return (perror("minishell"), exit(1), 0);
		status = select_buildin_commands(pexec.args, cmd->redir_list, ctx);
		if (status != -1)
			return (exit(status), 0);
		if (redirect(redir_list, ctx))
			return (exit(1), 0);
		reset_redir(redir_list, 0);
		if (init_command(&pexec, ctx, pexec.args))
			return (exit(pexec.err), 0);
		if (execve(pexec.cmd_name, pexec.args, ctx->env))
			return (perror("minishell"), exit(1), 0);
	}
	else
		return (close(fd[READ]), pid);
	return (-1);
}

int	exec_cmd(t_list *redir_list, char **args, t_context *ctx)
{
	extern int	last_exit_status;
	pid_t		pid;
	t_prexec	pexec;
	int			status;

	status = select_buildin_commands(args, redir_list, ctx);
	if (status != -1)
		return (status);
	pid = fork();
	if (pid < 0)
		return (perror("minishell"), 0);
	if (pid == 0)
	{
		if (redirect(redir_list, ctx))
			return (exit(EXIT_FAILURE), 0);
		reset_redir(redir_list, 0);
		if (init_command(&pexec, ctx, args))
			return (exit(pexec.err), 0);
		if (execve(pexec.cmd_name, pexec.args, ctx->env))
			return (perror("minishell"), exit(1), 0);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			status = WTERMSIG(status) + 128;
		return (status);
	}
	return (1);
}

// echo * segv when no file on dir/h
// export
