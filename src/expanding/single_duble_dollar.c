#include "minishell.h"



char	*get_value(char *name, char **env)
{
	int		i;

	i = 0;
	while (env && *env)
	{
		i = 0;
		while ((*env)[i] == name[i] && name[i] != '\0')
			i++;
		if ((*env)[i] == '=' && name[i] == '\0')
			return (*env + i + 1);
		env++;
	}
	return (NULL);
}


int is_valid_chars(char c)
{
    return (isalnum(c) || c == '_');
}
char	*extract_dollar(const char *str)
{
	const char	*start;
	const char	*end;
	size_t		length;
	char		*substring;

	if (str[0] != '$')
		return ((char *)str);
	start = str;
	end = str + 1;
	while (*end != '\0' && is_valid_chars(*end))
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
    //printf("string : %s\n", substring);
	return (substring);
}

char *ft_strjoin_free(char *s1, const char *s2)
{
    size_t len1 = (s1 != NULL) ? strlen(s1) : 0;
    size_t len2 = strlen(s2);
    
    // Allocate memory for the new string
    char *result = (char *)malloc(len1 + len2 + 1);
    if (result == NULL)
    {
        // Handle memory allocation failure
        perror("Memory allocation failed");
        return NULL;
    }
    
    // Copy s1 and s2 into the new string
    if (s1 != NULL)
        strcpy(result, s1);
    else
        result[0] = '\0'; // Initialize empty string if s1 is NULL
    
    strcat(result, s2); // Concatenate s2 to result
    
    // Free the original s1 string
    free(s1);
    
    return result;
}

char *ft_strndup(const char *src, size_t n)
{
    // Determine the length of the substring to copy
    size_t len = strnlen(src, n);
    
    // Allocate memory for the new string
    char *dest = (char *)malloc(len + 1);
    if (dest == NULL)
    {
        // Handle memory allocation failure
        perror("Memory allocation failed");
        return NULL;
    }
    
    // Copy the substring from src to dest
    strncpy(dest, src, len);
    dest[len] = '\0'; // Ensure null-terminated
    
    return dest;
}

t_list **ft_echo_process(t_list **list, char *temp, char **env)
{
	int		in_quotes = 0;
	int		in_single_quotes = 0;
	int		i = 0;
	char	quote = '\0';
	char	*join = NULL;
	int	have_asterisk = 0;

	//printf("i am in before while\n");
	while (temp[i])
	{ 
		//printf("i am in while\n");
		if (temp[i] == '*' && in_quotes  == 0 && in_single_quotes == 0)
		{
			have_asterisk = 1;
		}
		if (temp[i] == '"' && !in_single_quotes)
		{
			quote = (in_quotes) ? '\0' : '"';
			in_quotes = !in_quotes;
			i++;
			continue;
		}
		if (temp[i] == '\'' && !in_quotes)
		{
			quote = (in_single_quotes) ? '\0' : '\'';
			in_single_quotes = !in_single_quotes;
			i++;
			continue;
		}
		if (temp[i] == '$' && quote != '\'')
		{
			if (!strncmp(temp + i, "$?", 2))
			{
				char *str = ft_itoa(get_last_exit_status());
				join = ft_strjoin_free(join, str);
				free(str);
				i += 2;
				continue;
			}
			else
			{
				char *variable = extract_dollar(temp + i);
				if (variable)
				{
					join = ft_strjoin_free(join, get_value(variable + 1, env));
					int len = strlen(variable);
					free(variable);
					i += len;
					continue;
				}
			}
		}
		char *char_str = ft_strndup(temp + i, 1);
		join = ft_strjoin_free(join, char_str);
		free(char_str);
		i++;
	}
//	printf("join : %s\n", join);
	if (!have_asterisk)
	{
		ft_lstadd_back_libft(list, ft_lstnew(join));
		return (list);
	}
	//printf(" i am here : %s | %d\n", join, have_asterisk);
	
	expand_asterisk(join, list); YOUSSEF HELLO * HDHFHE
	// if (!list)
	// {
	// 	perror("expand_asterisk failed\n");
	// 	return (NULL);
	// }
	return (list);
}