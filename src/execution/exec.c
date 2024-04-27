
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 23:15:04 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/26 18:31:12 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_list	*expand_list(t_list *list, char **env)
{
	t_token	*token;
	char	*value;
	t_list	*expanding_list = NULL;

	if (!list)
		return (0);
	while (list)
	{
		token = list->content;
		value = ft_substr(token->value, 0, token->len);
		if (!value)
		{
			ft_printf("Error: failed to copy token\n");
			return (0);
		}
		if (!expand_arg_list(&expanding_list, value, env))
		{
			ft_printf("Error: failed to expand token\n");
			return (0);
		}
		free(value);
		list = list->next;
	}
	/* don't forget to memory */

	//printLinkedList(expanding_list);
	return (expanding_list);
}

char **get_expanded_args(t_cmd *cmd, char **env)
{
    t_list *list_args;
	t_list *expanding_list;

	if (!cmd)
		return NULL;
	
	list_args = cmd->cmd_args;
    expanding_list = expand_list(list_args, env);
    if (expanding_list)
    {
        char **args = ft_list_to_array(expanding_list);
        if (!args)
        {
            //freeLinkedList(expanding_list);
            return NULL;
        }
        //freeLinkedList(expanding_list);
		//printArray(args);
        //free_array(args);
		return args;
    }
	return NULL;
}

int exec_simple(t_btree *tree, char ***env)
{
	char **args;
	pid_t pid;

    if (!tree)
        return 0;

    t_cmd *cmd = tree->data;
	t_list	*redir = cmd->redir_list;

	args = get_expanded_args(cmd, *env);
	if (!args)
	{
		perror("minishell");
		return 0;
    }
	
	pid = exec_cmd(redir, args, env);
	if (pid < 0)
	{
		perror("minishell");
		return 0;
	}
	else if (pid == 1)
		return 1;
	else
		return 0;
	// else
	// {
	// 	return 1;
	// }

    return 0;
}

/*
int	exec_simple(t_btree *tree, char **env)
{
	t_token *token = NULL;
	t_list	*list_args;
	char	**args = NULL;
	t_list	*expanding_list = NULL;

	(void) env;
	(void) token;
	if (!tree)
		return (0);
	t_cmd *cmd = tree->data;
	list_args = cmd->cmd_args;
	t_list *redir_list = cmd->redir_list;
	(void)redir_list;

	ft_printf("EXEC: ");
	expanding_list = expand_list(list_args, env);
	if (expanding_list)
	{
		ft_list_to_array(list_args, &args);
		if (!args)
		{
			ft_printf("Error: failed to convert list to array\n");
			return (0);
		}
		//printLinkedList(expanding_list);
		freeLinkedList(expanding_list);
		if (args)
		{
			printf("args: %lu\n", sizeof(args) / sizeof(args[0]));
			printArray(args);
		}
		return (1);
	}
	// while (list)
	// {
	// 	token = list->content;
	// 	write(1, token->value, token->len);
	// 	ft_printf("\n");
	// 	list = list->next;
	// }
	// write(1, token->value, token->len);
	// ft_printf(" ");
	// token = list_args->content;
	// if (ft_strnstr("true", "true", 4))
		//return (1);
	return (0);
}*/

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

int exec_pipe(t_btree *tree, char ***env, int pipes[2][2], int is_root)
{
	if (!tree)
		return (0);
	if (tree->left->type == nt_simplecmd)
	{
		add_pipe(pipes);
		exec_piped_cmd(tree->left, env, pipes);
	}
	else
		exec_pipe(tree->left, env, pipes, 0);
	if (!is_root)
	{
		add_pipe(pipes);
		return (exec_piped_cmd(tree->right, env, pipes));
	}
	else
		return (exec_last_piped_cmd(tree->right, env, pipes[OUT_PIPE]));
}

int exec_and_or(t_btree *tree, char ***env)
{
	int first_res;
	first_res = 0;

	if (!tree)
		return (0);
	first_res = __exec(tree->left, env);
	if (first_res)
	{
		if (tree->type == nt_or_if)
		{
			return __exec(tree->right, env);
		}
		else
			return (0);
	}
	if (!first_res)
	{
		if (tree->type == nt_and_if)
		{
			return (__exec(tree->right, env));
		}
		else
			return (0);
	}
	return (0);
}

int	exec_sub(t_btree *tree, char ***env)
{
	(void)env;
	if (!tree)
		return (1);
	return (__exec(tree->left, env));
}

int __exec(t_btree *tree, char ***env)
{
	int pipes[2][2];

	pipes[OUT_PIPE][READ] = STDIN_FILENO;
	pipes[OUT_PIPE][WRITE] = STDOUT_FILENO;
	if (!tree)
		return (0);
	if (tree->type == nt_pipe)
	{
		pid_t	pid;
		int status;
		pid = exec_pipe(tree, env, pipes, 1);
		if (pid < 0)
			return (1);
		waitpid(pid, &status, 0);
		while (wait(NULL) > 0);
		if (WIFEXITED(status))
		{
			ft_printf("Pipe status: %d\n", (WEXITSTATUS(status)));
			return WEXITSTATUS(status);
		}
			
		else if (WIFSIGNALED(status))
		{
			ft_printf("Pipe status signaled: %d\n", (128 + WTERMSIG(status)));
			return (128 + WTERMSIG(status));
		}
	}
	else if (tree->type == nt_and_if)
		return exec_and_or(tree, env);
	else if (tree->type == nt_or_if)
		return exec_and_or(tree, env);
	else if (tree->type == nt_subcmd)
		return exec_sub(tree, env);
	else
		return exec_simple(tree, env);
	return (0);
}
