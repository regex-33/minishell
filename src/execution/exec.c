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

int	expand_list(t_list *list, char **env)
{
	t_token	*token;
	char	*value;
	t_list	*expanding_list;

	(void)value;
	(void)expanding_list;
	(void)env;
	if (!list)
		return (0);
	token = list->content;
	write(1, token->value, token->len);
	// while (list)
	// {
	// 	//printf("i am here\n");
	// 	token = list->content;
	// 	write(1, token->value, token->len);
	// 	ft_printf("\n");
	// 	list = list->next;
	// }
	
	return (1);
}

int	exec_simple(t_btree *tree, char **env)
{
	t_token *token = NULL;
	t_list	*list;

	(void) env;
	if (!tree)
		return (0);
	t_cmd *cmd = tree->data;
	list = cmd->cmd_args;
	t_list *redir_list = cmd->redir_list;
	(void)redir_list;

	ft_printf("EXEC: ");
	//expand_list(list, env);
	while (list)
	{
		token = list->content;
		write(1, token->value, token->len);
		ft_printf("\n");
		list = list->next;
	}
	// write(1, token->value, token->len);
	// ft_printf(" ");
	if (ft_strnstr(token->value, "true", 4))
		return (1);
	return (0);
}

int exec_pipe(t_btree *tree, char **env)
{
	__exec(tree->left, env);
	ft_printf("PIPED to ");
	return __exec(tree->right, env);
}

int exec_and_or(t_btree *tree, char **env)
{
	int first_res;

	if (!tree)
		return (0);
	first_res = __exec(tree->left, env);
	if (first_res)
	{
		if (tree->type == nt_and_if)
		{
			ft_printf(" AND ");
			return __exec(tree->right, env);
		}
		else
			return (0);
	}
	if (!first_res)
	{
		if (tree->type == nt_or_if)
		{
			ft_printf(" OR ");
			return (__exec(tree->right, env));
		}
		else
			return (0);
	}
	return (0);
}

int	exec_sub(t_btree *tree, char **env)
{
	(void)env;
	if (!tree)
		return (1);
	return (__exec(tree->left, env));
}

int __exec(t_btree *tree, char **env)
{
	if (!tree)
		return (0);
	if (tree->type == nt_pipe)
		return exec_pipe(tree, env);
	else if (tree->type == nt_and_if)
		return exec_and_or(tree, env);
	else if (tree->type == nt_or_if)
		return exec_and_or(tree, env);
	else if (tree->type == nt_subcmd)
		return exec_sub(tree, env);
	else
		return exec_simple(tree, env);
	return (0);
}
