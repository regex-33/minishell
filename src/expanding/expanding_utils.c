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

void	print_linked_list(t_list *head)
{
	t_list	*current;

	current = head;
	ft_printf("\nLinked List: \n");
	while (current != NULL)
	{
		ft_printf("str: %s\n", (char *)(current->content));
		current = current->next;
	}
}

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
	head = NULL;
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
