/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: daflynn <daflynn@student.42berlin.de>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/16 12:14:06 by daflynn           #+#    #+#             */
/*   Updated: 2025/09/16 12:14:14 by daflynn          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_execute_pipeline(t_dat *d, char ***cmd)
{
	int	**fd;

	if (ft_strcmp(cmd[0][0], "./minishell") == 0)
	{
		ft_nested_minishell(d, cmd[0], NULL);
		return ;
	}
	fd = init_fd_array(d->tot);
	if (!fd || !ft_create_pipes(fd, d->tot))
	{
		if (fd)
			ft_free_fd(fd);
		return ;
	}
	ft_fork_children(d, cmd, fd);
	ft_close_pipes(fd, d->tot);
	ft_wait_children(d->tot);
	ft_set_main_signals();
	ft_free_fd(fd);
}

void	ft_exec_command(t_dat *d, char **cmd)
{
	char	*cmd_path;

	if (!cmd || !cmd[0])
		exit(127);
	if (ft_is_pipe_builtin(cmd[0]))
	{
		ft_execute_builtin_in_child(d, cmd);
		exit(g_last_exit_status);
	}
	cmd_path = ft_get_cmd_path(d, cmd[0], 0);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd[0], 2);
		ft_putendl_fd(": command not found", 2);
		exit(127);
	}
	execve(cmd_path, cmd, d->evs);
	free(cmd_path);
	perror("execve");
	exit(1);
}
