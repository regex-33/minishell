
#include "minishell.h"

int	is_valid_identifier(const char *variable)
{
	size_t	i;

	i = 1;
	if (!ft_isalpha(variable[0]) && variable[0] != '_')
		return (0);
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

int	ft_sort_export_cmd(char **environ_exp, int fd, int i)
{
	char	**variable;
	int		env_count;

	env_count = 0;
	if (!environ_exp || !*environ_exp)
		return (perror("minishell"), 0);
	while (environ_exp[env_count] != NULL)
		env_count++;
	variable = malloc((env_count + 1) * sizeof(char *));
	if (!variable)
		return (perror("minishell"), 0);
	while (environ_exp[i])
	{
		variable[i] = ft_strdup(environ_exp[i]);
		if (variable[i] == NULL)
		{
			while (i > 0)
				free(variable[i--]);
			return (perror("minishell"), 0);
		}
		i++;
	}
	variable[env_count] = NULL;
	merge_sort(variable, 0, env_count - 1);
	return (ft_print_free(variable, fd), 1);
}

int	check_variable_name(char *variable)
{
	int			to_equal;
	char		*str;

	to_equal = ft_strchr(variable, '=') - variable;
	str = ft_substr(variable, 0, to_equal);
	if (!str)
		return (perror("minishell"), 0);
	if (ft_strchr(str, ' '))
		return (ft_putstr_fd("minishell: export: ", 2), ft_putstr_fd(str, 2),
			ft_putstr_fd(": not a valid identifier\n", 2), free(str), 0);
	return (free(str), 1);
}
