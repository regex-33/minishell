/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   etc_functions.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 14:04:08 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 14:04:09 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*join_strings(char **args, int index)
{
	char	*str;

	str = NULL;
	while (args[index])
	{
		str = ft_strjoin_free(str, args[index]);
		if (!str)
			return (NULL);
		if (!args[index + 1])
			return (str);
		str = ft_strjoin_free(str, " ");
		if (!str)
			return (NULL);
		index++;
	}
	return (str);
}

void	print_array(char **array)
{
	int	i;

	i = 0;
	ft_printf("print_array\n");
	if (!array)
		printf("Array is empty\n");
	while (array[i])
	{
		ft_printf("str :%s$\n", array[i]);
		i++;
	}
	ft_printf("-----------------\n");
}

void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

int	count_array(char **array)
{
	int	i;

	i = 0;
	if (!array || !*array)
		return (0);
	while (array[i])
		i++;
	return (i);
}

char	**ft_list_to_array(t_list *list, char **array, int i)
{
	if (!list)
		return (NULL);
	array = allocate_new_environ(ft_lstsize(list) + 1);
	if (!array)
		return (NULL);
	while (list)
	{
		if (!list->content)
		{
			list = list->next;
			continue ;
		}
		else
		{
			array[i] = ft_strdup(list->content);
			if (!array[i])
				return (perror("minishell"), free_array(array), NULL);
		}
		if (!array[i])
			return (perror("minishell"), free_array(array), NULL);
		list = list->next;
		i++;
	}
	return (array);
}
