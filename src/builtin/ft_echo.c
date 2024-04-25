#include "../../inc/minishell.h"


void	ft_echo_print(char *temp, int start_index, char **env)
{	
	int	in_quotes;
	int	in_single_quotes;
	int	i;
    char quote;
    char *str;

	in_quotes = 0;
	in_single_quotes = 0;
	i = start_index;
	while (temp[i])
	{
		if (temp[i] == '"' && in_single_quotes == 0)
		{
            quote = '"';
			in_quotes = !in_quotes;
			i++;
			continue ;
		}
		if (temp[i] == '\'' && in_quotes == 0)
		{
            quote = '\'';
			in_single_quotes = !in_single_quotes;
			i++;
			continue ;
		}
		if ((temp[i] == ' ' || temp[i] == '\t') && !in_quotes
			&& !in_single_quotes)
		{
			while ((temp[i + 1] == ' ' || temp[i + 1] == '\t'))
				i++;
			if (temp[i + 1] == '\0')
				break ;
		}
        if (temp[i] == '$' && quote != '\'')
        {
            //printf("i ma here : %s\n", temp + i);
            if (!ft_strncmp(temp + i, "$?", 2))
            {
                ft_printf("%d", get_last_exit_status());
                i += 2;
                continue;
            }
            else
            {
                str = extract_dollar(temp + i);
                int len = strlen(str);
                if (len > 1)
                {
                    i += len;
                    //printf("string : %s | quote : %c\n", str, quote);
                    ft_printf("%s", get_value(str + 1, env));
                    free(str);
                    continue;
                }
            }
        }
		ft_putchar(temp[i]);
		i++;
	}
}

void	ft_echo(char **args, char *str, char **env)
{
	char	*temp;
	int		i;

	if (!ft_strcmp(args[1], "-n"))
	{
		i = 3;
		temp = ft_strnstr(str, "-n", ft_strlen(str));
	}
	else if (!ft_strcmp(args[1], "$?"))
	{
		ft_printf("%d\n", get_last_exit_status());
		return ;
	}
	else
	{
		i = 5;
		temp = ft_strnstr(str, "echo", ft_strlen(str));
	}
	ft_echo_print(temp, i, env);
	if (!ft_strcmp(args[1], "-n"))
		return ;
	write(1, "\n", 1);
}
