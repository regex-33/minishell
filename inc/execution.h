void	printArray(char **array);
void	free_array(char **array);
char **ft_list_to_array(t_list *list);
pid_t exec_cmd(t_list *redir_list, char **args, char ***env);
char *join_strings(char **args, int index);
pid_t	exec_piped_cmd(t_btree *leaf, char ***env, int pipes[2][2]);
pid_t	exec_last_piped_cmd(t_btree *leaf, char ***env, int fd[2]);
char **get_expanded_args(t_cmd *cmd, char **env);


void    restore_redir(t_list *redir_list);
int open_files(t_list *redir_list, char ***env);

