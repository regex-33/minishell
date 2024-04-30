#include "../../inc/minishell.h"

int	ft_pwd(int fd, t_context *ctx)
{
	char	cwd[1024];
	char	*temp;

	if (getcwd(cwd, sizeof(cwd)) != NULL)
		return (ft_putstr_fd(cwd, fd), ft_putchar_fd('\n', fd), 0);
	else
	{
		if (ctx->last_pwd)
			return (ft_putstr_fd(ctx->last_pwd, fd), ft_putchar_fd('\n', fd), 0);
		else
		{
			temp = get_value("PWD", ctx->env);
			if (temp)
				return (ft_putstr_fd(temp, fd), ft_putchar_fd('\n', fd), 0);
			else
				return (ft_putstr_fd("minishell: pwd: error retrieving current directory: getcwd: cannot access parent directories: No such file or directory\n",
						fd), 1);
		}
	}
	return (0);
}

int	ft_env(char **env, int fd)
{
	char	**env_ptr;

	env_ptr = env;
	if (!env_ptr)
		return (printf("minishell"), 1);
	while (*env_ptr != NULL)
	{
		if (ft_strchr(*env_ptr, '=') != NULL)
		{
			ft_putstr_fd(*env_ptr, fd);
			ft_putchar_fd('\n', fd);
		}
		env_ptr++;
	}
	return (0);
}
