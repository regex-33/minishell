/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/17 13:30:17 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/17 13:30:18 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

bool	is_numeric(const char *str)
{
	if (*str == '+' || *str == '-')
		str++;
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (false);
		str++;
	}
	return (true);
}

int	free_and_exit(int code, char **args, t_context *ctx)
{
	clear_btree(ctx->parse_tree);
	ft_lstclear_libft(&ctx->tokens, free);
	free_array(ctx->env);
	free_array(args);
	free(ctx->last_pwd);
	return (exit(code), code);
}

int	ft_exit(char **exit_code, t_context *ctx)
{
	unsigned char	code;

	if (!exit_code || !exit_code[1])
		return (ft_putendl_fd("exit", 2), free_and_exit(get_status(0, 0),
				exit_code, ctx), 0);
	if (exit_code && count_array(exit_code) > 2)
		return (ft_putstr_fd("minishell: exit : too many arguments\n", 2), 1);
	if (!is_numeric(exit_code[1]))
	{
		ft_putendl_fd("exit", 2);
		ft_putstr_fd("minishell: exit : ", 2);
		ft_putstr_fd(exit_code[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		free_and_exit(2, exit_code, ctx);
	}
	if (exit_code && count_array(exit_code) > 2)
		return (ft_putstr_fd("minishell: exit : too many arguments\n", 2), 1);
	code = (unsigned char)ft_atoi(exit_code[1]);
	ft_putendl_fd("exit", 2);
	return (free_and_exit(code, exit_code, ctx), code);
}
