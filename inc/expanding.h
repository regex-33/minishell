/*  asterisk functions */

void	printLinkedList(t_list *head);
void	freeLinkedList(t_list *head);
int	contains_wildcard(const char *str);
int	match_wildcard(const char *pattern, const char *filename);
t_list	**expand_wildcard(const char *pattern, t_list **matches);
void	*execute_command2(char *command, t_list **list);

/*  single duble dollar */

char	*extract_dollar(const char *str);
//char	*get_value(char *name);
int is_valid_chars(char c);
char	*extract_dollar(const char *str);
char	*ft_strjoin_free(char *s1, const char *s2);
char *ft_strndup(const char *src, size_t n);
char	*ft_echo_process(char *temp, char **env);

/*  mereg sort  */

void	merge(char **arr, int left, int mid, int right);
void	merge_sort(char **arr, int left, int right);

