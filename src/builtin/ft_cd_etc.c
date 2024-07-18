/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_etc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:30:10 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 13:30:12 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_directory(const char *path)
{
	struct stat	path_stat;

	stat(path, &path_stat);
	return (S_ISDIR(path_stat.st_mode));
}

int	check_permission(const char *path)
{
	struct stat	path_stat;

	stat(path, &path_stat);
	return (path_stat.st_mode & S_IXUSR);
}

int	check_existence(const char *path)
{
	struct stat	path_stat;

	return (stat(path, &path_stat));
}

void	init_array(char **array, int size)
{
	int	i;

	i = 0;
	while (i < size)
	{
		array[i] = NULL;
		i++;
	}
}
