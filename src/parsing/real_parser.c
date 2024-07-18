/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   real_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:20:39 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/24 21:02:53 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_btree	*parse_simplecmd(t_list *tokens)
{
	t_btree	*simplecmd_root;
	t_cmd	*cmd;
	t_token	*token;

	cmd = new_cmd(NULL, NULL);
	if (!cmd)
		return (perror("minishell"), NULL);
	token = next_token(tokens, 0);
	while (token && (token->type == tok_redir || token->type == tok_literal))
	{
		if (token->type == tok_redir && \
			parse_redir(tokens, &cmd->redir_list) != 0)
			return (free_cmd(cmd), NULL);
		else if (token->type == tok_literal)
		{
			next_token(tokens, CONSUME_TOK);
			if (!append_token(&cmd->cmd_args, token))
				return (perror("minishell"), free_cmd(cmd), NULL);
		}
		token = next_token(tokens, 0);
	}
	simplecmd_root = new_leaf(nt_simplecmd, cmd);
	if (!simplecmd_root)
		return (perror("minishell"), free_cmd(cmd), NULL);
	return (simplecmd_root);
}

t_btree	*parse_subcmd(t_list *tokens)
{
	t_btree	*subcmd_root;
	t_token	*token;
	t_list	*redir_list;

	redir_list = NULL;
	next_token(tokens, CONSUME_TOK);
	subcmd_root = new_node(nt_subcmd, NULL, NULL, NULL);
	if (!subcmd_root)
		return (perror("minishell"), NULL);
	subcmd_root->left = parse_cmd(tokens, 0);
	if (!subcmd_root->left)
		return (clear_btree(subcmd_root), NULL);
	if (!expect(tok_r_par, tokens))
		return (clear_btree(subcmd_root),
			panic("minishell", PERR_EXP_TOK, 0), NULL);
	token = next_token(tokens, 0);
	while (token && token->type == tok_redir)
	{
		if (parse_redir(tokens, &redir_list) != 0)
			return (clear_btree(subcmd_root), NULL);
		token = next_token(tokens, 0);
	}
	subcmd_root->data = redir_list;
	return (subcmd_root);
}

t_btree	*parse_pair(t_list *tokens)
{
	t_btree	*pair_root;
	t_token	*token;

	token = next_token(tokens, 0);
	pair_root = NULL;
	if (!token)
		return (panic("minishell", PERR_EXP_TOK, 0), NULL);
	if (token->type == tok_l_par)
	{
		pair_root = parse_subcmd(tokens);
		return (pair_root);
	}
	else if (token->type == tok_redir || token->type == tok_literal)
	{
		pair_root = parse_simplecmd(tokens);
		return (pair_root);
	}
	else
		return (clear_btree(pair_root), panic("minishell", \
				PERR_UNEXP_TOK, 0), NULL);
	return (pair_root);
}

t_btree	*parse_cmd(t_list *tokens, int prec)
{
	t_btree	*cmd_root;
	t_btree	*tmp;
	t_token	*token;
	t_btree	*child;

	cmd_root = parse_pair(tokens);
	if (!cmd_root)
		return (NULL);
	token = next_token(tokens, 0);
	while (is_operator(token) && get_prec(token) >= prec)
	{
		next_token(tokens, CONSUME_TOK);
		tmp = cmd_root;
		child = parse_cmd(tokens, get_prec(token) + 1);
		if (!child)
			return (clear_btree(cmd_root), NULL);
		cmd_root = new_node(get_nt(token), token, tmp, child);
		if (!cmd_root)
			return (clear_btree(child), perror("minishell"), NULL);
		token = next_token(tokens, 0);
	}
	return (cmd_root);
}

t_btree	*parse(t_list *tokens)
{
	t_btree	*parse_tree;

	parse_tree = parse_cmd(tokens, 0);
	if (!parse_tree)
		return (NULL);
	else if (next_token(tokens, 0))
	{
		clear_btree(parse_tree);
		return (panic("minishell", PERR_UNEXP_TOK, 0), NULL);
	}
	return (parse_tree);
}
