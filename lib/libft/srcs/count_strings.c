#include "libft.h"

int	count_strings(const char *str, char delim)
{
	int	count;
	int	inside_quotes;

	count = 0;
	inside_quotes = 0;
	while (*str != '\0')
	{
		if (*str == '\'' || *str == '"')
		{
			inside_quotes = !inside_quotes;
		}
		if (*str == delim && !inside_quotes)
		{
			count++;
		}
		str++;
	}
	return (count + 1);
}