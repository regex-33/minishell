#include "../../inc/minishell.h"



/* don't forget to free the memory if join is failed */
void	ft_echo(char **args)
{
	char	*str = NULL;
	int		index;

	index = 1;
	if (!ft_strcmp(args[1], "-n"))
		index = 2;

	str = join_strings(args, index);
	if (str == NULL)
	{
		perror("minishell");
		return ;
	}
	if (index == 2)
		ft_printf("%s", str);
	else
		ft_printf("%s\n", str);
	free(str);
}
