#include "../../inc/minishell.h"


char	*ft_path(void)
{
	char *cwd;

	cwd = malloc(1024);
	if (!cwd)
	{
		printf("cwd malloc error\n");
		return (NULL);
	}
	if (!getcwd(cwd, 1024))
	{
		perror("getcwd failed");
		free(cwd);
		return (NULL);
	}
	return (cwd);
}

void	excute_chile_procces(char *str, int pipefd[2])
{
	char	*temp;

	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("Duplication failed");
		exit(EXIT_FAILURE);
	}
	close(pipefd[1]);
	temp = malloc(ft_strlen(str) + sizeof("/usr/bin/") + 1);
	if (!temp)
	{
		printf("temp allocat failed\n");
		exit(EXIT_FAILURE);
	}
	ft_strcpy(temp, "/usr/bin/");
	ft_strcat(temp, str);
	if (execve(temp, NULL, NULL) == -1)
	{
		perror("Execution failed");
		exit(EXIT_FAILURE);
	}
	free(temp);
}

void	excute_parrent_procces(pid_t pid, char *buffer, int pipefd[2])
{
	int	status;
	int	bytes_read;

	close(pipefd[1]);
	bytes_read = read(pipefd[0], buffer, sizeof(buffer));
	if (bytes_read == -1)
	{
		perror("Reading from pipe failed");
		exit(EXIT_FAILURE);
	}
	buffer[bytes_read] = '\0';
	if (waitpid(pid, &status, 0) == -1)
	{
		perror("Waitpid failed");
		exit(EXIT_FAILURE);
	}
	if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
	{
		ft_putstr_fd("Command execution failed\n", 2);
		exit(EXIT_FAILURE);
	}
	close(pipefd[0]);
	if (buffer[bytes_read - 1] == '\n')
		buffer[bytes_read - 1] = '\0';
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
		excute_chile_procces(str, pipefd);
	else
		excute_parrent_procces(pid, buffer, pipefd);
	return (ft_strdup(buffer));
}

void	print_prompt_with_user_details(void)
{
	char *username;
	char *hostname;
	char *pwd;

	username = get_user_host_name("whoami");
	hostname = get_user_host_name("hostname");
	pwd = ft_path();
	if (!ft_strcmp(username, "root"))
	{
		ft_printf(COLOR_BOLD_RED "┌──(" COLOR_BOLD_RED "root" COLOR_RESET "㉿" COLOR_BOLD_RED "%s)" COLOR_RESET COLOR_KHDER_FATH "-[" COLOR_RESET COLOR_BOLD_WHITE "%s" COLOR_RESET COLOR_KHDER_FATH "]\n" COLOR_RESET,
			hostname, pwd);
		ft_printf(COLOR_BOLD_RED "└─# " COLOR_RESET);
	}
	else
	{
		ft_printf(COLOR_BOLD_BLUE "┌──(" COLOR_BOLD_BLUE "%s" COLOR_RESET "㉿" COLOR_BOLD_BLUE "%s)" COLOR_RESET COLOR_KHDER_FATH "-[" COLOR_RESET COLOR_BOLD_WHITE "%s" COLOR_RESET COLOR_KHDER_FATH "]\n" COLOR_RESET,
			username, hostname, pwd);
		ft_printf(COLOR_BOLD_BLUE "└─$ " COLOR_RESET);
	}
	free(username);
	free(hostname);
	free(pwd);
}
/*
char	*get_user_host_name(char *str)
{
	int		pipefd[2];
	pid_t	pid;
	char	buffer[MAX_USERNAME_LENGTH];
	int		status;
	char *args[] = {"whoami", NULL};
	ssize_t	bytes_read;
	char	*username;

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
		close(pipefd[0]);
		if (dup2(pipefd[1], STDOUT_FILENO) == -1)
		{
			perror("Duplication failed");
			exit(EXIT_FAILURE);
		}
		close(pipefd[1]);
		char *temp = malloc(ft_strlen(str) + sizeof("/usr/bin/") + 1);
		if (!temp)
		{
			printf("temp allocat failed\n");
			exit(EXIT_FAILURE);
		}
		ft_strcpy(temp, "/usr/bin/");
		ft_strcat(temp, str);
		if (execve(temp, args, NULL) == -1)
		{
			perror("Execution failed");
			exit(EXIT_FAILURE);
		}
		free(temp);
	}
	else
	{
		close(pipefd[1]);
		bytes_read = read(pipefd[0], buffer, sizeof(buffer));
		if (bytes_read == -1)
		{
			perror("Reading from pipe failed");
			exit(EXIT_FAILURE);
		}
		buffer[bytes_read] = '\0';
		if (waitpid(pid, &status, 0) == -1)
		{
			perror("Waitpid failed");
			exit(EXIT_FAILURE);
		}
		if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
		{
			ft_putstr_fd("Command execution failed\n", 2);
			exit(EXIT_FAILURE);
		}
		close(pipefd[0]);
		if (buffer[bytes_read - 1] == '\n')
			buffer[bytes_read - 1] = '\0';
		username = malloc(bytes_read);
		if (!username)
		{
			perror("Memory allocation failed");
			exit(EXIT_FAILURE);
		}
		ft_strcpy(username, buffer);
	}
	return (username);
}*/