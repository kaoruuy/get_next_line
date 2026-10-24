/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kayoshid <kayoshid@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/24 13:52:34 by kayoshid          #+#    #+#             */
/*   Updated: 2026/10/24 20:08:19 by kayoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static t_stash	stash;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
		return (gnl_clear(&stash));
	if (!gnl_read(fd, &stash) || !stash.length)
		return (gnl_clear(&stash));
	line = gnl_take(&stash);
	if (!line || !stash.length)
		gnl_clear(&stash);
	return (line);
}
