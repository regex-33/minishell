/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expanding_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:46:47 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 13:46:48 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_linked_list(t_list *head)
{
	t_list	*current;
	t_list	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
}

bool	has_trailing_spaces(const char *str)
{
	int	len;
	int	i;

	if (!str)
		return (false);
	len = strlen(str);
	if (len == 0)
		return (false);
	i = len - 1;
	while (i >= 0 && (str[i] == ' ' || str[i] == '\t'))
		i--;
	return (i < len - 1);
}

int	handle_double_quotes(t_expanding *expanding)
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
	return (0);
}

int	handle_single_quotes(t_expanding *expanding)
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
	return (0);
}

int	handle_quotes_asterisk(t_expanding *expanding, char c, int *i)
{
	int	result;

	result = 0;
	if (c == '"' && !expanding->in_single_quotes)
		result = handle_double_quotes(expanding);
	else if (c == '\'' && !expanding->in_quotes)
		result = handle_single_quotes(expanding);
	if (result)
		return (1);
	(*i)++;
	return (0);
}
