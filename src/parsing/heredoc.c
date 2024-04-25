/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/24 20:01:09 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/24 20:47:52 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

# define BASE_HERE_FILENAME = "/tmp/minishell-"

char	*get_heredoc_filename(void)
{
	int		fd;
	char	*filename;

	filename = BASE_HERE_FILENAME;
	fd = -1;
	while (fd < 0)
	{
		fd = open(filename, O_RDWRITE);
	}
}

char	*read_heredoc(char *delimiter)
{
	int	has_delimiter;

	get_heredoc_filename();
}
