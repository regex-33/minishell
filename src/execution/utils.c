/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:21:06 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 14:21:07 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_file(char *path)
{
	struct stat	sb;

	if (stat(path, &sb))
		return (0);
	return ((sb.st_mode & S_IFMT) != S_IFDIR);
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

int	check_abs_cmd(char *pathname)
{
	struct stat	sb;

	if (stat(pathname, &sb))
	{
		if (errno == ENOENT)
			return (print_err(pathname, " : No such file or directory."), 127);
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

char	**get_expanded_args(t_cmd *cmd, t_context *ctx)
{
	t_list	*list_args;
	t_list	*expanding_list;
	char	**args;
	char	**temp;

	temp = NULL;
	expanding_list = NULL;
	if (!cmd)
		return (NULL);
	list_args = cmd->cmd_args;
	expanding_list = expand_list(list_args, ctx);
	if (expanding_list)
	{
		args = ft_list_to_array(expanding_list, temp, 0);
		if (!args)
			return (free_linked_list(expanding_list), NULL);
		return (free_linked_list(expanding_list), args);
	}
	return (NULL);
}
