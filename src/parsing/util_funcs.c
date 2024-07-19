/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   util_funcs.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 18:36:51 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/24 15:36:57 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	isquote(char c)
{
	return (c == '"' || c == '\'');
}

int	match_pattern(char *str, int flags, char *sentinel)
{
	int	i;
	int	found;
	int	has_sentinel;

	i = 0;
	has_sentinel = 0;
	while (str[i])
	{
		if (sentinel && ft_strchr(sentinel, str[i]))
		{
			has_sentinel = 1;
			break ;
		}
		found = ((flags & O_DIGITS) && ft_isdigit(str[i])) || \
				((flags & O_LETTERS) && ft_isalpha(str[i]));
		if (!found)
			return (-1);
		i++;
	}
	if (!sentinel || has_sentinel)
		return (i);
	return (-1);
}

size_t	wordlen(char *str)
{
	int	i;
	int	quote;

	while (*str && ft_strchr(TOKEN_D, *str))
		str++;
	i = 0;
	quote = 0;
	while (str[i])
	{
		if (quote == str[i])
			quote = 0;
		else if (!quote && isquote(str[i]))
			quote = str[i];
		else if (!quote && ft_strchr(TOKEN_D, str[i]))
			break ;
		i++;
	}
	if (quote)
		return (0);
	return (i);
}

void	toggle_quotes(char c, int *in_quotes, int *in_single_quotes)
{
	if (c == '"')
		*in_quotes = !(*in_quotes);
	else if (c == '\'')
		*in_single_quotes = !(*in_single_quotes);
}

int	process_char(char *temp, char **new_str, int i)
{
	char	*char_str;
	char	*temp_str;

	char_str = ft_strndup(temp + i, 1);
	if (!char_str)
		return (0);
	temp_str = ft_strjoin_free(*new_str, char_str);
	if (!temp_str)
		return (free(char_str), 0);
	*new_str = temp_str;
	free(char_str);
	return (1);
}
