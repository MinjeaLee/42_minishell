/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exe_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heejunki <heejunki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/06 14:41:22 by heejunki          #+#    #+#             */
/*   Updated: 2023/07/06 15:46:01 by heejunki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	wait_mini(t_cmd *cmd)
{
	size_t	i;
	int		status;
	pid_t	pid;

	i = 0;
	pid = cmd->pipe[cmd->pipe_count - 1].pid;
	while (i < cmd->pipe_count)
	{
		cmd->pid = waitpid(-1, &status, 0);
		if (cmd->pid == FAILURE)
			return (ft_perror(errno));
		if (pid == cmd->pid)
		{
			if (WIFSIGNALED(status))
				g_exit_code = 128 + WTERMSIG(status);
			else
				g_exit_code = WEXITSTATUS(status);
		}
		g_exit_code = WEXITSTATUS(status);
		i++;
	}
	unlink("/tmp/whine");
	return (SUCCESS);
}

int	exe_cmd(t_parse *parse, t_cmd *cmd, t_info *info)
{
	t_pipe	*tmp_pipe;

	if (is_heredoc(cmd) == FAILURE)
	{
		free_mini(parse, cmd);
		return (FAILURE);
	}
	cmd->pipe_index = 0;
	while (cmd->pipe_index < cmd->pipe_count)
	{
		tmp_pipe = &cmd->pipe[cmd->pipe_index];
		if (check_builtin_parent(cmd, tmp_pipe) == TRUE)
			return (exe_builtin_parent(parse, cmd, info, tmp_pipe));
		if (tmp_pipe->is_pipe == TRUE)
		{
			if (pipe(tmp_pipe->pipe_fd) == FAILURE)
			{
				free_mini(parse, cmd);
				return (ft_perror(errno));
			}
		}
		// if (exe_pipe(parse, cmd, info, tmp_pipe) == FAILURE)
		// {
		//     free_mini(parse, cmd);
		//     return (FAILURE);
		// }
		cmd->pipe_index++;
	}
	wait_mini(cmd);
	return (SUCCESS);
}
