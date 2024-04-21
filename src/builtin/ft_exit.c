#include "../../inc/minishell.h"

char	*skip_chars(const char *str)
{
	const char	*start = str;
	const char	*end;
	int			flag;
	size_t		new_len;
	char		*result;

	flag = 0;
	if (!str)
		return NULL;
	end = str + strlen(str) - 1;
	while (*start && (*start == '"' || *start == '\'' || *start == '+' || *start == '-'))
	{
		if (*start == '-')
			flag = 1;
		start++;
	}
	while (end >= start && (*end == '"' || *end == '\''))
		end--;
	new_len = end - start + 1 + flag;
	result = malloc(new_len + 1);
	if (!result)
	{
		ft_putstr_fd("skip chars malloc is failed\n", 2);
		return (NULL);
	}
	if (flag)
		result[0] = '-';
	if (result != NULL)
	{
		ft_strncpy(result + flag, start, new_len);
		result[new_len] = '\0';
	}
	return (result);
}

bool	is_numeric(const char *str)
{
	while (*str)
	{
		if (*str != '-' && !(*str >= '0' && *str <= '9'))
		{
			return (false);
		}
		str++;
	}
	return (true);
}

void	ft_exit(const char *exit_code)
{
	char	*endptr;
	int		code;

	if (exit_code && count_strings(exit_code, ' ') > 1)
	{
		ft_putstr_fd(" too many arguments\n", 2);
		exit(1);
	}
	endptr = skip_chars(exit_code);
	if (!endptr)
		exit(0);
	else if (!is_numeric(endptr))
	{
		ft_putstr_fd(" numeric argument required\n", 2);
		exit(2);
	}
	code = atoi(endptr);
	if (errno == ERANGE || code < -128 || code > 255)
	{
		exit(42);
	}
	if (code < 0)
		code = 256 + code;
	exit(code);
}