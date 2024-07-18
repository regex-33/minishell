/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_command.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 18:20:02 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 18:20:03 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
int	init_command(t_prexec *pexec, t_context *ctx, char **args)
{
	char	**path_dirs;

	pexec->err = 0;
	pexec->cmd_name = NULL;
	if (!*args)
		return (1);
	path_dirs = grep_paths(ctx);
	if (ft_strchr(args[0], '/'))
	{
		pexec->err = check_abs_cmd(args[0]);
		if (pexec->err)
			return (free_array(path_dirs), pexec->err);
		pexec->cmd_name = ft_strdup(args[0]);
	}
	else
	{
		pexec->cmd_name = ft_which(args[0], path_dirs);
		if (!pexec->cmd_name)
		{
			pexec->err = 127;
			print_err(args[0], " : command not found");
		}
		else if (access(pexec->cmd_name, X_OK))
		{
			pexec->err = 126;
			print_err(pexec->cmd_name, " : Permission denied.");
		}
	}
	pexec->args = args;
	return (free_array(path_dirs), pexec->err);
}
*/

int	check_absolute_command(t_prexec *pexec, char **args)
{
	pexec->err = check_abs_cmd(args[0]);
	if (pexec->err)
		return (pexec->err);
	pexec->cmd_name = ft_strdup(args[0]);
	return (0);
}

int	find_command(t_prexec *pexec, char **args, char **path_dirs)
{
	pexec->cmd_name = ft_which(args[0], path_dirs);
	if (!pexec->cmd_name)
	{
		pexec->err = 127;
		print_err(args[0], " : command not found");
		return (pexec->err);
	}
	else if (access(pexec->cmd_name, X_OK))
	{
		pexec->err = 126;
		print_err(pexec->cmd_name, " : Permission denied.");
		return (pexec->err);
	}
	return (0);
}

int	init_command_paths(t_prexec *pexec, t_context *ctx, char **args)
{
	char	**path_dirs;
	int		err;

	path_dirs = grep_paths(ctx);
	err = 0;
	if (ft_strchr(args[0], '/'))
		err = check_absolute_command(pexec, args);
	else
		err = find_command(pexec, args, path_dirs);
	return (free_array(path_dirs), err);
}

int	init_command(t_prexec *pexec, t_context *ctx, char **args)
{
	pexec->err = 0;
	pexec->cmd_name = NULL;
	if (!*args)
		return (1);
	pexec->err = init_command_paths(pexec, ctx, args);
	pexec->args = args;
	return (pexec->err);
}
