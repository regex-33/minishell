#ifndef EXECUTION_H
#define EXECUTION_H

#include "types.h"

void	printArray(char **array);
void	free_array(char **array);
char **ft_list_to_array(t_list *list);
pid_t exec_cmd(t_list *redir_list, char **args, t_context *ctx);
char *join_strings(char **args, int index);
pid_t	exec_piped_cmd(t_btree *leaf, t_context *ctx, int pipes[2][2]);
pid_t	exec_last_piped_cmd(t_btree *leaf, t_context *ctx, int fd[2]);
char **get_expanded_args(t_cmd *cmd, t_context *ctx);


void    restore_redir(t_list *redir_list);
int redirect(t_list *redir_list, t_context *ctx);
char **expand_filename_here_doc(char *filename, t_context *ctx);
int count_array(char **array);

#endif