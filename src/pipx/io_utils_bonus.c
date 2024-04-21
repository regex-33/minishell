/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   io_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 20:55:50 by bchanaa           #+#    #+#             */
/*   Updated: 2024/03/14 21:41:28 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex_bonus.h"

/*
 * Writes str to fd.
 * If valid, environment variables in str will be expanded and written to fd.
*/

int	put_heredoc_fd(char *str, char **env, int fd)
{
	int		i;
	char	*env_var;
	int		wlen;

	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			i++;
			env_var = ft_getenv(str + i, env);
			if (env_var)
				wlen = write(fd, env_var, ft_strlen(env_var));
			while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
				i++;
		}
		else
			wlen = write(fd, str + i++, 1);
		if (wlen < 0)
			return (-1);
	}
	return (0);
}

/*
 * Reads from stdin and writes to fd until limiter is encountered.
*/

int	read_heredoc(char *limiter, char **env, int fd)
{
	char	*line;
	size_t	limiter_len;

	if (!limiter)
		return (ft_putendl_fd("pipex: invalid limiter", 1), -1);
	limiter_len = ft_strlen(limiter);
	ft_putstr_fd("heredoc> ", STDOUT_FILENO);
	line = get_next_line(STDIN_FILENO);
	while (line)
	{
		if (limiter_len == ft_strlen(line) - 1 && \
			!ft_strncmp(line, limiter, limiter_len))
			return (free(line), get_next_line(INVALID_FD), 0);
		ft_putstr_fd("heredoc> ", STDOUT_FILENO);
		if (put_heredoc_fd(line, env, fd) < 0)
			return (free(line), get_next_line(INVALID_FD), 0);
		free(line);
		line = get_next_line(STDIN_FILENO);
	}
	get_next_line(INVALID_FD);
	return (0);
}

/*
 * If heredoc is specified, writes stdin to pipe.
 * Otherwise redirects pipe read end to the input file fd.
*/

int	handle_input(t_args *args, int fd[])
{
	if (args->is_heredoc)
	{
		read_heredoc(args->limiter, args->env, fd[WRITE]);
		return (close(fd[WRITE]), 0);
	}
	if (close(fd[WRITE]) < 0)
		return (-1);
	fd[READ] = open(args->av[1], O_RDONLY);
	if (fd[READ] < 0)
		perror(PROG_NAME);
	return (0);
}

/*
 * Switches OUT and IN pipes and redirects OUT pipe to a new pipe.
*/

int	add_pipe(int pipes[2][2])
{
	int	tmp[2];

	if (pipe(tmp) == -1)
		return (-1);
	pipes[IN_PIPE][READ] = pipes[OUT_PIPE][READ];
	pipes[IN_PIPE][WRITE] = pipes[OUT_PIPE][WRITE];
	pipes[OUT_PIPE][READ] = tmp[READ];
	pipes[OUT_PIPE][WRITE] = tmp[WRITE];
	return (0);
}
