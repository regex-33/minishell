/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   single_duble_dollar.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:47:05 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 13:47:07 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_chars(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}

int	is_spcial_chars(char c)
{
	return (c != '\0' && (ft_isalpha(c) || c == '_' || c == '?'));
}

char	*get_value(char *name, char **env)
{
	int	i;

	i = 0;
	if (ft_strcmp(name, "?") == 0)
		return (ft_itoa(get_status(0, 0)));
	while (env && *env)
	{
		i = 0;
		while ((*env)[i] == name[i] && name[i] != '\0')
			i++;
		if ((*env)[i] == '=' && name[i] == '\0')
			return (ft_strdup(*env + i + 1));
		env++;
	}
	return (NULL);
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
		return (NULL);
	ft_strncpy(substring, start, length);
	substring[length] = '\0';
	return (substring);
}

int	split_and_add_to_list(t_list **list, t_expanding *expanding, char *value)
{
	char	**value_split;
	int		index;
	int		space;

	index = 0;
	expanding->join = ft_strjoin_free(expanding->join, value);
	value_split = ft_split(expanding->join, ' ');
	if (!expanding->join || !value_split)
		return (free(value), false);
	space = has_trailing_spaces(value);
	while (value_split[index])
	{
		if (!space && value_split[index + 1] == NULL)
		{
			expanding->join = ft_strdup(value_split[index]);
			if (expanding->join == NULL)
				return (free_array(value_split), false);
			index++;
			break ;
		}
		if (!add_to_list(list, value_split[index]))
			return (free_array(value_split), false);
		index++;
	}
	return (free_array(value_split), true);
}
