/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yachtata <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/19 09:04:59 by yachtata          #+#    #+#             */
/*   Updated: 2024/07/19 09:05:00 by yachtata         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXECUTION_H
# define EXECUTION_H

# include "types.h"

/*          pipes         */

int		add_pipe(int pipes[2][2]);
int		exec_pipe(t_btree *tree, t_context *ctx, int pipes[2][2], int is_root);
int		exec_sub(t_btree *tree, t_context *ctx);
pid_t	exec_piped_cmd(t_btree *tree, t_context *ctx, int pipes[2][2]);
pid_t	exec_last_piped_cmd(t_btree *tree, t_context *ctx, int fd[2]);

/*         etc functions      */

char	*join_strings(char **args, int index);
void	print_array(char **array);
void	free_array(char **array);
int		count_array(char **array);
char	**ft_list_to_array(t_list *list, char **array, int i);

/*         exec         */

t_list	*expand_list(t_list *list, t_context *ctx);
char	**get_expanded_args(t_cmd *cmd, t_context *ctx);
int		exec_simple(t_btree *tree, t_context *ctx);
int		exec_and_or(t_btree *tree, t_context *ctx);
int		__exec(t_btree *tree, t_context *ctx);

/*         execut cmd         */

int		exec_cmd(t_list *redir_list, char **args, t_context *ctx);
void	print_fd_err(int fd);
char	**get_cmd_args(char **cmd_args, char **path_dirs);
int		init_command(t_prexec *pexec, t_context *ctx, char **args);
int		check_abs_cmd(char *pathname);
void	print_err(char *mid, char *suffix);
char	*ft_which(char *cmd, char **path_dirs);
int		is_file(char *path);

/*          heredoc         */

char	**expand_filename_here_doc(char *filename, t_context *ctx, int flag);
int		read_and_expand_heredoc(int old_fd, int new_fd, t_context *ctx);
int		handle_heredoc(char **filename, t_context *ctx, char *delimiter);

/*          redirections    */

void	reset_redir(t_list *redir_list, int restore);
int		open_file(char *file_name, t_redir *redir, t_context *ctx,
			t_list *redir_list);
int		redirect(t_list *redir_list, t_context *ctx);

/*  init command */
bool	add_to_list(t_list **list, char *split_value);

#endif
