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

int	ft_exit(char **exit_code, t_context *ctx)
{
	unsigned char	code;

	if (!exit_code[1])
	{
		ctx->last_status = 0;
		exit(0);
	}
	if (exit_code && count_array(exit_code) > 2)
		return (ft_putstr_fd("minishell: exit : too many arguments\n", 2), 1);
	if (!is_numeric(exit_code[1]))
	{
		/* don't forget to free the memory */
		ft_putstr_fd("minishell: exit : ", 2);
		ft_putstr_fd(exit_code[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		ctx->last_status = 255;
		exit(255);
	}
	code = (unsigned char) ft_atoi(exit_code[1]);
	ctx->last_status = code;
	exit(code);
	return (1);
}
