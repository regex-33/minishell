#include "../inc/minishell.h"


int	ft_unset(char *variable, char ***env_ptr)
{
	int			env_count;
	char		**new_environ;
	int			i;
	int			j = 0;
	int			k;
	char		*equals_pos;
	int			var_length;
	char		**env;

	env_count = 0;
	i = 0;
	env = *env_ptr;
	if (!variable)
		return (1);
	while (env[env_count] != NULL)
		env_count++;
	new_environ = malloc(env_count * sizeof(char *));
	if (new_environ == NULL)
		return (perror("malloc fial"), 1);
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
				k = 0;
				while (k < j)
				{
					free(new_environ[k]);
					k++;
				}
				return (free(new_environ), perror("minishell"), 1);
			}
			i++;
			j++;
		}
	}
	new_environ[j] = NULL;
	*env_ptr = new_environ;
	return (0);
}
