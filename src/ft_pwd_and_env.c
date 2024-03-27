#include "../inc/minishell.h"

void	ft_pwd(void)
{
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		printf("%s\n", cwd);
	}
	else
	{
		perror("getcwd() error");
	}
}

void	ft_env(void)
{
	char		**env_ptr;
	extern char	**environ;

	env_ptr = environ;
	while (*env_ptr != NULL)
	{
		printf("%s\n", *env_ptr);
		env_ptr++;
	}
}
char	*get_value(char *name)
{
	char	*temp;

	temp = getenv(name);
	if (!temp)
		printf("Variable not found!\n");
	return (temp);
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