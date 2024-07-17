#include "../../inc/minishell.h"

bool	is_numeric(const char *str)
{
	if (*str == '+' || *str == '-')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

int	ft_exit(char **exit_code)
{
	unsigned char	code;

	if (!exit_code || !exit_code[1])
		return (ft_putendl_fd("exit", 2), exit(get_status(0, 0)), 0);
	if (exit_code && count_array(exit_code) > 2)
		return (ft_putstr_fd("minishell: exit : too many arguments\n", 2), 1);
	if (!is_numeric(exit_code[1]))
	{
		ft_putendl_fd("exit", 2);
		ft_putstr_fd("bash: exit : ", 2);
		ft_putstr_fd(exit_code[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	if (exit_code && count_array(exit_code) > 2)
		return (ft_putstr_fd("minishell: exit : too many arguments\n", 2), 1);
	code = (unsigned char) ft_atoi(exit_code[1]);
	ft_putendl_fd("exit", 2);
	return (exit(code), code);
}