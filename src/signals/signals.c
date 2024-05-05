#include "minishell.h"

void	handle_equal_signal(int signum, siginfo_t *info, void *ptr)
{
	(void)signum;
	(void)ptr;
	(void)info;
	rl_redisplay();
}

void	handle_interrupt(int sig)
{
	(void)sig;
	ft_printf("\n");
	rl_on_new_line();
	// rl_replace_line("", 1);
	rl_redisplay();
}