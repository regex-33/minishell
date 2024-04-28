#include "../../inc/minishell.h"

char	*extract_after_dollar(const char *str, char quots)
{
	const char	*start;
	const char	*end;
	size_t		length;
	char		*substring;

	if (str[0] != '$')
	{
		return ((char *)str);
	}
	start = str;
	end = str + 1;
	while (*end != '\0' && *end != quots)
		end++;
	length = end - start;
	substring = (char *)malloc(length + 1);
	if (substring == NULL)
	{
		perror("Memory allocation failed");
		return (NULL);
	}
	ft_strncpy(substring, start, length);
	substring[length] = '\0';
	return (substring);
}

char	*extract_substring(const char *str)
{
	const char	*start;
	char		*substring;
	char		quote;

	substring = NULL;
	quote = '\0';
	if (*str == '\'' || *str == '\"')
		quote = *str;
	while (*str == quote)
		str++;
	start = str;
	while (*str != '\0' && *str != quote)
		str++;
	substring = (char *)malloc((str - start + 1) * sizeof(char));
	if (substring != NULL)
	{
		ft_memcpy(substring, start, str - start);
		substring[str - start] = '\0';
	}
	return (extract_after_dollar(substring, quote));
}

int	ft_change_dir(const char *path, char **env)
{
	const char	*dir;
	extern int	last_exit_status;

	if (path && count_strings(path, ' ') > 1)
	{
		last_exit_status = 1;
		ft_putstr_fd(" too many arguments\n", 2);
		return (1);
	}
	if (path != NULL)
	{
		dir = extract_substring(path);
		if (dir[0] == '$')
		{
			dir++;
			dir = get_value((char *)dir, env);
		}
		if (chdir(dir) == -1)
        {
		    last_exit_status = 1;
			printf(" i am here\n");
			ft_putstr_fd(" No such file or directory\n", 2);
			return (1);
        }
	}
	else
		chdir(get_value("HOME", env));
	return (0);
}