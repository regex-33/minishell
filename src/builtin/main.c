/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:29:51 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 13:29:52 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	select_buildin_commands(char **args, t_list *redir_list, t_context *ctx)
{
	int	status;

	status = 1;
	if (!*args || !is_builtin(args[0]))
		return (-1);
	if (redirect(redir_list, ctx))
		return (1);
	if (!ft_strcmp(args[0], "cd"))
		status = ft_change_dir(++args, ctx);
	else if (!ft_strcmp(args[0], "pwd"))
		status = ft_pwd(1, ctx);
	else if (!ft_strcmp(args[0], "exit"))
		status = ft_exit(args, ctx);
	else if (!ft_strcmp(args[0], "echo"))
		status = ft_echo(args, 1);
	else if (!ft_strcmp(args[0], "env"))
		status = ft_env(ctx, 1);
	else if (!ft_strcmp(args[0], "export"))
		status = ft_export(args, &ctx->env, 1, &ctx->unset_path);
	else if (!ft_strcmp(args[0], "unset"))
		status = ft_unset(args, &ctx->env, &ctx->unset_path);
	reset_redir(redir_list, 1);
	return (status);
}

char	*get_prompt(char *str, char *suffix)
{
	char	*prompt;
	size_t	size;

	if (!str)
		return (NULL);
	size = ft_strlen(str) + 1;
	if (suffix)
		size += ft_strlen(suffix);
	prompt = ft_calloc(size, sizeof(char));
	if (!prompt)
		return (NULL);
	ft_strlcat(prompt, str, size);
	if (suffix)
		ft_strlcat(prompt, suffix, size);
	return (prompt);
}

int	get_state(int new_state, int flags)
{
	static int	state;

	if (flags == SET_STATE)
		state = new_state;
	return (state);
}

int	minishell(t_context *ctx, char *line)
{
	get_state(ON_EXEC, SET_STATE);
	if (!line)
		exit(get_status(0, 0));
	ctx->tokens = lexer(line);
	if (!ctx->tokens)
		return (free(line), 0);
	add_history(line);
	ctx->parse_tree = parse(ctx->tokens);
	next_token(ctx->tokens, RESET_TOK);
	if (!ctx->parse_tree)
		return (get_status(2, SET_STATUS), \
				ft_lstclear_libft(&ctx->tokens, free), 0);
	if (prompt_heredoc(ctx->parse_tree) < 0)
		return (clear_btree(ctx->parse_tree), \
				ft_lstclear_libft(&ctx->tokens, free), 0);
	get_status(__exec(ctx->parse_tree, ctx), SET_STATUS);
	ft_lstclear_libft(&ctx->tokens, free);
	return (clear_btree(ctx->parse_tree), free(line), get_status(0, 0));
}

int	main(void)
{
	t_context	ctx;
	char		*tmp;
	char 		*line;

	tmp = NULL;
	line = NULL;
	signal(SIGINT, handle_interrupt);
	signal(SIGQUIT, handle_quit);
	if (init_context(&ctx))
		return (1);
	while (1)
	{
		get_state(ON_PROMPT, SET_STATE);
		if (!isatty(STDIN_FILENO))
		{
			tmp = get_next_line(STDIN_FILENO);
			line = ft_strtrim(tmp, "\n");
			free(tmp);
		}
		else
			line = readline("minishell $ ");
		minishell(&ctx, line);
	}
	return (get_status(0, 0));
}
