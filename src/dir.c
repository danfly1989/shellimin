/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dir.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 10:45:50 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/16 10:45:57 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_change_directory(t_dat *data, size_t k)
{
	char	*path;
	char	*oldpwd;

	if (data->xln[k + 1] != NULL && data->xln[k + 2] != NULL)
	{
		(write(2, "minishell: cd: too many arguments\n", 34));
		return (g_last_exit_status, (void)0);
	}
	oldpwd = getcwd(NULL, 0);
	if (data->xln[k + 1] == NULL || ft_strcmp(data->xln[k + 1], "~") == 0)
	{
		path = ft_get_val_from_list(data->ev, "HOME");
		if (path == NULL)
		{
			(write(2, "cd: HOME not set\n", 17), g_last_exit_status = 1);
			return ;
		}
	}
	else
		path = data->xln[k + 1];
	if (chdir(path) == 0)
		ft_update_directories(data, oldpwd);
	else
		(ft_cd_error(path), g_last_exit_status = 1);
}

void	ft_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		perror("pwd error");
		return ;
	}
	printf("%s\n", cwd);
	free(cwd);
}

void	ft_update_directories(t_dat *data, char *oldpwd)
{
	char	*newpwd;

	newpwd = getcwd(NULL, 0);
	ft_update_env_variable(data, "OLDPWD", oldpwd);
	ft_update_env_variable(data, "PWD", newpwd);
	free(oldpwd);
	free(newpwd);
}

void	ft_cd_error(char *path)
{
	char	*msg;

	msg = strerror(errno);
	write(2, "minishell: cd: ", 15);
	write(2, path, ft_strlen(path));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
}
