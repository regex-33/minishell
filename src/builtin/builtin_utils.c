/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:28:35 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 13:28:37 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	get_status(int new_status, int flags)
{
	static int	status;

	if (flags == SET_STATUS)
		status = new_status;
	return (status);
}

int	is_builtin(char *cmd_name)
{
	if (!ft_strcmp(cmd_name, "cd"))
		return (1);
	else if (!ft_strcmp(cmd_name, "pwd"))
		return (1);
	else if (!ft_strcmp(cmd_name, "exit"))
		return (1);
	else if (!ft_strcmp(cmd_name, "echo"))
		return (1);
	else if (!ft_strcmp(cmd_name, "env"))
		return (1);
	else if (!ft_strcmp(cmd_name, "export"))
		return (1);
	else if (!ft_strcmp(cmd_name, "unset"))
		return (1);
	return (0);
}

char	**grep_paths(t_context *ctx)
{
	char	*path_env;
	char	**path_dirs;

	path_env = get_value("PATH", ctx->env);
	if (!path_env)
	{
		if (ctx->unset_path)
		{
			path_env = ft_strdup(TEMP_PATH + 5);
			if (!path_env)
				return (perror(COMMAND_NOT_FOUND), NULL);
		}
		else
		{
			path_env = ft_strdup(".");
			if (!path_env)
				return (perror(COMMAND_NOT_FOUND), NULL);
		}
	}
	path_dirs = ft_split(path_env, ':');
	if (!path_dirs)
		return (free(path_env), perror(COMMAND_NOT_FOUND), NULL);
	return (free(path_env), path_dirs);
}

int	parse_existing_variable(char *str, int flag)
{
	int	add_to_value;

	add_to_value = 0;
	if (!str)
		return (perror("minishell"), -1);
	if (!is_valid_identifier(str))
	{
		if (flag)
		{
			ft_putstr_fd("minishell: export: ", 2);
			ft_putstr_fd(str, 2);
			ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO);
		}
		return (-1);
	}
	if (ft_strchr(str, '+') != NULL)
		add_to_value = 1;
	return (add_to_value);
}

int	init_context(t_context *ctx)
{
	ctx->parse_tree = NULL;
	ctx->tokens = NULL;
	ctx->env = NULL;
	ctx->unset_path = 1;
	ctx->env = ft_creat_env(ctx);
	if (ctx->env == NULL)
		return (1);
	ctx->last_pwd = get_value("PWD", ctx->env);
	if (!ctx->last_pwd)
		ctx->last_pwd = ft_strdup("");
	return (0);
}
