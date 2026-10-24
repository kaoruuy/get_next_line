/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kayoshid <kayoshid@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/24 13:52:34 by kayoshid          #+#    #+#             */
/*   Updated: 2026/10/24 13:52:58 by kayoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
		return (gnl_clear(&stash));
	buffer = malloc((size_t)BUFFER_SIZE + 1);
	if (!buffer)
		return (gnl_clear(&stash));
	stash = gnl_read(fd, stash, buffer);
	free(buffer);
	if (!stash || !stash[0])
		return (gnl_clear(&stash));
	line = gnl_take(stash);
	if (!line || !stash[0])
		gnl_clear(&stash);
	return (line);
}
