/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:21:51 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/20 21:21:29 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// parse -> PARSE_CMD(0)
// 
// PARSE_CMD(x) -> PAIR { OPERATOR PARSE_CMD(q) }
// PARSE_PAIR -> LPAR PARSE_CMD(0) RPAR | SIMPLE_COMMAND
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

	leaf = malloc(sizeof(t_btree));
	if (!leaf)
		return (NULL);
	leaf->data = data;
	leaf->type = type;
	leaf->left = NULL;
	leaf->right = NULL;
	return (leaf);
}

void	clear_btree(t_btree *tree, void (*del)(void *))
{
	t_btree	*left;
	t_btree	*right;

	if (!tree)
		return ;
	if (del)
		del(tree->data);
	right = tree->right;
	left = tree->left;
	free(tree);
	if (left)
		clear_btree(left, del);
	if (right)
		clear_btree(right, del);
}

void	print_node(t_btree *node, int depth, t_node_type parent_type)
{
	int	i;
	i = 0;
	char nt_strings[10][20] = 
	{
		"UNDEFINED",
		"CMD",
		"SIMPLE_CMD",
		"PIPE",
		"AND",
		"OR",
		"REDIR",
		"CMD_ARG"
	};
	while (i < depth)
	{
		ft_printf("     ");
		i++;
	}
	ft_printf("( %s [%d] {p: %s} )\n", nt_strings[node->type], depth, nt_strings[parent_type]);
}

void	print_tree(t_btree *tree, int depth, t_node_type parent_type)
{
	if (!tree)
		return ;
	print_node(tree, depth, parent_type);
	if (tree->left)
		print_tree(tree->left, depth + 1, tree->type);
	if (tree->right)
		print_tree(tree->right, depth + 1, tree->type);
}
