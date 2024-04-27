#include "minishell.h"

void	printLinkedList(t_list *head)
{
	t_list	*current;

	current = head;
	ft_printf("\nLinked List: \n");
	while (current != NULL)
	{
		ft_printf("%s ", (char *)(current->content));
		current = current->next;
	}
}

void	freeLinkedList(t_list *head)
{
	t_list	*current;
	t_list	*next;

	current = head;
	while (current != NULL)
	{
		next = current->next;
		free(current->content);
		free(current);
		current = next;
	}
	head = NULL;
}

int	contains_wildcard(const char *str)
{
	return (ft_strchr(str, '*') != NULL);
}
/*  check match filename    */
// mi*shell
int	match_wildcard(const char *pattern, const char *filename)
{
	while (*pattern)
	{
		if (*pattern == '*')
		{
			while (*pattern == '*')
				pattern++;
			/*'*' at the end matches any remaining characters*/
			if (*pattern == '\0')
				return (1);
			while (*filename)
			{
				if (match_wildcard(pattern, filename))
					return (1);
				filename++;
			}
			return (0);
		}
		else if (*filename == '\0' || (*pattern != '?'
				&& *pattern != *filename)) // chars between * and your char
			return (0);
		pattern++;
		filename++;
	}
	return (*filename == '\0' && *pattern == '\0');
}
/*
The	opendir(void) function in C is used to open a directory stream corresponding
to the directory named by a given path. It returns a pointer to a DIR stream,
which can then be used to read the contents of the directory using functions
like	readdir(void). This function is part of the <dirent.h> header and is commonly
 used for directory traversal and file listing operations.*/


int	move_temp_list_to_list(t_list **list, t_list **temp)
{
	t_list	*new;

	merge_sort_linkedlist(temp);
	while (*temp != NULL)
	{
		new = ft_lstnew(ft_strdup((*temp)->content));
		if (!new)
		{
			perror("lstnew failed\n");
			return (0);
		}
		ft_lstadd_back_libft(list, new);
		free((*temp)->content);
		free(*temp);
		*temp = (*temp)->next;
	}
	return (1);
}


int	expand_wildcard(const char *pattern, t_list **matches)
{
	DIR				*dir;
	struct dirent	*entry;
	t_list			*temp = NULL;
	t_list			*new;

	dir = opendir(".");
	if (dir == NULL)
	{
		perror("Error opening directory");
		return (0);
	}
	while ((entry = readdir(dir)) != NULL)
	{
		if (match_wildcard(pattern, entry->d_name))
		{
			//  don't forget to free memory 
			if (entry->d_name[0] == '.')
				continue;
			new = ft_lstnew(ft_strdup(entry->d_name));
			if (!new)
			{
				perror("lstnew failed\n");
				closedir(dir);
				return (0);
			}
			ft_lstadd_back_libft(&temp, new);
		}
	}
	if (!temp)
	{
		new = ft_lstnew(ft_strdup(pattern));
		if (!new)
		{
			perror("lstnew failed\n");
			closedir(dir);
			return (0);
		}
		ft_lstadd_back_libft(&temp, new);
	}
	closedir(dir);
	return (move_temp_list_to_list(matches, &temp));
}



void	*expand_asterisk(char *command, t_list **list)
{
	char	*token;
	t_list	*new;
//	t_list	**current;

	token = strtok(command, " \t");
	while (token != NULL)
	{
		if (contains_wildcard(token))
		{
			/* don't forget to free memory if is failed */
			if(!expand_wildcard(token, list))
			{
				printf("expand_wildcard failed\n");
				return (NULL);
			}
		}
		else
		{
			new = ft_lstnew(ft_strdup(token));
			if (!new)
			{
				perror("lstnew failed\n");
				return (NULL);
			}
			ft_lstadd_back_libft(list, new);
		}
		token = strtok(NULL, " ");
	}
	return (list);
}
