#include "minishell.h"

void	printArray(char **array)
{
	int i = 0;

	if (!array)
	{
		printf("Array is empty\n");
	}
	while (array[i])
	{
		ft_printf("%s\n", array[i]);
		i++;
	}
}

void	free_array(char **array)
{
	if (!array)
		return;
	for (size_t i = 0; array[i] != NULL; i++)
		free(array[i]);
	free(array);
}

char **ft_list_to_array(t_list *list)
{
    if (!list)
        return NULL;
    char **array = malloc(sizeof(char *) * (ft_lstsize(list) + 1));
    if (!array)
        return NULL;

    int i = 0;
    while (list)
    {
        array[i] = ft_strdup(list->content);
        if (!array[i])
        {
            perror("minishell");
            while (i > 0)
                free(array[i--]);
            free(array);
            return NULL;
        }
        list = list->next;
        i++;
    }
    array[i] = NULL;
    return array;
}