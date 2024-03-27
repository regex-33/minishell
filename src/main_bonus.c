/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/14 21:28:32 by bchanaa           #+#    #+#             */
/*   Updated: 2024/03/19 19:38:13 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex_bonus.h"

/*
 * Forks a new command child, redirects its stdout and stdin to
 * OUT_PIPE and IN_PIPE respectively.
*/

pid_t	exec_cmd(char *cmd, char **path_dirs, char **env, int pipes[2][2])
{
	char	**cmd_args;
	pid_t	pid;

	pid = fork();
	if (pid < 0)
		return (perror(PROG_NAME), -1);
	if (pid == 0)
	{
		close(pipes[OUT_PIPE][READ]);
		if (dup2(pipes[IN_PIPE][READ], STDIN_FILENO) < 0)
			return (close(pipes[OUT_PIPE][WRITE]), exit(1), 0);
		dup2(pipes[OUT_PIPE][WRITE], STDOUT_FILENO);
		close(pipes[IN_PIPE][READ]);
		close(pipes[OUT_PIPE][WRITE]);
		cmd_args = get_cmd_args(cmd, path_dirs);
		if (!cmd_args)
			return (0);
		if (execve(cmd_args[0], cmd_args, env))
			return (perror(PROG_NAME), 0);
	}
	return (close(pipes[IN_PIPE][READ]), close(pipes[OUT_PIPE][WRITE]), pid);
}

/*
 * Forks the last command child, and redirects its stdout to outfile.
*/

pid_t	exec_last_cmd(t_args *args, char *outfile, int fd[])
{
	pid_t	pid;
	char	**cmd_args;
	char	output_fd;

	pid = fork();
	if (pid < 0)
		return (perror(PROG_NAME), -1);
	if (pid == 0)
	{
		if (!args->is_heredoc)
			output_fd = open(outfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else
			output_fd = open(outfile, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (output_fd < 0)
			return (close(fd[READ]), perror(PROG_NAME), 0);
		cmd_args = get_cmd_args(args->av[args->ac - 2], args->path_dirs);
		if (!cmd_args)
			return (close(fd[READ]), 0);
		dup2(fd[READ], STDIN_FILENO);
		dup2(output_fd, STDOUT_FILENO);
		(close(output_fd), close(fd[READ]));
		if (execve(cmd_args[0], cmd_args, args->env))
			return (perror(PROG_NAME), 0);
	}
	return (close(fd[READ]), pid);
}

/*
 * Waits for all child processes.
 * Returns exit status of the last child process.
*/

int	wait_all(pid_t last_child_pid)
{
	int	status;

	if (last_child_pid < 0)
		return (perror(PROG_NAME), EXIT_FAILURE);
	waitpid(last_child_pid, &status, 0);
	while (wait(NULL) > 0)
		;
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	else if (WIFSIGNALED(status))
		return (128 + WTERMSIG(status));
	return (0);
}

int	main(int ac, char *av[], char **env)
{
	t_args	args;
	pid_t	last_child_pid;
	int		pipes[2][2];
	int		i;

	if (init_args(ac, av, env, &args) != 0)
		return (1);
	if (pipe(pipes[OUT_PIPE]) == -1)
		return (perror(PROG_NAME), EXIT_FAILURE);
	i = 1 + args.is_heredoc;
	if (handle_input(&args, pipes[OUT_PIPE]) < 0)
		return (0);
	while (++i < ac - 2)
	{
		if (add_pipe(pipes) == -1)
			return (ft_free_arr(args.path_dirs), \
					close(pipes[OUT_PIPE][READ]), perror(PROG_NAME), 1);
		if (exec_cmd(av[i], args.path_dirs, env, pipes) == 0)
			return (0);
	}
	last_child_pid = exec_last_cmd(&args, av[ac - 1], pipes[OUT_PIPE]);
	if (last_child_pid == 0)
		return (0);
	ft_free_arr(args.path_dirs);
	return (wait_all(last_child_pid));
}
