#include "../../inc/minishell.h"

/*
int	handle_plus_sign(char **env, char *str, int name_len, int *env_count_local)
{
	while (env[*env_count_local] != NULL)
	{
		if (!ft_strncmp(env[*env_count_local], str, name_len))
		{
			if (!ft_strchr(env[*env_count_local], '='))
				env[*env_count_local] = ft_strjoin(env[*env_count_local],
						"=");
			env[*env_count_local] = ft_strjoin(env[*env_count_local],
					&str[name_len + 2]);
			return (1);
		}
		*env_count_local += 1;
	}
	return (0);
}

int	handle_equal_sign(char **env, char *str, int to_equal, int *env_count_local)
{
	char	*temp;
	int		len;

	len = 0;
	while (env[*env_count_local] != NULL)
	{
		len = ft_strchr(env[*env_count_local], '=') - env[*env_count_local];
		if (len == to_equal && !ft_strncmp(env[*env_count_local], str,
				to_equal))
		{
			//printf("str: %s\n", str);
			temp = ft_strdup(str);
			if (!temp)
				return (-1);
			env[*env_count_local] = temp;
			return (1);
		}
		// if (ft_strchr(env[*env_count_local], '=') == NULL)
		// {
		// 	temp = ft_strjoin(env[*env_count_local], "=");
		// 	if (!temp)
		// 		return (-1);
		// 	env[*env_count_local] = temp;
		// }
		*env_count_local += 1;
	}
	return (0);
}*/
int	handle_plus_sign(char **env, char *str, int name_len, int *env_count_local)
{
	while (env[*env_count_local] != NULL)
	{
		if (!ft_strncmp(env[*env_count_local], str, name_len))
		{
			if (!ft_strchr(env[*env_count_local], '='))
				env[*env_count_local] = ft_strjoin(env[*env_count_local], "=");
			env[*env_count_local] = ft_strjoin(env[*env_count_local],
					&str[name_len + 2]);
			return (1);
		}
		*env_count_local += 1;
	}
	return (0);
}

int	handle_equal_sign(char **env, char *str, int to_equal, int *env_count_local)
{
	char	*temp;
	int		len;

	if (to_equal < 0)
		to_equal = ft_strlen(str);
	while (env[*env_count_local] != NULL)
	{
		len = ft_strchr(env[*env_count_local], '=') - env[*env_count_local];
		if (len < 0)
			len = ft_strlen(env[*env_count_local]);
		// printf("str : %s\n", str);
		// printf("env : %s\n", env[*env_count_local]);
		// printf("to_equal : %d\n", to_equal);
		// printf("len : %d\n", len);
		if ((ft_strchr(str, '=') && len == to_equal)
			&& !ft_strncmp(env[*env_count_local], str, to_equal))
		{
			temp = ft_strdup(str);
			if (!temp)
				return (perror("minishell"), -1);
			env[*env_count_local] = temp;
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
			free(new_environ);
			return (perror("minishell"), -1);
		}
		i++;
	}
	new_environ[env_count] = ft_strdup(variable[index]);
	new_environ[env_count + 1] = NULL;
	free_array(*env_ptr);
	*env_ptr = new_environ;
	return (1);
}

int	parse_existing_variable(char *str)
{
	int	add_to_value;

	add_to_value = 0;
	if (!str)
		return (perror("minishell"), -1);
	if (!is_valid_identifier(str))
		return (ft_putstr_fd("minishell: export: ", 2), ft_putstr_fd(str, 2),
			ft_putstr_fd(": not a valid identifier\n", STDERR_FILENO), -1);
	if (ft_strchr(str, '+') != NULL)
		add_to_value = 1;
	return (add_to_value);
}

int	update_existing_variable(char **variable, char ***env_ptr, int *env_count,
		int add_to_value)
{
	char	**env;
	char	*str;
	int		env_count_local;
	int		result;

	env = *env_ptr;
	str = variable[*env_count];
	env_count_local = 0;
	add_to_value = parse_existing_variable(str);
	if (add_to_value == -1)
		return (-1);
	if (add_to_value == 1)
		result = handle_plus_sign(env, str, (ft_strchr(str, '+') - str),
				&env_count_local);
	else
		result = handle_equal_sign(env, str, (ft_strchr(str, '=') - str),
				&env_count_local);
	if (result == -1)
		return (perror("minishell"), -1);
	else if (result == 1)
	{
		*env_ptr = env;
		*env_count = env_count_local;
		return (1);
	}
	*env_count = env_count_local;
	return (0);
}

int	ft_export(char **variable, char ***env_ptr, int fd)
{
	int	j;
	int	result;
	int	env_count;

	j = 1;
	env_count = 1;
	if (variable[1] == NULL)
	{
		if (!ft_sort_export_cmd(*env_ptr, fd, 0))
			return (1);
		return (0);
	}
	while (variable[j])
	{
		printf("variable: %s\n", variable[j]);
		env_count = j;
		if (!check_variable_name(variable[j]))
			return (1);
		result = update_existing_variable(variable, env_ptr, &env_count, 0);
		if (result == -1)
			return (1);
		else if (result == 0)
			result = add_new_variable(variable, env_ptr, env_count, j);
		if (result == -1)
			return (1);
		env_count++;
		j++;
	}
	return (0);
}
