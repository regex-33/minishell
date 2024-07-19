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
/*
int	split_and_add_to_list(t_list **list, t_expanding *expanding, char *value)
{
	char	**value_split;
	int		index;
	int		space;

	index = 0;
	expanding->join = ft_strjoin_free(expanding->join, value);
	if (!expanding->join)
		return (false);
	value_split = ft_split(expanding->join, ' ');
	if (!value_split)
		return (free(expanding->join), (false));
	free(expanding->join);
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
*/

int	process_split_value(t_list **list, t_expanding *expanding,
		char **value_split, int space)
{
	int	index;

	index = 0;
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

int	split_and_add_to_list(t_list **list, t_expanding *expanding, char *value)
{
	char	**value_split;
	int		space;

	expanding->join = ft_strjoin_free(expanding->join, value);
	if (!expanding->join)
		return (false);
	value_split = ft_split(expanding->join, ' ');
	if (!value_split)
		return (free(expanding->join), false);
	free(expanding->join);
	space = has_trailing_spaces(value);
	return (process_split_value(list, expanding, value_split, space));
}
