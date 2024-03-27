/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bchanaa <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/05 17:49:51 by bchanaa           #+#    #+#             */
/*   Updated: 2024/03/18 00:19:49 by bchanaa          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/pipex_bonus.h"

/*
 * Initializes the args struct.
 * Returns 0 on success, or 1 on error.
*/

int	init_args(int ac, char **av, char **env, t_args *args)
{
	char	*path_env;

	if (ac < 5)
		return (ft_putendl_fd(INVALID_ARGS_STR, 2), 1);
	args->is_heredoc = (ft_strlen(av[1]) == 8 && \
						!ft_strncmp("here_doc", av[1], 8));
	if (args->is_heredoc && ac < 6)
		return (ft_putendl_fd(INVALID_ARGS_STR, 2), 1);
	path_env = ft_getenv("PATH", env);
	if (!path_env)
		path_env = "";
	args->path_dirs = ft_split(path_env, ':');
	if (!args->path_dirs)
		return (perror(PROG_NAME), 1);
	args->ac = ac;
	args->av = av;
	args->env = env;
	if (args->is_heredoc)
		args->limiter = av[2];
	else
		args->limiter = NULL;
	return (0);
}

/*
 * Returns environment variable if it exists, otherwise returns NULL.
*/

char	*ft_getenv(char *str, char **env)
{
	int	i;
	int	vlen;

	if (!ft_isalpha(*str) && *str != '_')
		return (NULL);
	i = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	vlen = i;
	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], str, vlen) == 0 && env[i][vlen] == '=')
			return (env[i] + (vlen + 1));
		i++;
	}
	return (NULL);
}

/*
* Checks if a command exists and has execute permission
* Returns command pathname (allocated) if it exists, otherwise returns NULL.
*/

char	*ft_which(char *cmd, char **path_dirs)
{
	char	*tmp;
	char	*cmd_pathname;
	int		i;

	if (ft_strchr(cmd, '/'))
	{
		if (!access(cmd, F_OK) && !access(cmd, X_OK))
			return (ft_strdup(cmd));
		return (NULL);
	}
	tmp = ft_strjoin("/", cmd);
	if (!tmp)
		return (NULL);
	i = -1;
	while (path_dirs[++i])
	{
		cmd_pathname = ft_strjoin(path_dirs[i], tmp);
		if (!cmd_pathname)
			return (free(tmp), NULL);
		if (!access(cmd_pathname, F_OK) && !access(cmd_pathname, X_OK))
			return (free(tmp), cmd_pathname);
		free(cmd_pathname);
	}
	return (free(tmp), NULL);
}

/*
 * If valid, returns a command arguments and
 * replaces its name with its absolute pathname.
 * otherwise returns NULL.
 */

char	**get_cmd_args(char	*cmd, char **path_dirs)
{
	char	*cmd_pathname;
	char	**cmd_args;

	if (!cmd || !path_dirs)
		return (NULL);
	cmd_args = ft_split(cmd, ' ');
	if (!cmd_args)
		return (perror(PROG_NAME), NULL);
	cmd_pathname = ft_which(cmd_args[0], path_dirs);
	if (!cmd_pathname)
	{
		if (!ft_strchr(cmd_args[0], '/'))
		{
			ft_putstr_fd(CMD_NOT_FOUND, STDERR_FILENO);
			ft_putendl_fd(cmd_args[0], STDERR_FILENO);
			return (ft_free_arr(cmd_args), NULL);
		}
		return (ft_free_arr(cmd_args), perror(PROG_NAME), NULL);
	}
	free(cmd_args[0]);
	cmd_args[0] = cmd_pathname;
	return (cmd_args);
}
