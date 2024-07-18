/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 12:00:55 by bchanaa           #+#    #+#             */
/*   Updated: 2024/07/18 12:01:05 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	add_pipe(int pipes[2][2])
{
	int	tmp[2];

	if (pipe(tmp) == -1)
		return (-1);
	pipes[IN_PIPE][READ] = pipes[OUT_PIPE][READ];
	pipes[IN_PIPE][WRITE] = pipes[OUT_PIPE][WRITE];
	pipes[OUT_PIPE][READ] = tmp[READ];
	pipes[OUT_PIPE][WRITE] = tmp[WRITE];
	return (0);
}
