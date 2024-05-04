#include "minishell.h"

char *join_strings(char **args, int index)
{
	char *str = NULL;

	while (args[index])
	{
		str = ft_strjoin_free(str, args[index]);
        if (!str)
            return NULL;
        if (!args[index + 1])
            return str;
		str = ft_strjoin_free(str, " ");
        if (!str)
            return NULL;
		index++;
	}
	return (str);
}

void	printArray(char **array)
{
	int i = 0;
    ft_printf("printArray\n");
	if (!array)
	{
		printf("Array is empty\n");
	}
    //printf("Array is not empty\n");
	while (array[i])
	{
		ft_printf("str :%s$\n", array[i]);
		i++;
	}
    ft_printf("-----------------\n");
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
        if (!list->content) 
        {
           // array[i] = NULL;
            list = list->next;
            continue;
            //array[i] = strdup("");
        }
        else
            array[i] = ft_strdup(list->content);
        //printf("array[%d] = %s\n", i, array[i]);
        if (!array[i])
        {
            printf("malloc error\n");
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
