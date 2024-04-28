#include "../../inc/minishell.h"



/* don't forget to free the memory if join is failed */
int	ft_echo(char **args, int fd)
{
	char	*str = NULL;
	int		index;

	index = 1;
	if (!args || !args[0] || !args[1])
		return (ft_putchar_fd('\n', fd), 1);
	if (!ft_strcmp(args[1], "-n"))
	{
		if (args[2] != NULL)
			index = 2;
		else
			return (0);
	}
	str = join_strings(args, index);
	if (str == NULL)
		return (perror("minishell"), 1);
	ft_putstr_fd(str, fd);
	if (index != 2)
		ft_putchar_fd('\n', fd);
	free(str);
	return (0);
}
