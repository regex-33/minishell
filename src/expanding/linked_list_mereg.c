/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   linked_list_mereg.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 15:52:16 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 15:52:17 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
void	merge_linked_list(t_list **head, t_list *left, t_list *right)
{
	t_list	*result;

	result = NULL;
	if (left == NULL)
	{
		*head = right;
		return ;
	}
	else if (right == NULL)
	{
		*head = left;
		return ;
	}
	if (strcmp(left->content, right->content) <= 0)
	{
		*head = left;
		left = left->next;
	}
	else
	{
		*head = right;
		right = right->next;
	}
	result = *head;
	while (left != NULL && right != NULL)
	{
		if (strcmp(left->content, right->content) <= 0)
		{
			result->next = left;
			left = left->next;
		}
		else
		{
			result->next = right;
			right = right->next;
		}
		result = result->next;
	}
	if (left != NULL)
		result->next = left;
	else if (right != NULL)
		result->next = right;
}
*/

void	merge_remaining(t_list **result, t_list *left, t_list *right)
{
	while (left != NULL && right != NULL)
	{
		if (ft_strcmp(left->content, right->content) <= 0)
		{
			(*result)->next = left;
			left = left->next;
		}
		else
		{
			(*result)->next = right;
			right = right->next;
		}
		*result = (*result)->next;
	}
	if (left != NULL)
		(*result)->next = left;
	else if (right != NULL)
		(*result)->next = right;
}

void	merge_linked_list(t_list **head, t_list *left, t_list *right)
{
	t_list	*result;

	if (left == NULL)
	{
		*head = right;
		return ;
	}
	else if (right == NULL)
	{
		*head = left;
		return ;
	}
	if (ft_strcmp(left->content, right->content) <= 0)
	{
		*head = left;
		left = left->next;
	}
	else
	{
		*head = right;
		right = right->next;
	}
	result = *head;
	merge_remaining(&result, left, right);
}

void	split_list(t_list *head, t_list **left, t_list **right)
{
	t_list	*slow;
	t_list	*fast;

	slow = head;
	fast = head->next;
	while (fast != NULL)
	{
		fast = fast->next;
		if (fast != NULL)
		{
			slow = slow->next;
			fast = fast->next;
		}
	}
	*left = head;
	*right = slow->next;
	slow->next = NULL;
}

void	merge_sort_linkedlist(t_list **head)
{
	t_list	*current;
	t_list	*left;
	t_list	*right;

	current = *head;
	if (current == NULL || current->next == NULL)
		return ;
	split_list(current, &left, &right);
	merge_sort_linkedlist(&left);
	merge_sort_linkedlist(&right);
	merge_linked_list(head, left, right);
}
