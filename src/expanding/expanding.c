/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:46:38 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 13:46:39 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
char	*handle_dollar_sign(t_list **list, int *i, char *temp,
		t_expanding *expanding)
{
	char	*variable;
	char	*value;

	variable = NULL;
	value = NULL;
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
		if (value && !ft_strchr(value, '\"') && ft_strchr(value, '*'))
			expanding->have_asterisk = 1;
		if (value && expanding->quote != '\"' && (ft_strchr(value, ' ')
				|| ft_strchr(value, '\t')))
		{
			if (!split_and_add_to_list(list, expanding, value))
			{
				*i += ft_strlen(variable);
				return (free(value), free(variable), NULL);
			}
		}
		else
			expanding->join = ft_strjoin_free(expanding->join, value);
		*i += ft_strlen(variable);
		free(value);
		free(variable);
	}
	return (temp);
}
*/

char	*handle_dollar_sign(t_list **list, int *i, char *temp,
		t_expanding *expanding)
{
	char	*variable;
	char	*value;

	variable = extract_dollar(temp + *i);
	if (variable)
	{
		value = extract_and_get_value(temp, i, expanding);
		if (!value)
			return (free(variable), NULL);
		if (!handle_special_cases(list, expanding, value))
		{
			*i += ft_strlen(variable);
			return (free(value), free(variable), NULL);
		}
		*i += ft_strlen(variable);
		free(value);
		free(variable);
	}
	return (temp);
}

/*
int	handle_quotes_asterisk(t_expanding *expanding, char c, int *i)
{
	if (c == '"' && !expanding->in_single_quotes)
	{
		if (expanding->in_quotes)
			expanding->quote = '\0';
		else
		{
			expanding->quote = '"';
			expanding->join = ft_strjoin_free(expanding->join, "");
			if (!expanding->join)
				return (1);
		}
		expanding->in_quotes = !expanding->in_quotes;
	}
	if (c == '\'' && !expanding->in_quotes)
	{
		if (expanding->in_single_quotes)
			expanding->quote = '\0';
		else
		{
			expanding->quote = '\'';
			expanding->join = ft_strjoin_free(expanding->join, "");
			if (!expanding->join)
				return (1);
		}
		expanding->in_single_quotes = !expanding->in_single_quotes;
	}
	(*i)++;
	return (0);
}
t_list	**expand_arg_list(t_list **list, char *temp, t_context *ctx)
{
	t_expanding	expanding;
	int			i;
	t_list		*new_node;

	i = 0;
	if (!init_expanding(&expanding, temp, ctx))
		return (NULL);
	while (temp[i])
	{
		if (temp[i] == '*' && expanding.in_quotes == 0
			&& expanding.in_single_quotes == 0)
			expanding.have_asterisk = 1;
		if ((temp[i] == '"' && !expanding.in_single_quotes) || (temp[i] == '\''
				&& !expanding.in_quotes))
		{
			if (handle_quotes_asterisk(&expanding, temp[i], &i))
				return (free(expanding.join), NULL);
			continue ;
		}
		if (temp[i] == '$' && is_spcial_chars(temp[i + 1])
			&& expanding.quote != '\'')
		{
			if (!handle_dollar_sign(list, &i, temp, &expanding))
				return (free(expanding.join), NULL);
			continue ;
		}
		if (handle_regular_char(temp, &expanding.join, &i))
			return (free(expanding.join), NULL);
	}
	if (expanding.have_asterisk)
		return (expand_asterisk(expanding.join, list), list);
	new_node = ft_lstnew(expanding.join);
	if (!new_node)
		return (free(expanding.join), NULL);
	return (ft_lstadd_back_libft(list, new_node), list);
}*/
int	process_asterisk_and_quotes(t_expanding *expanding, char *temp, int *i)
{
	if (temp[*i] == '*' && expanding->in_quotes == 0
		&& expanding->in_single_quotes == 0)
		expanding->have_asterisk = 1;
	if ((temp[*i] == '"' && !expanding->in_single_quotes) || (temp[*i] == '\''
			&& !expanding->in_quotes))
	{
		if (handle_quotes_asterisk(expanding, temp[*i], i))
			return (0);
		return (-1);
	}
	return (1);
}

int	process_dollar_sign(t_list **list, char *temp, int *i,
		t_expanding *expanding)
{
	if (temp[*i] == '$' && is_spcial_chars(temp[*i + 1])
		&& (expanding->quote != '\'' || !expanding->do_expand))
	{
		if (!handle_dollar_sign(list, i, temp, expanding))
			return (0);
		return (-1);
	}
	return (1);
}

t_list	**final_expansion(t_expanding *expanding, t_list **list)
{
	t_list	*new_node;

	if (expanding->have_asterisk)
		return (expand_asterisk(expanding->join, list), list);
	new_node = ft_lstnew(expanding->join);
	if (!new_node)
		return (free(expanding->join), NULL);
	return (ft_lstadd_back_libft(list, new_node), list);
}

t_list	**expand_arg_list(t_list **list, char *temp, t_context *ctx,
		int do_expand)
{
	t_expanding	expanding;
	int			i;
	int			flag;

	i = 0;
	if (!init_expanding(&expanding, temp, ctx, do_expand))
		return (NULL);
	while (temp[i])
	{
		flag = 2;
		if (!expanding.do_expand)
			flag = process_asterisk_and_quotes(&expanding, temp, &i);
		if (flag == -1)
			continue ;
		else if (!flag)
			return (free(expanding.join), NULL);
		flag = process_dollar_sign(list, temp, &i, &expanding);
		if (flag == -1)
			continue ;
		else if (!flag)
			return (free(expanding.join), NULL);
		if (handle_regular_char(temp, &expanding.join, &i))
			return (free(expanding.join), NULL);
	}
	return (final_expansion(&expanding, list));
}
