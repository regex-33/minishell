/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:30:26 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 13:30:27 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_plus(char **env, char *str, int name_len, int *env_count_local)
{
	while (env[*env_count_local] != NULL)
	{
		if (!ft_strncmp(env[*env_count_local], str, name_len))
		{
			if (!ft_strchr(env[*env_count_local], '='))
			{
				env[*env_count_local] = ft_strjoin_free(env[*env_count_local],
						"=");
				if (!env[*env_count_local])
					return (-1);
			}
			env[*env_count_local] = ft_strjoin_free(env[*env_count_local],
					&str[name_len + 2]);
			return (1);
		}
		*env_count_local += 1;
	}
	return (0);
}

int	handle_equal(char **env, char *str, int to_equal, int *env_count_local)
{
	int	len;

	if (to_equal < 0)
		to_equal = ft_strlen(str);
	while (env[*env_count_local] != NULL)
	{
		len = ft_strchr(env[*env_count_local], '=') - env[*env_count_local];
		if (len < 0)
			len = ft_strlen(env[*env_count_local]);
		if ((ft_strchr(str, '=') && len == to_equal)
			&& !ft_strncmp(env[*env_count_local], str, to_equal))
		{
			free(env[*env_count_local]);
			env[*env_count_local] = ft_strdup(str);
			if (!env[*env_count_local])
				return (-1);
			return (1);
		}
		else if (!ft_strncmp(env[*env_count_local], str, to_equal)
			&& !ft_strchr(str, '=') && (ft_strchr(env[*env_count_local], '=')
				|| !ft_strchr(env[*env_count_local], '=')))
			return (1);
		*env_count_local += 1;
	}
	return (0);
}

int	add_new_variable(char **variable, char ***env_ptr, int env_count, int index)
{
	char	**env;
	char	**new_environ;
	int		i;

	env = *env_ptr;
	i = 0;
	new_environ = malloc((env_count + 2) * sizeof(char *));
	if (!new_environ)
		return (perror("minishell"), -1);
	while (i < env_count)
	{
		new_environ[i] = ft_strdup(env[i]);
		if (!new_environ[i])
		{
			while (i > 0)
				free(new_environ[i--]);
			return (free(new_environ), perror("minishell"), -1);
		}
		i++;
	}
	new_environ[env_count] = ft_strdup(variable[index]);
	new_environ[env_count + 1] = NULL;
	free_array(*env_ptr);
	*env_ptr = new_environ;
	return (1);
}

/*
int	ft_export(char **variable, char ***env_ptr, int fd, int *unset_path)
{
	int	j;
	int	result;
	int	env_count;
	int	result;

	j = 1;
	env_count = 1;
	if (variable[1] == NULL)
		return (ft_handle_no_variables(env_ptr, fd, *unset_path));
	while (variable[j])
	{
		env_count = j;
		if (!check_variable_name(variable[j], unset_path))
			return (1);
		result = update_existing_variable(variable, env_ptr, &env_count, 0);
		if (result == -1)
		{
			if (parse_existing_variable(variable[env_count], 0) == -1)
			{
				j++;
				continue ;
			}
			return (1);
		}
		else if (result == 0)
			result = add_new_variable(variable, env_ptr, env_count, j);
		if (result == -1)
			return (1);
		env_count++;
		j++;
	}
	return (0);
}*/

int	process_variable(char **variable, char ***env_ptr, int *unset_path,
		t_norm *norm)
{
	int	result;

	if (!check_variable_name(variable[norm->j], unset_path))
		return (1);
	result = update_existing_variable(variable, env_ptr, &norm->env_count, 0);
	if (result == -1)
	{
		if (parse_existing_variable(variable[norm->env_count], 0) == -1)
			return (-2);
		return (1);
	}
	else if (result == 0)
		result = add_new_variable(variable, env_ptr, norm->env_count, norm->j);
	if (result == -1)
		return (1);
	norm->env_count++;
	return (0);
}

int	ft_export(char **variable, char ***env_ptr, int fd, int *unset_path)
{
	t_norm	norm;
	int		result;

	norm.j = 1;
	norm.env_count = 1;
	if (variable[1] == NULL)
		return (ft_handle_no_variables(env_ptr, fd, *unset_path));
	while (variable[norm.j])
	{
		norm.env_count = norm.j;
		result = process_variable(variable, env_ptr, unset_path, &norm);
		if (result == 1)
			return (1);
		else if (result == -2)
		{
			norm.j++;
			continue ;
		}
		norm.j++;
	}
	return (0);
}
