/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:21:51 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/24 21:22:09 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// parse -> PARSE_CMD(0)
// 
// PARSE_CMD(x) -> PAIR { OPERATOR PARSE_CMD(q) }
// PARSE_PAIR -> SUBCMD | SIMPLE_COMMAND
// PARSE_SUBCMD -> LPAR CMD(0) RPAR {REDIR}
// SIMPLE_COMMAND -> {REDIR} LITERAL { LITERAL | REDIR }
// REDIR -> IO_REDIR LITERAL

#include "minishell.h"

t_btree	*new_node(t_node_type type, void *data, t_btree *left, t_btree *right)
{
	t_btree	*tree;

	tree = malloc(sizeof(t_btree));
	if (!tree)
		return (NULL);
	tree->data = data;
	tree->type = type;
	tree->left = left;
	tree->right = right;
	return (tree);
}

t_btree	*new_leaf(t_node_type type, void *data)
{
	t_btree	*leaf;

	if (!data)
		return (NULL);
	leaf = malloc(sizeof(t_btree));
	if (!leaf)
		return (NULL);
	leaf->data = data;
	leaf->type = type;
	leaf->left = NULL;
	leaf->right = NULL;
	return (leaf);
}

void	clear_btree(t_btree *tree)
{
	t_btree	*left;
	t_btree	*right;
	t_list	*redir_list;

	if (!tree)
		return ;
	if (tree->type == nt_subcmd)
	{
		redir_list = tree->data;
		ft_lstclear_libft(&redir_list, free_redir);
	}
	else if (tree->type == nt_simplecmd)
		free_cmd(tree->data);
	right = tree->right;
	left = tree->left;
	free(tree);
	if (left)
		clear_btree(left);
	if (right)
		clear_btree(right);
}
