#include "minishell.h"

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

char *skip_quotes_alloc(char *str)
{
	char	*new_str;
	size_t	len;
	size_t	i;

	len = ft_strlen(str);
	new_str = ft_calloc(len + 1, sizeof(char));
	if (!new_str)
		return (NULL);
	i = 0;
	while (*str)
	{
		if (!isquote(*str))
			new_str[i++] = *str;
		str++;
	}
	if (i < len)
		new_str[i] = '\0';
	return (new_str);
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
		if (handle_heredoc(redir, ctx))
			return (reset_redir(redir_list, 1), -1);
		// redir->delimiter = skip_quotes_alloc(redir->delimiter);
		// printf("delimiter: %s\n", redir->delimiter);
		fd = open(redir->filename, O_RDONLY);
		if (fd < 0)
			return (reset_redir(redir_list, 1), -1);
		unlink(redir->filename);
	}
	if (fd < 0)
		return (reset_redir(redir_list, 1), ft_putstr_fd("minishell: ", 2), perror(redir->filename), -1);
	return (fd);
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