/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redir_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 08:44:55 by bchanaa           #+#    #+#             */
/*   Updated: 2024/07/18 08:44:56 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	getredir_fd(char *str, int len)
{
	int		fd;
	char	tmp;

	if (str[0] == '<' || str[0] == '>')
		return (str[0] == '>');
	tmp = str[len];
	if (len - 1 > 10 || \
		(len - 1 == 10 && ft_strncmp(str, "2147483647", 10) > 0))
		return (-1);
	str[len] = 0;
	fd = ft_atoi(str);
	str[len] = tmp;
	return (fd);
}

t_redir_type	getredir_type(char *str, int len)
{
	int				i;
	t_redir_type	type;

	i = 0;
	while (ft_isdigit(str[i]))
		i++;
	if (!str[i])
		return (REDIR_APPEND);
	if (str[i] == '>')
		type = REDIR_OUT;
	else
		type = REDIR_IN;
	if (len - i > 1)
		type += str[i] == str[i + 1];
	return (type);
}

t_redir	*new_redir(t_token *redir_token, t_token *file_token)
{
	t_redir	*redir;
	char	*str;

	redir = malloc(sizeof(t_redir));
	if (!redir || !redir_token || !file_token)
		return (free(redir), NULL);
	redir->filename = NULL;
	redir->delimiter = NULL;
	redir->bak_fd = -1;
	redir->fd = getredir_fd(redir_token->value, redir_token->len);
	redir->type = getredir_type(redir_token->value, redir_token->len);
	str = ft_substr(file_token->value, 0, file_token->len);
	if (!str)
		return (free(redir), NULL);
	if (redir->type == REDIR_HERE)
		redir->delimiter = str;
	else
		redir->filename = str;
	return (redir);
}

void	free_redir(void *p_redir)
{
	t_redir	*redir;

	redir = (t_redir *)p_redir;
	if (!redir)
		return ;
	free(redir->filename);
	free(redir->delimiter);
	free(redir);
}

char	*skip_quotes_alloc(char *temp)
{
	char	*new_str;
	int		in_quotes;
	int		in_single_quotes;
	int		i;

	new_str = NULL;
	in_quotes = 0;
	in_single_quotes = 0;
	i = 0;
	while (temp[i])
	{
		if ((temp[i] == '"' && !in_single_quotes) || (temp[i] == '\''
				&& !in_quotes))
		{
			toggle_quotes(temp[i], &in_quotes, &in_single_quotes);
			i++;
			continue ;
		}
		if (!process_char(temp, &new_str, i))
			return (free(new_str), NULL);
		i++;
	}
	return (new_str);
}
