/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execut_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:04:38 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 14:04:39 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_which(char *cmd, char **path_dirs)
{
	char	*tmp;
	char	*cmd_pathname;
	int		i;

	if (!path_dirs)
		return (NULL);
	tmp = ft_strjoin("/", cmd);
	if (!tmp)
		return (NULL);
	i = -1;
	while (path_dirs[++i])
	{
		cmd_pathname = ft_strjoin(path_dirs[i], tmp);
		if (!cmd_pathname)
			return (free(tmp), NULL);
		if (is_file(cmd_pathname))
			return (free(tmp), cmd_pathname);
		free(cmd_pathname);
	}
	return (free(tmp), NULL);
}

char	**get_cmd_args(char **cmd_args, char **path_dirs)
{
	char	*cmd_pathname;

	if (!cmd_args || !path_dirs)
		return (NULL);
	cmd_pathname = ft_which(cmd_args[0], path_dirs);
	if (!cmd_pathname)
	{
		if (!ft_strchr(cmd_args[0], '/'))
		{
			ft_putstr_fd(CMD_NOT_FOUND, STDERR_FILENO);
			ft_putendl_fd(cmd_args[0], STDERR_FILENO);
			return (ft_free_arr(cmd_args), NULL);
		}
		return (ft_free_arr(cmd_args), perror("minishell"), NULL);
	}
	free(cmd_args[0]);
	cmd_args[0] = cmd_pathname;
	return (cmd_args);
}

int	handle_child_process(t_list *redir_list, t_context *ctx, char **args)
{
	t_prexec	pexec;

	if (redirect(redir_list, ctx))
		return (exit(EXIT_FAILURE), 0);
	reset_redir(redir_list, 0);
	if (init_command(&pexec, ctx, args))
		return (exit(pexec.err), 0);
	if (execve(pexec.cmd_name, pexec.args, ctx->env))
		return (perror("minishell"), exit(1), 0);
	return (0);
}

int	exec_cmd(t_list *redir_list, char **args, t_context *ctx)
{
	pid_t		pid;
	int			status;

	status = select_buildin_commands(args, redir_list, ctx);
	if (status != -1)
		return (status);
	pid = fork();
	if (pid < 0)
		return (perror("minishell"), 0);
	if (pid == 0)
		handle_child_process(redir_list, ctx, args);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
			status = WTERMSIG(status) + 128;
		return (status);
	}
	return (1);
}
