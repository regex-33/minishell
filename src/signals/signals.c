/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/18 11:11:07 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/18 11:11:08 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	my_putchar(int c)
{
	return (write(1, &c, 1));
}

void	handle_interrupt(int sig)
{
	(void)sig;
	get_status(130, SET_STATUS);
	rl_replace_line("", 1);
	ft_printf("\n");
	if (get_state(0, 0) == ON_PROMPT)
	{
		rl_on_new_line();
		rl_replace_line("", 1);
		rl_redisplay();
	}
}

void	handle_quit(int sig)
{
	(void)sig;
	if (get_state(0, 0) == ON_EXEC)
		ft_putendl_fd("Quit: (core dumped)", 1);
	else
	{
		rl_on_new_line();
		rl_redisplay();
	}
}
