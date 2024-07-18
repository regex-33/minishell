/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 08:53:51 by bchanaa           #+#    #+#             */
/*   Updated: 2024/07/18 08:53:53 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	panic(char *prog_name, int err, char c)
{
	if (prog_name)
	{
		ft_putstr_fd(prog_name, 2);
		ft_putstr_fd(": ", 2);
	}
	if (err == PERR_UNEXP_TOK)
		ft_putendl_fd("parse error: unexpected token", 2);
	else if (err == PERR_EXP_TOK)
		ft_putendl_fd("parse error: expected token not found", 2);
	else if (err == PERR_UNC_PAR)
		ft_putendl_fd("parse error: unclosed parenthesis", 2);
	else if (err == PERR_UNC_QUOT)
		ft_putendl_fd("parse error: unclosed quotes", 2);
	else if (err == PERR_NEAR)
	{
		ft_putstr_fd("parse error near ", 2);
		write(2, &c, 1);
		ft_putendl_fd("", 2);
	}
	return (0);
}

t_token	*next_token(t_list *token_list, int flags)
{
	static t_list	*current;
	static int		is_init;

	if (flags & RESET_TOK)
	{
		is_init = 0;
		current = NULL;
		return (NULL);
	}
	if (!is_init)
	{
		current = token_list;
		is_init = 1;
	}
	if (current && (flags & CONSUME_TOK))
		current = current->next;
	if (!current)
		return (NULL);
	return ((t_token *)current->content);
}

int	expect(t_token_type type, t_list *token_list)
{
	t_token	*token;

	token = next_token(token_list, 0);
	if (!token)
		return (0);
	if (token->type == type)
		next_token(token_list, CONSUME_TOK);
	else
		return (0);
	return (1);
}

int	is_operator(t_token *token)
{
	if (!token)
		return (0);
	return (token->type == tok_pipe || token->type == tok_op_or \
			|| token->type == tok_op_and);
}

int	get_prec(t_token *token)
{
	if (!token)
		return (0);
	if (token->type == tok_op_or || token->type == tok_op_and)
		return (1);
	if (token->type == tok_pipe)
		return (2);
	return (0);
}
