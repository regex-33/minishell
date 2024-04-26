void	printArray(char **array);
void	free_array(char **array);
char **ft_list_to_array(t_list *list);
pid_t exec_cmd(t_list *redir_list, char **args, char ***env);
char *join_strings(char **args, int index);