#include "../../inc/minishell.h"

/*
int	is_valid_identifier(const char *variable)
{
	size_t i; 

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
	extern char	**environ;
	int			env_count;
	char		**new_environ;
	extern		int last_exit_status;
	int			i;

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
	size_t i; 

	i = 1;
	printf("variable : %s\n", variable);
	if (!ft_isalpha(variable[0]) && variable[0] != '_')
	{
		return (0);
	}
	
	while (variable[i] != '\0' && variable[i] != '=')
	{
		if (!ft_isalnum(variable[i]) && variable[i] != '_' && variable[i] != '$')
		{
			if (variable[i] == '+' && variable[i + 1] != '=')
				return (0);
		}
		i++;
	}
	return (1);
}

void ft_print_free(char **variable)
{
	int i;
	char *str;
	char *temp;

	i = 0;
	while (variable[i] != NULL)
	{
		temp = variable[i];
		str = ft_strchr(variable[i], '=');
		ft_putstr_fd("declare -x ", 1);
		while(*variable[i] && *variable[i] != '=')
			write(1, variable[i]++, 1);

		ft_putstr_fd("=\"", 1);
		if (str != NULL)
			ft_putstr_fd(str + 1, 1);
		ft_putstr_fd("\"\n", 1);
		free(temp);
		i++;
	}
	free(variable);
}
int	ft_sort_export_cmd(char **environ_exp)
{
	int i;
	char **variable;
	int	 env_count;

	i = 0;
	env_count = 0;
	while (environ_exp[env_count] != NULL)
		env_count++;
	/*	don't forget to free memory	*/
	variable = malloc((env_count + 1) * sizeof(char *));
	if (variable == NULL)
	{
		perror("malloc has failed\n");
		return 0;
	}	
	if (environ_exp[i] == NULL)
	{
		printf("environ is null\n");
		return 0;
	}
	while(environ_exp[i])
	{
		//write(1, "hello\n", 6);
		//ft_printf("environ[%d] : %s\n\n\n", i, environ[i]);
		variable[i] = ft_strdup(environ_exp[i]);
		i++;
	}
	variable[env_count] = NULL;
	merge_sort(variable, 0, env_count - 1);	
	ft_print_free(variable);
	return 1;

}


int	ft_export(char **variable, char ***envv)
{
	int			env_count;
	char		**new_environ;
	extern		int last_exit_status;
	char		*str = NULL;
	int			add_to_value;
	int			i;
	int 		name_len;
	int		 	to_equal;
	int			j = 1;
	int			should_countinue = 0;

	char **env = *envv;
	env_count = 0;
	i = 0;
	/* check if str is null */
	to_equal = 0;
	if (variable[1] == NULL)
	{
		if (!ft_sort_export_cmd(env))
			return (1);
		return (0);
	}

	while (variable[j])
	{
		env_count = 0;
		str = variable[j];
		if (!str)
		{
			perror("minishell: malloc error");
			return 0;
		}
		printf("str : %s\n", str);
		if (!is_valid_identifier(str))
		{
			last_exit_status = 1;
			ft_putstr_fd(" not a valid identifier\n", 2);
			j++;
			should_countinue = 1;
			break;
	//		return (1);
		}

		if (ft_strchr(str, '+') != NULL)
			add_to_value = 1;
		else
			add_to_value = 0;
		name_len = ft_strchr(str, '+') - str;
		to_equal = ft_strchr(str, '=') - str;
		while (env[env_count] != NULL)
		{
			if (add_to_value == 1 && ft_strncmp(env[env_count], str, name_len) == 0)
			{
				printf("env[%d] : %s\n", env_count, env[env_count]);
				env[env_count] = ft_strjoin(env[env_count], &str[name_len + 2]);
				break;
				//return (1);
			}
			else if (ft_strncmp(env[env_count], str, to_equal) == 0)
			{
				//printf("env[%d] : %s\n", env_count, env[env_count]);
				env[env_count] = ft_strdup(str);
				should_countinue = 1;
				break;
				//return (1);
			}
			env_count++;
		}
		if (should_countinue == 1)
		{
			j++;
			should_countinue = 0;
			continue;
		}
		new_environ = malloc((env_count + 2) * sizeof(char *));
		if (new_environ == NULL)
		{
			perror("minishell");
			j++;
			continue;
	//		return (1);
		}
		while (i < env_count)
		{
			new_environ[i] = ft_strdup(env[i]);
			i++;
		}
		new_environ[env_count] = ft_strdup(str);
		printf("new_environ[%d] : %s\n", env_count, new_environ[env_count]);
		new_environ[env_count + 1] = NULL;
		// while(env_count-- > 0)
		// 	free(env[env_count]);
		//free_array(env);
		*envv = new_environ;
		printf("i am here\n");
		//env_count = 1;
		// while(env[env_count] != NULL)
		// {
		// 	printf("env[%d] : %s\n", env_count, env[env_count]);
		// 	env_count++;
		// }
		//printArray(env);
		j++;
	}
	return (1);
}
