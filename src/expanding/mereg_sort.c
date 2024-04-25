#include "minishell.h"

void	merge(char **arr, int left, int mid, int right)
{
	int		n1;
	int		n2;
	char	**left_arr;
	char	**right_arr;

	int i, j, k;
	n1 = mid - left + 1;
	n2 = right - mid;
	// Create temporary arrays
	left_arr = (char **)malloc(n1 * sizeof(char *));
	right_arr = (char **)malloc(n2 * sizeof(char *));
	// Copy data to temporary arrays left_arr[] and right_arr[]
	for (i = 0; i < n1; i++)
		left_arr[i] = arr[left + i];
	for (j = 0; j < n2; j++)
		right_arr[j] = arr[mid + 1 + j];
	// Merge the temporary arrays back into arr[left..right]
	i = 0;    // Initial index of first subarray
	j = 0;    // Initial index of second subarray
	k = left; // Initial index of merged subarray
	while (i < n1 && j < n2)
	{
		if (strcmp(left_arr[i], right_arr[j]) <= 0)
		{
			arr[k++] = left_arr[i++];
		}
		else
		{
			arr[k++] = right_arr[j++];
		}
	}
	// Copy the remaining elements of left_arr[], if any
	while (i < n1)
	{
		arr[k++] = left_arr[i++];
	}
	// Copy the remaining elements of right_arr[], if any
	while (j < n2)
	{
		arr[k++] = right_arr[j++];
	}
	// Free the temporary arrays
	free(left_arr);
	free(right_arr);
}

void	merge_sort(char **arr, int left, int right)
{
	int	mid;

	if (left < right)
	{
		mid = left + (right - left) / 2;
		// Sort first and second halves
		merge_sort(arr, left, mid);
		merge_sort(arr, mid + 1, right);
		// Merge the sorted halves
		merge(arr, left, mid, right);
	}
}

