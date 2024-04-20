/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   binary_tree.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:21:51 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/20 17:28:39 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
