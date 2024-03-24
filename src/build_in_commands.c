#include "../inc/minishell.h"

int	ft_echo_actions(char *str, int *index, int *in_quotes)
{
	if (str[*index] == '"' && !*in_quotes)
	{
		*in_quotes = 1;
		*index += 1;
		return (0);
	}
	else if (str[*index] == '"' && *in_quotes)
	{
		*in_quotes = 0;
		*index += 1;
		return (0);
	}
	if (str[*index] == ' ' && !*in_quotes)
	{
		if (str[*index + 1] == ' ')
		{
			*index += 1;
			return (0);
		}
	}
	return (1);
}

void	ft_echo(char **args, char *str)
{
	int		i;
	char	*temp;
	int		in_quotes;

	in_quotes = 0;
	if (!strcmp(args[1], "-n"))
	{
		i = 3;
		temp = ft_strnstr(str, "-n", strlen(str));
		while (temp[i])
		{
			if (!ft_echo_actions(temp, &i, &in_quotes))
				continue ;
			if (temp[i] != ' ' && temp[i + 1] != '\0')
				ft_putchar(temp[i]);
			i++;
		}
	}
	else
	{
		i = 5;
		temp = ft_strnstr(str, "echo", strlen(str));
		while (temp[i])
		{
			if (!ft_echo_actions(temp, &i, &in_quotes))
				continue ;
			if (temp[i] != ' ' && temp[i + 1] != '\0')
				ft_putchar(temp[i]);
			i++;
		}
		write(1, "\n", 1);
	}
}

void	ft_change_dir(const char *path)
{
	if (path != NULL)
	{
		chdir(path);
	}
	else
	{
		printf("Usage: cd <directory>\n");
	}
}

void	ft_exit(void)
{
	exit(0);
}

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
void	get_value(char *name)
{
	char	*temp;

	temp = getenv(name);
	if (!temp)
		printf("Variable not found!\n");
	else
		printf("%s\n", temp);
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