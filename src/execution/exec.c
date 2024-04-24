/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 23:15:04 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/24 15:20:54 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_simple(t_btree *tree)
{
	t_token *token;
	t_list	*list;

	if (!tree)
		return (0);
	list = tree->data;
	token = list->content;

	ft_printf("EXEC: ");
	write(1, token->value, token->len);
	ft_printf(" ");
	if (ft_strnstr(token->value, "true", 4))
		return (1);
	return (0);
}

int exec_pipe(t_btree *tree)
{
	__exec(tree->left);
	ft_printf("PIPED to ");
	return __exec(tree->right);
}

int exec_and_or(t_btree *tree)
{
	int first_res;

	if (!tree)
		return (0);
	first_res = __exec(tree->left);
	if (first_res)
	{
		if (tree->type == nt_and_if)
		{
			ft_printf(" AND ");
			return __exec(tree->right);
		}
		else
			return (0);
	}
	if (!first_res)
	{
		if (tree->type == nt_or_if)
		{
			ft_printf(" OR ");
			return (__exec(tree->right));
		}
		else
			return (0);
	}
	return (0);
}

int __exec(t_btree *tree)
{
	if (!tree)
		return (0);
	if (tree->type == nt_pipe)
		return exec_pipe(tree);
	else if (tree->type == nt_and_if)
		return exec_and_or(tree);
	else if (tree->type == nt_or_if)
		return exec_and_or(tree);
	else
		return exec_simple(tree);
	return (0);
}
