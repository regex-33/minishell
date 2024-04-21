/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   real_parser.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 17:20:39 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/21 12:19:23 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	panic(char *str)
{
	ft_putendl_fd(str, 2);
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
	return (token->type == tok_pipe || token->type == tok_op_or || token->type == tok_op_and);
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

t_node_type	get_nt(t_token *token)
{
	if (!token)
		return (nt_undefined);
	if (token->type == tok_redir)
		return (nt_io_redir);
	if (token->type == tok_pipe)
		return (nt_pipe);
	if (token->type == tok_literal)
		return (nt_simplecmd);
	if (token->type == tok_op_or)
		return (nt_or_if);
	if (token->type == tok_op_and)
		return (nt_and_if);
	return (nt_undefined);
}

t_btree	*parse_simplecmd(t_list *tokens)
{
	t_list	*args;
	t_btree	*simplecmd_root;
	t_token	*token;
	bool	is_redir;
	int		literals;
	t_list	*node;

	literals = 0;
	is_redir = 0;
	token = next_token(tokens, 0);
	args = NULL;
	while (token && (token->type == tok_literal || token->type == tok_redir))
	{
		if (token->type == tok_literal)
		{
			if (is_redir)
				is_redir = 0;
			else
				literals++;
		}
		else
		{
			if (is_redir)	
				return (ft_lstclear_libft(&args, free), panic("Expected literal, found redirection."), NULL);
			else
				is_redir = 1;
		}
		node = ft_lstnew(token);
		if (!node)
			return (ft_lstclear_libft(&args, free), panic("malloc error"), NULL);
		ft_lstadd_back_libft(&args, node);
		token = next_token(tokens, CONSUME_TOK);
	}
	if (is_redir)
		return (ft_lstclear_libft(&args, free), panic("Expected literal after redirection."), NULL);
	if (!literals)
		return (ft_lstclear_libft(&args, free), panic("Expected at least 1 literal"), NULL);
	simplecmd_root = new_leaf(nt_simplecmd, args);
	if (!simplecmd_root)
		return (ft_lstclear_libft(&args, free), panic("malloc error"), NULL);
	return (simplecmd_root);
}

t_btree	*parse_pair(t_list *tokens)
{
	t_btree	*pair_root;
	t_token	*token;

	token = next_token(tokens, 0);
	pair_root = NULL;
	if (!token)
		return (panic("Token Expected at parse pair"), NULL);
	if (token->type == tok_l_par)
	{
		next_token(tokens, CONSUME_TOK);
		pair_root = parse_cmd(tokens, 0);
		if (!expect(tok_r_par, tokens))
			return (clear_btree(pair_root, free), panic("Expected token not found."), NULL);
		return (pair_root);
	}
	else if (token->type == tok_redir || token->type == tok_literal)
	{
		pair_root = parse_simplecmd(tokens);
		return (pair_root);
	}
	else
		return (clear_btree(pair_root, free), panic("Parse Pair Error"), NULL);
	return (pair_root);
}

t_btree	*parse_cmd(t_list *tokens, int prec)
{
	t_btree	*cmd_root;
	t_btree	*tmp;
	t_token	*token;

	cmd_root = parse_pair(tokens);
	token = next_token(tokens, 0);
	while (is_operator(token) && get_prec(token) >= prec)
	{
		next_token(tokens, CONSUME_TOK);
		tmp = cmd_root;
		cmd_root = new_node(get_nt(token), token, tmp, parse_cmd(tokens, get_prec(token) + 1));
		token = next_token(tokens, 0);
	}
	return (cmd_root);
}

t_btree	*parse(t_list *tokens)
{
	t_btree	*parse_tree;

	parse_tree = parse_cmd(tokens, 0);
	if (next_token(tokens, 0))
		return (clear_btree(parse_tree, free), panic("Token not expected."), NULL);
	else
		ft_printf("PARSING COMPLETE!\n");
	return (parse_tree);
}

// int main(void)
// {
// 	t_list	*tokens; 
// 	t_btree	*parse_tree;
// 	char *line = get_next_line(0);
// 	while (line)
// 	{
// 		line[ft_strlen(line) - 1] = 0;
// 		tokens = lexer(line);
// 		print_token_list(tokens);
// 		parse_tree = parse(tokens);
// 		ft_printf("--------- TREE -------\n");
// 		print_tree(parse_tree, 0, nt_undefined);
// 		ft_printf("--------- COMMAND ----------\n");
// 		ft_printf("%s\n", line);
// 		ft_printf("----------- EXECUTION ---------\n");
// 		__exec(parse_tree);
// 		ft_printf("\n");
// 		ft_lstclear_libft(&tokens, free);
// 		next_token(tokens, RESET_TOK);
// 		free(line);
// 		line = get_next_line(0);
// 	}
// 	return (0);
// }
