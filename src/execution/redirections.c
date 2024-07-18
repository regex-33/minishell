/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:04:27 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 14:04:27 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

int	open_file(char *file_name, t_redir *redir, \
				t_context *ctx, t_list *redir_list)
{
	int	fd;

	fd = 1;
	if (redir->type == REDIR_IN)
		fd = open(file_name, O_RDONLY);
	else if (redir->type == REDIR_OUT)
		fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (redir->type == REDIR_APPEND)
		fd = open(file_name, O_WRONLY | O_CREAT | O_APPEND, 0644);
	else if (redir->type == REDIR_HERE)
	{
		if (handle_heredoc(&file_name, ctx))
			return (reset_redir(redir_list, 1), -1);
		fd = open(file_name, O_RDONLY);
		if (fd < 0)
			return (reset_redir(redir_list, 1), -1);
		unlink(file_name);
	}
	if (fd < 0)
		return (reset_redir(redir_list, 1), ft_putstr_fd("minishell: ", 2),
			perror(file_name), -1);
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
		if (!files || count_array(files) > 1)
			return (reset_redir(redir_list, 1), ft_putstr_fd("minishell: ", 2),
				ft_putstr_fd("ambiguous redirect\n", 2), free_array(files), 1);
		fd = open_file(files[0], redir, ctx, redir_list);
		if (fd < 0)
			return (1);
		free_array(files);
		redir->bak_fd = dup(redir->fd);
		if (redir->bak_fd < 0)
			return (reset_redir(redir_list, 1), print_fd_err(redir->fd), 1);
		dup2(fd, redir->fd);
		close(fd);
		redir_list = redir_list->next;
	}
	return (0);
}
