/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd_and_env.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:30:41 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 13:30:42 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* export etc funtion i dno't have space */
int	update_existing_variable(char **variable, char ***env_ptr, int *env_count,
		int add_to_value)
{
	char	**env;
	char	*str;
	int		count;
	int		result;

	env = *env_ptr;
	str = variable[*env_count];
	count = 0;
	add_to_value = parse_existing_variable(str, 1);
	if (add_to_value == -1)
		return (-1);
	if (add_to_value == 1)
		result = handle_plus(env, str, (ft_strchr(str, '+') - str), &count);
	else
		result = handle_equal(env, str, (ft_strchr(str, '=') - str), &count);
	if (result == -1)
		return (perror("minishell"), -1);
	else if (result == 1)
	{
		*env_ptr = env;
		*env_count = count;
		return (1);
	}
	*env_count = count;
	return (0);
}

int	ft_handle_no_variables(char ***env_ptr, int fd, int unset_path)
{
	if (!ft_sort_export_cmd(*env_ptr, fd, unset_path))
		return (1);
	return (0);
}

int	ft_pwd(int fd, t_context *ctx)
{
	char	cwd[1024];
	char	*temp;
	char	*path;

	path = NULL;
	path = getcwd(cwd, sizeof(cwd));
	if (path)
		return (ft_putstr_fd(cwd, fd), ft_putchar_fd('\n', fd), 0);
	else
	{
		if (ctx->last_pwd)
			return (ft_putstr_fd(ctx->last_pwd, fd), ft_putchar_fd('\n', fd),
				0);
		else
		{
			temp = get_value("PWD", ctx->env);
			if (temp)
				return (ft_putstr_fd(temp, fd), ft_putchar_fd('\n', fd), 0);
			else
				return (ft_putstr_fd(PWD_ERROR, fd), 1);
		}
	}
	return (0);
}

int	ft_env(t_context *ctx, int fd)
{
	char	**env_ptr;

	env_ptr = ctx->env;
	if (!env_ptr)
		return (printf("minishell"), 1);
	while (*env_ptr != NULL)
	{
		if (ft_strchr(*env_ptr, '='))
		{
			if (!(ctx->unset_path && !ft_strncmp(*env_ptr, "PATH=", 5)))
			{
				ft_putstr_fd(*env_ptr, fd);
				ft_putchar_fd('\n', fd);
			}
		}
		env_ptr++;
	}
	return (0);
}
