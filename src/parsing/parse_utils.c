/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 19:38:51 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/24 17:27:25 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	parse_redir(t_list *tokens, t_list **redir_list)
{
	t_redir	*redir;
	t_token	*token;
	t_token	*file_token;
	t_list	*node;

	token = next_token(tokens, 0);
	file_token = next_token(tokens, CONSUME_TOK);
	if (!file_token || file_token->type != tok_literal)
		return (ft_lstclear_libft(redir_list, free_redir), \
				panic("minishell", PERR_EXP_TOK, 0), 1);
	redir = new_redir(token, file_token);
	if (!redir)
		return (ft_lstclear_libft(redir_list, free_redir), \
				perror("minishell"), 1);
	node = ft_lstnew(redir);
	if (!node)
		return (ft_lstclear_libft(redir_list, free_redir), \
				perror("minishell"), 1);
	ft_lstadd_back_libft(redir_list, node);
	next_token(tokens, CONSUME_TOK);
	return (0);
}

int	append_token(t_list **lst, t_token *token)
{
	t_list	*node;

	node = ft_lstnew(token);
	if (!node)
		return (0);
	ft_lstadd_back_libft(lst, node);
	return (1);
}

t_cmd	*new_cmd(t_list *cmd_args, t_list *redir_list)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->cmd_args = cmd_args;
	cmd->redir_list = redir_list;
	return (cmd);
}

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->cmd_args)
		ft_lstclear_libft(&cmd->cmd_args, NULL);
	if (cmd->redir_list)
		ft_lstclear_libft(&cmd->redir_list, free_redir);
	free(cmd);
}
