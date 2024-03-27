#include "../inc/minishell.h"

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