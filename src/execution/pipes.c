/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 09:19:30 by bchanaa           #+#    #+#             */
/*   Updated: 2024/07/21 14:47:34 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	exec_pipe(t_btree *tree, t_context *ctx, int pipes[2][2], int is_root)
{
	if (!tree)
		return (0);
	if (tree->left->type != nt_pipe)
	{
		add_pipe(pipes);
		exec_piped_cmd(tree->left, ctx, pipes);
	}
	else
		exec_pipe(tree->left, ctx, pipes, 0);
	if (!is_root)
	{
		add_pipe(pipes);
		return (exec_piped_cmd(tree->right, ctx, pipes));
	}
	else
		return (exec_last_piped_cmd(tree->right, ctx, pipes[OUT_PIPE]));
}

int	exec_sub(t_btree *tree, t_context *ctx)
{
	t_list	*redir_list;
	pid_t	pid;
	int		status;

	redir_list = tree->data;
	pid = fork();
	if (pid < 0)
		return (perror("minishell"), 1);
	if (pid == 0)
	{
		if (redirect(redir_list, ctx))
			return (exit(1), 0);
		exit(__exec(tree->left, ctx));
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			return (WEXITSTATUS(status));
		else if (WIFSIGNALED(status))
			return (128 + WTERMSIG(status));
		return (1);
	}
}

pid_t	run_command(t_btree *tree, t_context *ctx)
{
	int			status;
	t_prexec	pexec;
	t_cmd		*cmd;

	cmd = tree->data;
	if (!cmd->cmd_args)
		return (exit(0), 0);
	pexec.args = get_expanded_args(cmd, ctx);
	if (!pexec.args)
		return (perror("minishell"), exit(1), 0);
	status = select_buildin_commands(pexec.args, cmd->redir_list, ctx);
	if (status != -1)
		return (exit(status), 0);
	if (redirect(cmd->redir_list, ctx))
		return (exit(1), 0);
	reset_redir(cmd->redir_list, 0);
	if (init_command(&pexec, ctx, pexec.args))
		return (exit(pexec.err), 0);
	if (execve(pexec.cmd_name, pexec.args, ctx->env))
		return (perror("minishell"), exit(1), 0);
	return (0);
}

pid_t	exec_piped_cmd(t_btree *tree, t_context *ctx, int pipes[2][2])
{
	pid_t		pid;

	pid = fork();
	if (pid < 0)
		return (perror("minishell"), exit(1), -1);
	if (pid != 0 && pipes[IN_PIPE][READ] != STDIN_FILENO)
		close(pipes[IN_PIPE][READ]);
	if (pid != 0)
		return (close(pipes[OUT_PIPE][WRITE]), pid);
	close(pipes[OUT_PIPE][READ]);
	if (dup2(pipes[IN_PIPE][READ], STDIN_FILENO) < 0)
		return (close(pipes[OUT_PIPE][WRITE]), exit(1), 0);
	dup2(pipes[OUT_PIPE][WRITE], STDOUT_FILENO);
	close(pipes[OUT_PIPE][WRITE]);
	if (pipes[IN_PIPE][READ] != STDIN_FILENO)
		close(pipes[IN_PIPE][READ]);
	if (tree->type == nt_subcmd)
	{
		if (redirect(tree->data, ctx))
			return (exit(1), 0);
		reset_redir(tree->data, 0);
		exit(__exec(tree->left, ctx));
	}
	return (run_command(tree, ctx));
}

pid_t	exec_last_piped_cmd(t_btree *tree, t_context *ctx, int fd[2])
{
	pid_t		pid;

	pid = fork();
	if (pid < 0)
		return (perror("minishell"), exit(1), -1);
	if (pid == 0)
	{
		if (dup2(fd[READ], STDIN_FILENO))
			return (close(fd[READ]), exit(1), 0);
		close(fd[READ]);
		if (tree->type == nt_subcmd)
		{
			if (redirect(tree->data, ctx))
				return (exit(1), 0);
			reset_redir(tree->data, 0);
			exit(__exec(tree->left, ctx));
		}
		return (run_command(tree, ctx));
	}
	return (close(fd[READ]), pid);
}
