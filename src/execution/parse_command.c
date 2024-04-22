#include <minishell.h>

/*
t_parsed_command	*parse_command(char *input)
{
	t_parsed_command	*parsed_commands;
	char			*token;
	char			*command;
	char			*redirect_operator;
	char			*redirect_file;
	char			*redirect_token;
    char            *logical_operator;

    logical_operator = strstr(input, "||");
    if (logical_operator != NULL)
        logical_operator = strstr(input, "&&");
	parsed_commands = malloc(sizeof(t_parsed_command));
	if (!parsed_commands)
	{
		ft_putstr_fd("Memory allocation failed\n", STDERR_FILENO);
		return (NULL);
	}

	token = strtok(input, "||&&");
	if (token != NULL)
	{
		command = ft_strtrim(token, " \n\t\r\v\f");
		redirect_operator = NULL;
		redirect_file = NULL;
		//Check for redirection (> or >>) 
		redirect_token = strstr(command, ">");
		if (redirect_token != NULL)
		{
            if (*(redirect_token + 1) == '>')
            {
                redirect_operator = ">>";
			    redirect_file = ft_strtrim(redirect_token + 2, " \n\t\r\v\f");
            }
            else
            {
			    redirect_operator = ">";
			    redirect_file = ft_strtrim(redirect_token + 1, " \n\t\r\v\f");
            }
			*redirect_token = '\0';
		}
		// Store parsed command
		parsed_commands->command = ft_strdup(command);
		parsed_commands->redirect_operator = redirect_operator;
		parsed_commands->redirect_file = redirect_file;
        printf("Command : %s\n", parsed_commands->command);
        printf("Redirect Operator : %s\n", parsed_commands->redirect_operator);
        printf("Redirect File : %s\n", parsed_commands->redirect_file);
	}
	return (parsed_commands);
}*/

void	free_parsed_commands(t_parsed_command *parsed_commands)
{
	if (!parsed_commands)
		return ;
    if (parsed_commands->command)
        free(parsed_commands->command);
	free(parsed_commands);
}
