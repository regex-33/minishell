#include "../../inc/minishell.h"
  
int	ft_pwd(int fd)
{
	//extern char last_path[1024];
	char	cwd[1024];

	if (getcwd(cwd, sizeof(cwd)) != NULL)
	{
		ft_putstr_fd(cwd, fd);
		ft_putchar_fd('\n', fd);
		//ft_strcpy(last_path, cwd);
	}
	else
	{
		// if (last_path[0] != '\0')
		// {
		// 	ft_putstr_fd(last_path, fd);
		// 	ft_putchar_fd('\n', fd);
		// }
		// else
		// {
		//	perror("getcwd() error");
			return (1);
//		}
	}
	return (0);
}

int	ft_env(char **env, int fd)
{
	char	**env_ptr;

	env_ptr = env;
	if (!env_ptr)
	{
		ft_printf("THIS IS NULL\n");
		return (1);
	}
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
