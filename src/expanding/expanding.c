#include "minishell.h"

void	handle_regular_char(char *temp, char **join, int *i)
{
	char	*char_str;

	char_str = ft_strndup(temp + *i, 1);
	*join = ft_strjoin_free(*join, char_str);
	free(char_str);
	(*i)++;
}

char	*handle_dollar_sign(t_list **list, int *i, char *temp,
		t_expanding *expanding)
{
	char	*variable;
	char	*value;

	variable = extract_dollar(temp + *i);
	if (variable)
	{
		value = get_value(variable + 1, expanding->ctx->env);
		if (!value && expanding->quote == '\"')
			value = ft_strdup("");
		if (value && !ft_strchr(value, '\"') && ft_strchr(value, '*'))
			expanding->have_asterisk = 1;
		if (value && expanding->quote != '\"' && (ft_strchr(value, ' ')
				|| ft_strchr(value, '\t')))
		{
			if (!split_and_add_to_list(list, expanding, value))
            {
                *i += ft_strlen(variable);
                free(variable);
				return (NULL);
            }
			//printf("expanding->join: %s\n", expanding->join);
		}
		else
			expanding->join = ft_strjoin_free(expanding->join, value);
		*i += ft_strlen(variable);
		free(variable);
	}
	return (temp);
}

int	init_expanding(t_expanding *expanding, char *temp, t_context *ctx)
{
	if (!temp)
		return (0);
	expanding->in_quotes = 0;
	expanding->in_single_quotes = 0;
	expanding->have_asterisk = 0;
	expanding->quote = '\0';
	expanding->join = NULL;
	expanding->ctx = ctx;
	return (1);
}

void	handle_quotes_asterisk(t_expanding *expanding, char c, int *i)
{

	if (c == '"' && !expanding->in_single_quotes)
	{
		if (expanding->in_quotes)
			expanding->quote = '\0';
		else
		{
			expanding->quote = '"';
			expanding->join = ft_strjoin_free(expanding->join, "");
		}
		expanding->in_quotes = !expanding->in_quotes;
	}
	if (c == '\'' && !expanding->in_quotes)
	{
		if (expanding->in_single_quotes)
			expanding->quote = '\0';
		else
		{
			expanding->quote = '\'';
			expanding->join = ft_strjoin_free(expanding->join, "");
		}
		expanding->in_single_quotes = !expanding->in_single_quotes;
	}
	(*i)++;
}

t_list	**expand_arg_list(t_list **list, char *temp, t_context *ctx)
{
	t_expanding	expanding;
	int			i;

	i = 0;
	if (!init_expanding(&expanding, temp, ctx))
		return (NULL);
	while (temp[i])
	{
		if (temp[i] == '*' && expanding.in_quotes == 0 && expanding.in_single_quotes == 0)
			expanding.have_asterisk = 1;
		if ((temp[i] == '"' && !expanding.in_single_quotes) || (temp[i] == '\''
				&& !expanding.in_quotes))
		{
			handle_quotes_asterisk(&expanding, temp[i], &i);
			continue ;
		}
		if (temp[i] == '$' && is_spcial_chars(temp[i + 1])
			&& expanding.quote != '\'')
		{
			handle_dollar_sign(list, &i, temp, &expanding);
			continue ;
		}
		handle_regular_char(temp, &expanding.join, &i);
	}
	if (expanding.have_asterisk)
		return (expand_asterisk(expanding.join, list), list);
    // if (!expanding.join)
    //     	return (list);
	return (ft_lstadd_back_libft(list, ft_lstnew(expanding.join)), list);
}
