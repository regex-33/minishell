/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 23:15:04 by bchanaa           #+#    #+#             */
/*   Updated: 2024/04/22 17:20:19 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

typedef enum e_redir_type
{
	REDIR_OUT,
	REDIR_OUT_APPEND,
	REDIR_IN,
	REDIR_HERE_DOC,
}			t_redir_type;


typedef struct	s_redir
{
	char			*delimeter;
	char			*filename;
	t_redir_type	type;
	int				fd;
}				t_redir;

typedef struct s_cmd
{
	char	**args;
	t_list	*redir_list;
}				t_cmd;

int get_count_without_redir(t_list *list)
{
	int count;
	t_list *temp;

	temp = list;
	count = ft_lstsize(temp);
	while(temp)
	{
		if (temp->content->type == tok_redir)
			count -= 2;
		temp = temp->next;
	}
	return (count);
}

int	edit_cmd(t_list *list)
{
	int count;
	t_parsed_command *cmd;
	char *find_redir;
	char *temp;
	int i;


	i = 0;
	cmd = malloc(sizeof(t_parse_command));
	if (!cmd)
		return (0);

	count = get_count_without_redir(list);
	if (!count)
		return (0);

	cmd->command = malloc(sizeof(char) * count + 1);
	if (!cmd->command)
		return (0);
	cmd->command[count] = NULL;

	while(list)
	{
		if (list->content->type == tok_redir)
		{
			t_redir_list *new_redir = malloc(sizeof(t_redir_list));
			if (!new_redir)
			{
				//free_parse_command(cmd);
				return (0);
			}
			new_redir->next = NULL;
			new_redir->type = //check the type of redirection
			list = list->next;
			new_redir->filename = ft_substr(list->content, 0, list->content->len);
			new_redir->fd = //check the file descriptor to use is in (this)>
			while(list->content->type != tok_redir)
			{
				temp = ft_substr(list->content, 0, list->content->len);
				cmd->command[i] = temp;
				i++;
				list = list->next;
			}
			//here we need to add the redirection to the list  cmd->redir_list
			
		}
		else
		// {
		// 		temp = ft_substr(list->content, 0, list->content->len);
		// 		cmd->command[i] = temp;
		// 		i++;
		// 		list = list->next;
		}
		i++;
	}	
	return (1);
}

char	*ft_which(char *cmd, char **path_dirs)
{
	char	*tmp;
	char	*cmd_pathname;
	int		i;

	if (ft_strchr(cmd, '/'))
	{
		if (!access(cmd, F_OK) && !access(cmd, X_OK))
			return (ft_strdup(cmd));
		return (NULL);
	}
	tmp = ft_strjoin("/", cmd);
	if (!tmp)
		return (NULL);
	i = -1;
	while (path_dirs[++i])
	{
		cmd_pathname = ft_strjoin(path_dirs[i], tmp);
		if (!cmd_pathname)
			return (free(tmp), NULL);
		if (!access(cmd_pathname, F_OK) && !access(cmd_pathname, X_OK))
			return (free(tmp), cmd_pathname);
		free(cmd_pathname);
	}
	return (free(tmp), NULL);
}

char	**get_cmd_args(char	*cmd, char **path_dirs)
{
	char	*cmd_pathname;
	char	**cmd_args;

	if (!cmd || !path_dirs)
		return (NULL);
	cmd_args = ft_split(cmd, ' ');
	if (!cmd_args)
		return (perror("minishell"), NULL);
	cmd_pathname = ft_which(cmd_args[0], path_dirs);
	//printf("CMD PATHNAME: %s, cmd : %s\n", cmd_pathname, cmd);
	if (!cmd_pathname)
	{
		if (!ft_strchr(cmd_args[0], '/'))
		{
			ft_putstr_fd(CMD_NOT_FOUND, STDERR_FILENO);
			ft_putendl_fd(cmd_args[0], STDERR_FILENO);
			return (ft_free_arr(cmd_args), NULL);
		}
		return (ft_free_arr(cmd_args), perror("minishell"), NULL);
	}
	free(cmd_args[0]);
	cmd_args[0] = cmd_pathname;
	return (cmd_args);
}

pid_t exec_cmd(char *cmd, char **path_dirs)
{
    char **cmd_args;
    pid_t pid;

    pid = fork();
    if (pid < 0)
    {
        perror("Fork failed");
        return -1;
    }
    if (pid == 0)
    {
        cmd_args = get_cmd_args(cmd, path_dirs);
        if (!cmd_args)
            exit(EXIT_FAILURE);
		printf("CMD ARGS: %s ==> args : %s\n", cmd_args[0], cmd_args[1]);
        execve(cmd_args[0], cmd_args, NULL);
        perror("Execve failed");
        exit(EXIT_FAILURE);
    }
    else
    {
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status))
        {
            int exit_status = WEXITSTATUS(status);
            if (exit_status != 0)
            {
                ft_printf("Command execution failed with status %d\n", exit_status);
				return 0;
            }
        }
        return pid;
    }
}



int	exec_simple(t_btree *tree, char **path_dirs)
{
	t_token *token;
	t_list	*list;
 	char **temp;
	
	(void)path_dirs;
	cmd = malloc(sizeof(t_parsed_command));
	if (!cmd)
		return (0);
	cmd->redirect_operator = NULL;
	cmd->redirect_file = NULL;
	cmd->command = NULL;
	if (!tree)
		return (0);
	list = tree->data;
	token = list->content;

	edit_cmd(list, cmd);
	temp = cmd->command;
	int i = 0;
	//printf("CMD: %s\n", cmd->command);
	while(temp[i])
	{
		printf("CMD: %s\n", temp[i]);
		i++;
	}
	printf("REDIRECT OPERATOR: %s\n", cmd->redirect_operator);
	printf("REDIRECT FILE: %s\n", cmd->redirect_file);
	//token = list->content;
	
	// printf("number of nodes: %d\n", ft_lstsize(list));
	// while(list)
	// {
	// 	// if (strcmp)
	// 	// printf("TOKEN: %s\n", list->content);
	// 	token = list->content;
	// 	write(1, token->value, token->len);
	// 	write(1, "\n", 1);
	// 	list = list->next;
	// }
	// printf("number of nodes: %d\n", ft_lstsize(list));
	//ft_printf("EXEC: ");
	// if (token->type == tok_literal)
	//ft_printf("first command : %s\n", token->valu);
	//write(1, token->value, token->len);
	//ft_printf(" ");
	// cmd = parse_command(token->value);
	// printf("CMD: %s\n", cmd->command);
	// if (exec_cmd(token, path_dirs))
	// 	return (1);
	// ft_printf("EXEC: ");
	// ft_printf("%s", token->value);
	// ft_printf(" ");
	if (ft_strnstr(token->value, "true", 4))
		return (1);
	return (0);
}




int exec_pipe(t_btree *tree, char **path_dirs)
{
	int	first_res;
	first_res = __exec(tree->left, path_dirs);
	ft_printf("PIPED to ");
	return __exec(tree->right, path_dirs);
}
int exec_and_or(t_btree *tree, char **path_dirs)
{
    if (!tree)
        return 0;

    int first_res = __exec(tree->left, path_dirs);

    if (tree->type == nt_or_if)
    {
        if (first_res == 0) // Previous command succeeded, execute next command
            return __exec(tree->right, path_dirs);
        else
            return 1;
    }
    else if (tree->type == nt_and_if)
    {
        if (first_res != 0)
            return __exec(tree->right, path_dirs);
        else
            return 0; // Previous command succeeded, short-circuit
    }

    return 0;
}



int __exec(t_btree *tree, char **path_dirs)
{
	if (!tree)
		return (0);
	if (tree->type == nt_pipe)
		return exec_pipe(tree, path_dirs);
	else if (tree->type == nt_and_if || tree->type == nt_or_if)
		return exec_and_or(tree, path_dirs);
	else
		return exec_simple(tree, path_dirs);
	return (0);
}
// int exec_and_or(t_btree *tree, char **path_dirs)
// {
// 	int first_res;

// 	if (!tree)
// 		return (0);
// 	first_res = __exec(tree->left, path_dirs);
// 	if (first_res)
// 	{
// 		printf("i am here, AND\n");
// 		if (tree->type == nt_and_if)
// 		{
// 			ft_printf(" AND ");
// 			return __exec(tree->right, path_dirs);
// 		}
// 		else
// 			return (0);
// 	}
// 	if (!first_res)
// 	{
// 		printf("i am here OR1\n");
// 		if (tree->type == nt_or_if)
// 		{
// 			ft_printf(" OR ");
// 			return (__exec(tree->right, path_dirs));
// 		}
// 		else
// 			return (0);
// 	}
// 	return (0);
// }
/*
int	exec_pipe(t_btree *tree, char **path_dirs)
{
	int	first_res;

	if (!tree || !tree->left || !tree->right)
		return (0);
	first_res = __exec(tree->left, path_dirs);
	if (first_res != 0)
		return (first_res);
	return (__exec(tree->right, path_dirs));
}

int	exec_and_or(t_btree *tree, char **path_dirs)
{
	int	first_res;

	if (!tree || !tree->left || !tree->right)
		return (0);
	first_res = __exec(tree->left, path_dirs);
	if (tree->type == nt_and_if)
		return (first_res && __exec(tree->right, path_dirs));
	else if (tree->type == nt_or_if)
		return (first_res || __exec(tree->right, path_dirs));
	return (0);
}

int	__exec(t_btree *tree, char **path_dirs)
{
	if (!tree)
		return (0);
	if (tree->type == nt_pipe)
		return (exec_pipe(tree, path_dirs));
	else if (tree->type == nt_and_if || tree->type == nt_or_if)
		return (exec_and_or(tree, path_dirs));
	else
		return (exec_simple(tree, path_dirs));
}*/
