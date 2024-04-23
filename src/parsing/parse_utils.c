/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 19:38:51 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/23 19:53:33 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	getredir_fd(char *str, int len)
{
	int	fd;
	char	tmp;

	if (str[0] == '<' || str[1] == '>')
		return (str[0] == '>');
	tmp = str[len];
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
	if (str[i] == '>')
		type = REDIR_OUT;
	else
		type = REDIR_IN;
	type += str[i] == str[i + 1];
	return (type);
}

// int	set_redir_attrs(t_redir *redir, t_token *token)
// {
// 	redir->fd = getredir_fd(token->value, token->len);
// 	redir->type = getredir_type(token->value, token->len);
// 	if (redir->type == REDIR_HERE)
// 
// }
