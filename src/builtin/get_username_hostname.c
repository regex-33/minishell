#include "../../inc/minishell.h"

char	*ft_path(void)
{
	char	*cwd;

	cwd = malloc(1024);
	if (!cwd)
		return (NULL);
	if (!getcwd(cwd, 1024))
	{
		free(cwd);
		return (NULL);
	}
	return (cwd);
}

int	excute_chile_procces(char *str, int pipefd[2])
{
	char		*temp;
	extern char	**environ;
	char		**args;

	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		return (perror("Dup2 failed"), 0);
	close(pipefd[1]);
	temp = malloc(ft_strlen(str) + sizeof("/usr/bin/") + 1);
	if (!temp)
		return (perror("minishell"), 0);
	if (!strcmp(str, "hostname"))
		ft_strcpy(temp, "/bin/");
	else
		ft_strcpy(temp, "/usr/bin/");
	ft_strcat(temp, str);
	args = malloc(sizeof(char *) * 2);
	args[0] = ft_strdup(str);
	args[1] = NULL;
	if (execve(temp, args, environ) == -1)
		return (perror("Execve failed"), 0);
	return (free(temp), 1);
}

int	excute_parrent_procces(pid_t pid, char *buffer, int pipefd[2])
{
	int	status;
	int	bytes_read;

	close(pipefd[1]);
	bytes_read = read(pipefd[0], buffer, sizeof(buffer));
	if (bytes_read == -1)
		return (perror("Read failed"), 0);
	buffer[bytes_read] = '\0';
	if (waitpid(pid, &status, 0) == -1)
		return (perror("Waitpid failed"), 0);
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		return (ft_putstr_fd("Command execution failed\n", 2), 0);
	close(pipefd[0]);
	if (buffer[bytes_read - 1] == '\n')
		buffer[bytes_read - 1] = '\0';
	return 1;
}

char	*get_user_host_name(char *str)
{
	int		pipefd[2];
	pid_t	pid;
	char	buffer[MAX_USERNAME_LENGTH];

	if (pipe(pipefd) == -1)
	{
		perror("Pipe creation failed");
		exit(EXIT_FAILURE);
	}
	pid = fork();
	if (pid == -1)
	{
		perror("Fork failed");
		exit(EXIT_FAILURE);
	}
	if (pid == 0)
	{
		if (!excute_chile_procces(str, pipefd))
			return (NULL);
	}
	else
		if (!excute_parrent_procces(pid, buffer, pipefd))
			return (NULL);
	return (ft_strdup(buffer));
}

char	*user_prompt(char *hostname, char *username, int is_prompt)
{
	char *pwd = NULL;
	int prompt_length;
	char *prompt;

	pwd = ft_path();
	prompt_length = strlen(username) + strlen(hostname) + strlen(pwd) + 110;
	prompt = (char *)malloc(prompt_length * sizeof(char));
	if (prompt == NULL)
		return (perror("minishell"), ft_strdup("minishell> "));
	strcpy(prompt, ANSI_COLOR_BOLD_BLUE "┌──(" ANSI_COLOR_BOLD_BLUE);
	strcat(prompt, username);
	strcat(prompt, ANSI_COLOR_RESET " ㉿" ANSI_COLOR_BOLD_BLUE);
	strcat(prompt, hostname);
	strcat(prompt, ")");
	strcat(prompt, ANSI_COLOR_RESET ANSI_COLOR_KHDER_FATH "-[" ANSI_COLOR_RESET ANSI_COLOR_BOLD_WHITE);
	strcat(prompt, pwd);
	strcat(prompt, ANSI_COLOR_RESET ANSI_COLOR_KHDER_FATH "]\n" ANSI_COLOR_RESET);
	if (!is_prompt)
		strcat(prompt, ANSI_COLOR_BOLD_BLUE "└─# " ANSI_COLOR_RESET);
	return (free(hostname), free(username), prompt);
}

char	*root_prompt(char *hostname, char *username, int is_prompt)
{
	char *pwd = NULL;
	int	prompt_length;
	char *prompt;

	pwd = ft_path();
	prompt_length = strlen(username) + strlen(hostname) + strlen(pwd) + 110;
	prompt = (char *)malloc(prompt_length * sizeof(char));
	if (prompt == NULL)
		return (perror("minishell"), ft_strdup("minishell> "));
	strcpy(prompt, ANSI_COLOR_BOLD_RED "┌──(" ANSI_COLOR_BOLD_RED "root" ANSI_COLOR_RESET " ㉿ " ANSI_COLOR_BOLD_RED);
	strcat(prompt, hostname);
	strcat(prompt, ")");
	strcat(prompt, ANSI_COLOR_RESET ANSI_COLOR_KHDER_FATH "-[" ANSI_COLOR_RESET ANSI_COLOR_BOLD_WHITE);
	strcat(prompt, pwd);
	strcat(prompt, ANSI_COLOR_RESET ANSI_COLOR_KHDER_FATH "]\n" ANSI_COLOR_RESET );
	if (!is_prompt)
		strcat(prompt, ANSI_COLOR_BOLD_RED "└─# " ANSI_COLOR_RESET);
	return (free(hostname), free(username) ,prompt);
}

char	*get_prompt_with_user_details(int is_prompt)
{
	char	*username;
	char	*hostname;

	hostname = get_user_host_name("hostname");
	if (!hostname)
		return (ft_strdup("minishell> "));
	username = get_user_host_name("whoami");
	if (!username)
		return (free(hostname), ft_strdup("minishell> "));
	if (!strcmp(username, "root"))
		return (root_prompt(hostname, username, is_prompt));
	else
		return (user_prompt(hostname, username, is_prompt));
	return (ft_strdup("minishell> "));
}
