#include "../../inc/minishell.h"

/*
int	is_valid_identifier(const char *variable)
{
	size_t	i;

	i = 1;
	if (!ft_isalpha(variable[0]) && variable[0] != '_')
		return (0);
	while (variable[i] != '\0' && variable[i] != '=')
	{
		if (!isalnum(variable[i]) && variable[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

int	ft_export(char *variable)
{
	extern char		**environ;
	int				env_count;
	char			**new_environ;
	extern		int	last_exit_status;
	int				i;

	env_count = 0;
	i = 0;
	if (!is_valid_identifier(variable))
	{
		last_exit_status = 1;
		ft_putstr_fd(" not a valid identifier\n", 2);
		return (1);
	}
	while (environ[env_count] != NULL)
		env_count++;
	new_environ = malloc((env_count + 2) * sizeof(char *));
	if (new_environ == NULL)
	{
		perror("malloc");
		return (1);
	}
	while (i < env_count)
	{
		new_environ[i] = ft_strdup(environ[i]);
		i++;
	}
	new_environ[env_count] = ft_strdup(variable);
	new_environ[env_count + 1] = NULL;
	environ = new_environ;
	return (1);
}
*/

int	is_valid_identifier(const char *variable)
{
	size_t	i;

	i = 1;
	printf("variable : %s\n", variable);
	if (!ft_isalpha(variable[0]) && variable[0] != '_')
	{
		return (0);
	}
	while (variable[i] != '\0' && variable[i] != '=')
	{
		if (!ft_isalnum(variable[i]) && variable[i] != '_'
			&& variable[i] != '$')
		{
			if (variable[i] == '+' && variable[i + 1] != '=')
				return (0);
		}
		i++;
	}
	return (1);
}

void	ft_print_free(char **variable, int fd)
{
	int		i;
	char	*str;
	char	*temp;

	i = 0;
	while (variable[i] != NULL)
	{
		temp = variable[i];
		str = ft_strchr(variable[i], '=');
		ft_putstr_fd("declare -x ", fd);
		while (*variable[i] && *variable[i] != '=')
			write(fd, variable[i]++, 1);
		ft_putstr_fd("=\"", 1);
		if (str != NULL)
			ft_putstr_fd(str + 1, fd);
		ft_putstr_fd("\"\n", fd);
		free(temp);
		i++;
	}
	free(variable);
}
int	ft_sort_export_cmd(char **environ_exp, int fd)
{
	int		i;
	char	**variable;
	int		env_count;

	i = 0;
	env_count = 0;
	while (environ_exp[env_count] != NULL)
		env_count++;
	/*	don't forget to free memory	*/
	variable = malloc((env_count + 1) * sizeof(char *));
	if (variable == NULL)
	{
		perror("malloc has failed\n");
		return (0);
	}
	if (environ_exp[i] == NULL)
	{
		printf("environ is null\n");
		return (0);
	}
	while (environ_exp[i])
	{
		// write(1, "hello\n", 6);
		// ft_printf("environ[%d] : %s\n\n\n", i, environ[i]);
		variable[i] = ft_strdup(environ_exp[i]);
		if (variable[i] == NULL)
		{
			perror("malloc has failed\n");
			while (i > 0)
				free(variable[i--]);
			return (0);
		}
		i++;
	}
	variable[env_count] = NULL;
	merge_sort(variable, 0, env_count - 1);
	ft_print_free(variable, fd);
	return (1);
}



int	check_variable_name(char *variable)
{
	int		to_equal;
	char	*str;
	extern int	last_exit_status;

	to_equal = ft_strchr(variable, '=') - variable;
	str = ft_substr(variable, 0, to_equal);
	if (!str)
	{
		perror("minishell: malloc error");
		return (0);
	}
	if (ft_strchr(str, ' '))
		return (free(str), last_exit_status = 1, ft_putstr_fd("not a valid identifier\n", 2), 0);
	return (free(str), 1);
}

int	ft_export(char **variable, char ***env_ptr, int fd)
{
	int			env_count;
	char		**new_environ;
	extern int	last_exit_status;
	char		*str = NULL;
	int			add_to_value;
	int			i;
	int			name_len;
	int			to_equal;
	int			j;
	int			should_countinue;
	char		**env = NULL;

	name_len = 0;
	to_equal = 0;
	j = 1;
	//printf("variable[1] : %s\n", variable[1]);
	if (variable[1] == NULL)
	{
		if (!ft_sort_export_cmd(*env_ptr, fd))
			return (1);
		return (0);
	}
	//if (check_variable_name(variable[1]) == 0)
	//	return (0);
	while (variable[j])
	{
		if (!check_variable_name(variable[j]))
			return (1);
		env = *env_ptr;
		should_countinue = 0;
		env_count = 0;
		i = 0;
		add_to_value = 0;
		str = variable[j];

		if (!str)
			return (perror("minishell: malloc error"), 1);

		if (!is_valid_identifier(str))
		{
			last_exit_status = 1;
			ft_putstr_fd(" not a valid identifier\n", 2);
			j++;
			should_countinue = 1;
			break ;
		}
		if (ft_strchr(str, '+') != NULL)
			add_to_value = 1;
		else
			add_to_value = 0;

		name_len = ft_strchr(str, '+') - str;
		to_equal = ft_strchr(str, '=') - str;
		while (env[env_count] != NULL)
		{
			if (add_to_value == 1 && ft_strncmp(env[env_count], str,
					name_len) == 0)
			{
				env[env_count] = ft_strjoin(env[env_count], &str[name_len + 2]);
				should_countinue = 1;
				break ;
			}
			else if (!ft_strncmp(env[env_count], str, to_equal) || !ft_strncmp(env[env_count], str, ft_strlen(str)))
			{
				env[env_count] = ft_strdup(str);
				should_countinue = 1;
				break ;
			}
			env_count++;
		}
		if (!should_countinue)
		{
			new_environ = malloc((env_count + 2) * sizeof(char *));
			if (!new_environ) 
				return (perror("minishell"), 1); // should free
			while (i < env_count)
			{
				new_environ[i] = ft_strdup(env[i]);
				i++;
			}
			new_environ[env_count] = ft_strdup(str);
			//printf("new_environ[%d] : %s\n", env_count, new_environ[env_count]);
			new_environ[env_count + 1] = NULL;
			*env_ptr = new_environ;
		}
		else
			*env_ptr = env;


		//while(env_count-- > 0)
		 	//free(env[env_count]);
		// free_array(env);
		j++;
	}
	return (0);
}
