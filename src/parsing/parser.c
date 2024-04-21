/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:29:47 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/20 19:37:20 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "minishell.h" 
// CMD -> PAIR { AND_OR PAIR }
// PAIR -> PIPELINE | LPAR CMD RPAR
// PIPELINE -> SIMPLE_COMMAND { PIPE SIMPLE_COMMAND }
// SIMPLE_COMMAND -> {REDIR} LITERAL {REDIR|LITERAL}
// REDIR-> IO_REDIR LITERAL
// AND_OR -> ‘&&’ | ‘||’

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
		is_init = 0; current = NULL; return (NULL);
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

// t_tree	*parse_cmd(t_list *tokens)
// {
// 	t_tree	*root;
// 
// 	root = new_tree(nt_cmd, NULL, NULL, NULL);
// 	if (expect(tok_l_par, tokens) || 
// 		root->child = parse_pair(tokens);
// 
// }

void	Simple_Command(t_list *tokens)
{
	t_token *token;
	int		is_redir;
	token = next_token(tokens, 0);
	is_redir = 0;
	int		literalCount;

	literalCount = 0;
	while (token && (token->type == tok_literal || token->type == tok_redir))
	{
		if (token->type == tok_literal)
		{
			if (is_redir)
				is_redir = 0;
			literalCount++;
		}
		else if (token->type == tok_redir)
		{
			if (is_redir)
				panic("Redir after redir");
			else
				is_redir = 1;
		}
		token = next_token(tokens, CONSUME_TOK); // consume
	}
	if (literalCount == 0 || is_redir)
		panic("No literals found\n");
}

void	Pipeline(t_list *tokens)
{
	t_token	*token;

	Simple_Command(tokens);
	token = next_token(tokens, 0);
	while (token && token->type == tok_pipe)
	{
		next_token(tokens, CONSUME_TOK);
		Simple_Command(tokens);
		token = next_token(tokens, 0);
	}
}

void	P(t_list *tokens)
{
	t_token	*token;

	token = next_token(tokens, 0);
	if (token && (token->type == tok_l_par))
	{
		next_token(tokens, 1); // consume
		E(tokens);
		expect(tok_r_par, tokens);
	}
	else
		Pipeline(tokens);
}

void	E(t_list *tokens)
{
	t_token	*current_token;

	P(tokens);
	current_token = next_token(tokens, 0);
	while (current_token && (current_token->type == tok_op_or || current_token->type == tok_op_and))
	{
		current_token = next_token(tokens, CONSUME_TOK); // CONSUME
		P(tokens);
		current_token = next_token(tokens, 0);
	}
}

void	parse_tokens(t_list *token_list)
{
	E(token_list);
	if (next_token(token_list, 0))
		panic("error at last");
	else
		ft_printf("all is good!\n");
}

int main(int ac, char *av[])
{
	t_list	*tokens;
 	char *line;
 	line = get_next_line(0);
 	while (line)
 	{
 		line[ft_strlen(line) - 1] = 0;
 		ft_printf("---------------- PARSE --------------\n");
 		tokens = parse_line(line);
		parse_tokens(tokens);
		ft_lstclear_libft(&tokens, free);
 		free(line);
		next_token(tokens, RESET_TOK);
 		line = get_next_line(0);
 	}
 	return (0);
}
