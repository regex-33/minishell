#include "../inc/minishell.h"

int	ft_export(char *variable)
{
	extern char	**environ;
	int			env_count;
	char		**new_environ;
	int			i;

	env_count = 0;
	i = 0;
	if (!ft_strchr(variable, '='))
		return (1);
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
		new_environ[i] = strdup(environ[i]);
		i++;
	}
	new_environ[env_count] = strdup(variable);
	new_environ[env_count + 1] = NULL;
	environ = new_environ;
	return (0);
}

int	ft_unset(char *variable)
{
	extern char	**environ;
	int			env_count;
	char		**new_environ;
	int			i;
	int			j;
    int         k;
	char		*equals_pos;
	int			var_length;

	env_count = 0;
	i = 0;
	while (environ[env_count] != NULL)
		env_count++;
	new_environ = malloc(env_count * sizeof(char *));
	if (new_environ == NULL)
	{
		perror("malloc fial");
		return (1);
	}
	j = 0;
	while (i < env_count)
	{
		equals_pos = strchr(environ[i], '=');
		if (equals_pos == NULL)
		{
			i++;
			continue ;
		}
		var_length = equals_pos - environ[i];
		if (!ft_strncmp(variable, environ[i], var_length))
			i++;
		else
		{
			new_environ[j] = strdup(environ[i]);
			if (new_environ[j] == NULL)
			{
				perror("strdup");
				k = 0;
				while (k < j)
				{
					free(new_environ[k]);
					k++;
				}
				free(new_environ);
				return (1);
			}
			i++;
			j++;
		}
	}
	new_environ[j] = NULL;
	environ = new_environ;
	return (0);
}
