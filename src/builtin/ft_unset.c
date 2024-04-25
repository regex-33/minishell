#include "../inc/minishell.h"


int	ft_unset(char *variable, char **env)
{
	//extern char	**env;
	int			env_count;
	char		**new_environ;
	int			i;
	int			j;
	int			k;
	char		*equals_pos;
	int			var_length;

	env_count = 0;
	i = 0;
	if (!variable)
		return (1);
	while (env[env_count] != NULL)
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
		equals_pos = ft_strchr(env[i], '=');
		if (equals_pos == NULL)
		{
			i++;
			continue ;
		}
		var_length = equals_pos - env[i];
		if (!ft_strncmp(variable, env[i], var_length))
			i++;
		else
		{
			new_environ[j] = ft_strdup(env[i]);
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
	env= new_environ;
	return (0);
}
