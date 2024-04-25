#include "../../inc/minishell.h"

void	ft_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
	}
	else
	{
		// hadle this case if errno :  errno = ERANGE; should add 5000 to the size of the buffer	
		perror("getcwd() error");
	}
}

void	ft_env(char **env)
{
	char		**env_ptr;

	env_ptr = env;
	if (!env_ptr)
	{
		ft_printf("THIS IS NULL\n");
		return ;
	}
	while (*env_ptr != NULL)
	{
		if (ft_strchr(*env_ptr, '=') != NULL)
			ft_printf("%s\n", *env_ptr);
		env_ptr++;
	}
}

/* i think should free the memory when you change the value of environ*/

/*
int	ft_unset(char *variable)
{
	extern char	**environ;
	int			env_count;
	char		**new_environ;
	int			i;

	env_count = 0;
	i = 0;
	while (environ[env_count] != NULL)
		env_count++;
	new_environ = malloc(env_count * sizeof(char *));
	if (new_environ == NULL)
	{
		perror("malloc");
		return (1);
	}
	int j = 0;
	while (i < env_count)
	{
		if (!strcmp(variable, environ[i]))
		{
			printf("result : %s\n",environ[i]);
			i++;
		}
		else
		{
			new_environ[j] = strdup(environ[i]);
			i++;
			j++;
		}
	}
	new_environ[env_count] = NULL;
	environ = new_environ;
	return (0);
}*/