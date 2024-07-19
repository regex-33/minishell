/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_helps.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 15:41:00 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 15:41:09 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	init_expanding(t_expanding *expanding, char *temp, t_context *ctx,
		int do_expand)
{
	if (!temp)
		return (0);
	expanding->in_quotes = 0;
	expanding->do_expand = do_expand;
	expanding->in_single_quotes = 0;
	expanding->have_asterisk = 0;
	expanding->quote = '\0';
	expanding->join = NULL;
	expanding->ctx = ctx;
	expanding->do_split = key_contains_dollar(temp);
	return (1);
}

bool	add_to_list(t_list **list, char *split_value)
{
	t_list	*new_node;

	new_node = ft_lstnew(strdup(split_value));
	if (!new_node)
		return (false);
	ft_lstadd_back_libft(list, new_node);
	return (true);
}

int	handle_regular_char(char *temp, char **join, int *i)
{
	char	*char_str;

	char_str = ft_strndup(temp + *i, 1);
	if (!char_str)
		return (1);
	*join = ft_strjoin_free(*join, char_str);
	if (!*join)
		return (free(char_str), 1);
	free(char_str);
	(*i)++;
	return (0);
}

char	*extract_and_get_value(char *temp, int *i, t_expanding *expanding)
{
	char	*variable;
	char	*value;

	variable = extract_dollar(temp + *i);
	if (variable)
	{
		value = get_value(variable + 1, expanding->ctx->env);
		if (!value && expanding->quote == '\"')
		{
			value = ft_strdup("");
			if (!value)
				return (free(variable), NULL);
		}
	}
	else
		value = NULL;
	if (variable && value)
		return (free(variable), value);
	return (free(variable), NULL);
}

int	handle_special_cases(t_list **list, t_expanding *expanding, char *value)
{
	if (value && !ft_strchr(value, '\"') && ft_strchr(value, '*'))
		expanding->have_asterisk = 1;
	if (value && expanding->quote != '\"' && expanding->do_split
		&& (ft_strchr(value, ' ') || ft_strchr(value, '\t')))
	{
		if (!split_and_add_to_list(list, expanding, value))
			return (0);
	}
	else
	{
		expanding->join = ft_strjoin_free(expanding->join, value);
		if (!expanding->join)
			return (0);
	}
	return (1);
}
