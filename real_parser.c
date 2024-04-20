/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   real_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:20:39 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/20 17:28:10 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	panic(char *str)
{
	ft_putendl_fd(str, 2);
	exit(1);
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
	{
		ft_printf("!!! Consumed: ");
		print_token(current->content);
		ft_printf("\n");
		current = current->next;
	}
	if (!current)
		return (NULL);
	return ((t_token *)current->content);
}

int	expect(t_token_type type, t_list *token_list)
{
	t_token	*token;

	token = next_token(token_list, 0);
	if (!token)
		panic("parse error");
	if (token->type == type)
		next_token(token_list, CONSUME_TOK);
	else
		panic("parse error");
	return (1);
}
