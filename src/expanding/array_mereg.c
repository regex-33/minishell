/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   array_mereg.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 15:52:21 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 15:52:22 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/*
void	merge(char **arr, int left, int mid, int right)
{
	int		n1;
	int		n2;
	char	**left_arr;
	char	**right_arr;
	int		i;
	int		j;
	int		k;

	i = 0;
	j = 0;
	n1 = mid - left + 1;
	n2 = right - mid;
	left_arr = (char **)malloc(n1 * sizeof(char *));
	right_arr = (char **)malloc(n2 * sizeof(char *));
	while (i < n1)
	{
		left_arr[i] = arr[left + i];
		i++;
	}
	while (j < n2)
	{
		right_arr[j] = arr[mid + 1 + j];
		j++;
	}
	i = 0;
	j = 0;
	k = left;
	while (i < n1 && j < n2)
	{
		if (strcmp(left_arr[i], right_arr[j]) <= 0)
			arr[k++] = left_arr[i++];
		else
			arr[k++] = right_arr[j++];
	}
	while (i < n1)
		arr[k++] = left_arr[i++];
	while (j < n2)
		arr[k++] = right_arr[j++];
	return (free(left_arr), free(right_arr));
}
*/
void	copy_subarray(char **arr, char **sub_arr, int start, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		sub_arr[i] = arr[start + i];
		i++;
	}
}

void	merge(char **arr, int left, int mid, int right)
{
	t_merge	merge;

	merge.i = 0;
	merge.j = 0;
	merge.left_size = mid - left + 1;
	merge.right_size = right - mid;
	merge.left_arr = allocate_new_environ(merge.left_size);
	merge.right_arr = allocate_new_environ(merge.right_size);
	if (!merge.left_arr || !merge.left_arr)
		return ;
	copy_subarray(arr, merge.left_arr, left, merge.left_size);
	copy_subarray(arr, merge.right_arr, mid + 1, merge.right_size);
	merge.k = left;
	while (merge.i < merge.left_size && merge.j < merge.right_size)
	{
		if (ft_strcmp(merge.left_arr[merge.i], merge.right_arr[merge.j]) <= 0)
			arr[merge.k++] = merge.left_arr[merge.i++];
		else
			arr[merge.k++] = merge.right_arr[merge.j++];
	}
	while (merge.i < merge.left_size)
		arr[merge.k++] = merge.left_arr[merge.i++];
	while (merge.j < merge.right_size)
		arr[merge.k++] = merge.right_arr[merge.j++];
	return (free(merge.left_arr), free(merge.right_arr));
}

void	merge_sort(char **arr, int left, int right)
{
	int	mid;

	if (left < right)
	{
		mid = left + (right - left) / 2;
		merge_sort(arr, left, mid);
		merge_sort(arr, mid + 1, right);
		merge(arr, left, mid, right);
	}
}

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
