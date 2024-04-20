/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 15:48:45 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/20 17:17:46 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_token_length(char *line, t_token *token)
{
	if (token->type == tok_redir)
	{
		token->len = match_pattern(line, O_DIGITS, "<>") + 1;
		ft_printf("match pattern: %d\n", token->len);
		if (line[token->len - 1] == line[token->len])
			token->len++;
	}
	else if (token->type == tok_op_or || token->type == tok_op_and)
		return (2);
	else if (token->type == tok_literal)
		token->len = wordlen(line);
	else
		return (1);
	return (token->len);
}

t_token	*gettoken(char *line, int *index)
{
	int		i;
	t_token	*token;

	i = 0;
	token = malloc(sizeof(t_token));
	token->len = 0;
	if (!token)
		return (NULL);
	if (line[i] == '(')
		token->type = tok_l_par;
	else if (line[i] == ')')
		token->type = tok_r_par;
	else if (line[i] == '|')
		token->type = tok_pipe + (line[i] == line[i + 1]);
	else if (line[i] == '&' && line[i] == line[i + 1])
		token->type = tok_op_and;
	else if (ft_isdigit(line[i]) && (line[i + 1] && ft_strchr("<>", line[i + 1])))
		token->type = tok_redir;
	else if (line[i] == '>' || line[i] == '<')
		token->type = tok_redir;
	else
		token->type = tok_literal;
	token->value = line;
	token->len = get_token_length(line, token);
	ft_printf("token len %d type: %d start: %s-- \n", token->len, token->type, token->value);
	if (token->len < 1)
		return (free(token), NULL);
	*index += token->len;
	return (token);
}

t_list	*lexer(char *line)
{
	t_token	*token;
	t_list	*node;
	t_list	*tokens;
	int		i;

	tokens = NULL;
	i = 0;
	while (line[i])
	{
		if (line[i] == 32 || line[i] == '\t')
		{
			i++;
			continue ;
		}
		token = gettoken(line + i, &i);
		if (!token)
			return (NULL); // handle this
		node = ft_lstnew(token);
		if (!node)
			return (NULL); // malloc error
		ft_lstadd_back_libft(&tokens, node);
	}
	return (tokens);
}


int main(void)
{
	t_list	*tokens;

	char *line = get_next_line(0);
	while (line)
	{
		line[ft_strlen(line) - 1] = 0;
		tokens = lexer(line);
		print_token_list(tokens);
		ft_lstclear_libft(&tokens, free);
		free(line);
		line = get_next_line(0);
	}
	return (0);
}
