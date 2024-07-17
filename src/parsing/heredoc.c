/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 20:01:09 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/25 12:20:54 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*random_filename(void)
{
	unsigned char	filename[SUFF_LEN + 1];
	int				fd;
	int				i;
	int				r_len;

	fd = open("/dev/random", O_RDONLY);
	if (fd < 0)
		return (NULL);
	r_len = read(fd, filename, SUFF_LEN);
	if (r_len <= 0)
		return (NULL);
	i = 0;
	filename[r_len] = 0;
	while (i < r_len)
	{
		filename[i] = (filename[i] % 25) + 65;
		i++;
	}
	close(fd);
	return (ft_strjoin(BASE_HERE_FILENAME, (char *)filename));
}

char *skip_quotes_alloc(char *temp)
{
	char	*new_str = NULL;
	int		i;
	int 	in_single_quotes = 0;
	int		in_quotes = 0;
	char	*char_str = NULL;

	i = 0;
	while (temp[i])
	{
		if ((temp[i] == '"' && !in_single_quotes) || (temp[i] == '\'' && !in_quotes))
		{
			if (temp[i] == '"')
				in_quotes = !in_quotes;
			if (temp[i] == '\'')
				in_single_quotes = !in_single_quotes;
			i++;
		}
		char_str = ft_strndup(temp + i, 1);
		if (!char_str)
			return (free(new_str), NULL);
		new_str = ft_strjoin(new_str, char_str);
		if (!new_str)
			return (free(char_str), NULL);
		free(char_str);
		i++;
	}
	return (new_str);
}

// pid_t	fork_heredoc(char *limiter, size_t lim_len, int fd)
// {
// 	pid_t	pid;
// 	char	*line;

// 	pid = fork();
// 	if (pid < 0)
// 		return (1);
// 	if (pid == 0)
// 	{
// 		signal(SIGINT, SIG_DFL);
// 		ft_putstr_fd(HERE_PROMPT, STDOUT_FILENO);
// 		line = get_next_line(STDIN_FILENO);
// 		while (line)
// 		{
// 			if (lim_len == ft_strlen(line) - 1 && 
// 				!ft_strncmp(line, limiter, lim_len))
// 				return (free(line), get_next_line(INVALID_FD), close(fd), exit(0), 0);
// 			ft_putstr_fd(HERE_PROMPT, STDOUT_FILENO);
// 			if (write(fd, line, ft_strlen(line)) < 0)
// 				return (free(line), get_next_line(INVALID_FD), close(fd), exit(1), 0);
// 			free(line);
// 			line = get_next_line(STDIN_FILENO);
// 		}
// 		close(fd);
// 		ft_putendl_fd("\n", 2);
// 		ft_putendl_fd("minishell: warning: here document expected delimiter not found", 2);
// 		get_next_line(INVALID_FD);
// 		exit(0);
// 	}
// 	return (pid);
// }
pid_t	fork_heredoc(char *lim, size_t lim_len, int fd)
{
	pid_t	pid;
	char	*line;

	pid = fork();
	if (pid < 0)
		return (1);
	if (pid == 0)
	{
		signal(SIGINT, SIG_DFL);
		line = readline(HERE_PROMPT);
		while (line)
		{
			if (lim_len == ft_strlen(line) && !ft_strncmp(line, lim, lim_len))
				return (free(line), close(fd), exit(0), 0);
			if (write(fd, line, ft_strlen(line)) < 0 || write(fd, "\n", 1) < 0)
				return (free(line), close(fd), exit(0), 0);
			free(line);
			line = readline(HERE_PROMPT);
		}
		close(fd);
		ft_putendl_fd("\nminishell: warning: here document expected delimiter not found", 2);
		exit(0);
	}
	return (pid);
}

int	read_heredoc(char *limiter, int fd)
{
	size_t	limiter_len;
	int		status;
	pid_t	heredoc_pid;

	if (!limiter)
		return (ft_putendl_fd("minishell: invalid here document delimiter", 1), -1);
	limiter = skip_quotes_alloc(limiter);
	if (!limiter)
		return (ft_putendl_fd("minishell: invalid here document delimiter", 1), -1);
	limiter_len = ft_strlen(limiter);
	heredoc_pid = fork_heredoc(limiter, limiter_len, fd);
	if (heredoc_pid < 0)
		return (-1);
	waitpid(heredoc_pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	new_heredoc(t_redir *redir)
{
	char	*filename;
	int		i;
	int		fd;

	filename = random_filename();
	if (!filename)
		return (-1);
	i = 0;
	while (i < HERE_MAX_RETRY && !access(filename, F_OK))
	{
		free(filename);
		filename = random_filename();
		if (!filename)
			return (-1);
	}
	fd = open(filename, O_RDWR | O_CREAT, 0644);
	if (fd < 0)
		return (free(filename), -1);
	if (read_heredoc(redir->delimiter, fd) < 0)
		return (unlink(filename), close(fd), free(filename), -1);
	close(fd);
	redir->filename = filename;
	return (0);
}

int	prompt_heredoc(t_btree *tree)
{
	t_list	*redir_list;
	t_redir	*redir;

	if (tree->type != nt_subcmd && tree->type != nt_simplecmd)
	{
		if (tree->left && prompt_heredoc(tree->left) < 0)
			return (-1);
		if (tree->right && prompt_heredoc(tree->right) < 0)
			return (-1);
	}
	else
	{
		if (tree->type == nt_subcmd)
			redir_list = tree->data;
		else
			redir_list = ((t_cmd *)tree->data)->redir_list;
		while(redir_list)
		{
			redir = redir_list->content;
			if (redir->type == REDIR_HERE && new_heredoc(redir) < 0)
				return (-1);
			redir_list = redir_list->next;
		}
	}
	return (0);
}

// int main(int ac, char **av)
// {
// 	t_redir	redir;
// 	char	*filename;
// 
// 	redir.delimiter = av[1];
// 	filename = heredoc(&redir);
// 	if (!filename)
// 		ft_printf("ERROR HEREDOC\n");
// 	return (0);
// }