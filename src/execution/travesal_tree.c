/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   travesal_tree.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 16:19:15 by yachtata          #+#    #+#             */
/*   Updated: 2024/04/21 16:19:56 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
# define COMMAND 0
# define AND 1
# define OR 2

typedef enum
{
	COMMAND,
	AND,
	OR,
	PIPE
}					NodeType;

typedef struct TreeNode
{
	NodeType		type;
	void			*value;
	struct TreeNode	*left;
	struct TreeNode	*right;
	int  Redirect = 0;
}					TreeNode;

int	execute_command(TreeNode *node)
{
	char	*args[] = {node->value, NULL};
	int		pipefd[2];
	pid_t	pid_left;
	pid_t	pid_right;

	if (node == NULL)
		return (0);
	if (node->type == COMMAND)
	{
		// Execute the command
		execvp(node->value, args);
		perror("execvp");
		exit(EXIT_FAILURE);
	}
	else if (node->type == AND)
	{
		if (execute_command(node->left) == 0)
			return (execute_command(node->right));
		else
			return (1); 
	}
	else if (node->type == OR)
	{
		if (execute_command(node->left) != 0)
			return (execute_command(node->right));
		else
			return (0); 
	}
	else if (node->type == PIPE)
	{
		pipe(pipefd);
		pid_left = fork();
		if (pid_left == 0)
		{
			// Child process (left command)
			close(pipefd[0]);               // Close unused read end
			dup2(pipefd[1], STDOUT_FILENO); // Redirect stdout to pipe
			close(pipefd[1]);               // Close original stdout
			execute_command(node->left);    // Execute left command
			exit(EXIT_SUCCESS);
		}
		pid_right = fork();
		if (pid_right == 0)
		{
			// Child process (right command)
			close(pipefd[1]);              // Close unused write end
			dup2(pipefd[0], STDIN_FILENO); // Redirect stdin to pipe
			close(pipefd[0]);              // Close original stdin
			execute_command(node->right);  // Execute right command
			exit(EXIT_SUCCESS);
		}
		// Parent process
		close(pipefd[0]);            // Close unused read end
		close(pipefd[1]);            // Close unused write end
		waitpid(pid_left, NULL, 0);  // Wait for left command to finish
		waitpid(pid_right, NULL, 0); // Wait for right command to finish
	}
	return (0);
}
