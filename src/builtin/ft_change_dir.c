#include "../../inc/minishell.h"

int	is_directory(const char *path)
{
	struct stat	path_stat;

	stat(path, &path_stat);
	return (S_ISDIR(path_stat.st_mode));
}

int check_permission(const char *path)
{
	struct stat	path_stat;

	stat(path, &path_stat);
	return (path_stat.st_mode & S_IXUSR);
}


int check_existence(const char *path)
{
	struct stat	path_stat;

	return (stat(path, &path_stat));
}
int excute_failed(char **args)
{
	struct stat sb;

	stat(args[0], &sb);
	//if (access(args[0], F_OK))
	if (check_existence(args[0]))
		return (ft_putstr_fd("minishell: cd ", 2), ft_putstr_fd(args[0], 2),
			ft_putendl_fd(": No such file or directory", 2), 1);
	else if (!is_directory(args[0]))
		return (ft_putstr_fd("minishell: cd ", 2), ft_putstr_fd(args[0], 2),
			ft_putendl_fd(": Not a directory", 2), 1);
	else if (!check_permission(args[0]))
		return (ft_putstr_fd("minishell: cd ", 2), ft_putstr_fd(args[0], 2),
			ft_putendl_fd(": Permission denied", 2), 1);
	return (1);
}

int	ft_change_dir(char **path, t_context *ctx)
{
	char	last_path[1024] = "";
	char	*home;

	if (path && path[0])
	{
		if (chdir(path[0]) == -1)
			return (excute_failed(path));
		getcwd(last_path, sizeof(last_path));
		//free(ctx->last_pwd);
		ctx->last_pwd = ft_strdup(last_path);
	}
    else
	{
		home = get_value("HOME", ctx->env);
		if (!home)
			return (ft_putendl_fd("minishell: cd: HOME not set", 2), 1);
        if (last_path[0] == '\0')
		{
            /* If last_path is empty, use HOME directory*/
            if (chdir(home) == -1)
				return (perror("minishell: cd"), 1);
            getcwd(last_path, sizeof(last_path));
			//free(ctx->last_pwd);
			ctx->last_pwd = ft_strdup(last_path);
        }
		else
			chdir(home);
	}
	return (0);
}
