/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expanding.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:46:52 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 13:46:53 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_value(char *name, char **env)
{
	int	i;

	i = 0;
	if (ft_strcmp(name, "?") == 0)
		return (ft_itoa(get_status(0, 0)));
	while (env && *env)
	{
		i = 0;
		while ((*env)[i] == name[i] && name[i] != '\0')
			i++;
		if ((*env)[i] == '=' && name[i] == '\0')
			return (ft_strdup(*env + i + 1));
		env++;
	}
	return (NULL);
}

char	**expand_filename_here_doc(char *filename, t_context *ctx, int flag)
{
	char	**files;
	t_list	*expanding_list;
	char	**temp;

	files = NULL;
	temp = NULL;
	expanding_list = NULL;
	if (!expand_arg_list(&expanding_list, filename, ctx, flag))
		return (NULL);
	if (expanding_list)
	{
		files = ft_list_to_array(expanding_list, temp, 0);
		if (!files)
			return (free_linked_list(expanding_list), NULL);
		return (free_linked_list(expanding_list), files);
	}
	return (NULL);
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
		args = expand_filename_here_doc(line, ctx, 1);
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

int	read_and_without_expand_heredoc(int old_fd, int new_fd)
{
	char	*line;

	line = NULL;
	line = get_next_line(old_fd);
	while (line)
	{
		ft_putstr_fd(line, new_fd);
		free(line);
		line = get_next_line(old_fd);
	}
	return (0);
}

int	handle_heredoc(char **filename, t_context *ctx, char *delimiter)
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
	if (ft_strchr(delimiter, '\'') || ft_strchr(delimiter, '\"'))
		read_and_without_expand_heredoc(old_fd, new_fd);
	else if (read_and_expand_heredoc(old_fd, new_fd, ctx) == -1)
		return (-1);
	close(old_fd);
	close(new_fd);
	unlink(*filename);
	*filename = new_filename;
	return (0);
}
