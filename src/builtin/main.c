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
		status = ft_change_dir(++args, ctx); // return status code
	else if (!ft_strcmp(args[0], "pwd"))
		status = ft_pwd(1, ctx);
	else if (!ft_strcmp(args[0], "exit"))
		status = ft_exit(args);
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
	//	size += ft_strlen(COLOR_KHDER_FATH);
	//	size += ft_strlen(ANSI_COLOR_RESET);
	if (suffix)
		size += ft_strlen(suffix);
	prompt = ft_calloc(size, sizeof(char));
	if (!prompt)
		return (NULL);
	//	ft_strlcat(prompt, COLOR_KHDER_FATH, size);
	ft_strlcat(prompt, str, size);
	//	ft_strlcat(prompt, ANSI_COLOR_RESET, size);
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

void	init_terminal(void)
{
	struct termios	old_term;
	struct termios	new_term;

	if (!isatty(STDIN_FILENO))
		return ;
	tcgetattr(STDIN_FILENO, &old_term);
	//ft_memcpy(&new_term, &old_term, sizeof(struct termios));
	new_term = old_term;
	new_term.c_lflag &= ~(ECHOCTL);
	tcsetattr(STDIN_FILENO, TCSANOW, &new_term);
}



int	main(void)
{
	char				*line;
	//char				*pwd;
	char				*prompt;
	t_list				*tokens;
	t_context			ctx;
	t_btree				*parse_tree;
	struct sigaction	saint;

	if (init_context(&ctx))
		return 1;
	//printf("path : %d\n", ctx.unset_path);
	saint.sa_handler = handle_interrupt;
	sigemptyset(&saint.sa_mask);
	sigaction(SIGINT, &saint, NULL);
	signal(SIGQUIT, handle_quit);
	get_state(ON_PROMPT, SET_STATE);
	//init_terminal();
	while (1)
	{
		get_state(ON_PROMPT, SET_STATE);
		prompt = get_prompt("minishell", " $ ");
		line = readline(prompt);
		get_state(ON_EXEC, SET_STATE);
		if (!line)
			exit(0);
		tokens = lexer(line);
		if (!tokens)
		{
			free(line);
			continue ;
		}
		add_history(line);
		parse_tree = parse(tokens);
		next_token(tokens, RESET_TOK);
		if (!parse_tree)
			continue;
		if (prompt_heredoc(parse_tree) < 0)
			continue;
		//ft_printf("----------- EXECUTION ---------\n");
 		get_status(__exec(parse_tree, &ctx), SET_STATUS);
		clear_btree(parse_tree, NULL);	
		ft_lstclear_libft(&tokens, free);
		free(prompt);
		free(line);
		//init_terminal();
	}
	return (0);
}
