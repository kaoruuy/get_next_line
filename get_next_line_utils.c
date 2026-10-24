/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kayoshid <kayoshid@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/24 13:52:34 by kayoshid          #+#    #+#             */
/*   Updated: 2026/10/24 13:52:58 by kayoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

size_t	gnl_length(const char *text, char stop)
{
	size_t	length;

	length = 0;
	while (text && text[length] && text[length] != stop)
		length++;
	return (length);
}

char	*gnl_clear(char **stash)
{
	free(*stash);
	*stash = NULL;
	return (NULL);
}

char	*gnl_append(char *stash, const char *buffer)
{
	char	*joined;
	size_t	length;
	size_t	i;

	length = gnl_length(stash, '\0');
	joined = malloc(length + gnl_length(buffer, '\0') + 1);
	if (!joined)
		return (gnl_clear(&stash));
	i = 0;
	while (i < length)
	{
		joined[i] = stash[i];
		i++;
	}
	i = 0;
	while (buffer[i])
	{
		joined[length + i] = buffer[i];
		i++;
	}
	joined[length + i] = '\0';
	free(stash);
	return (joined);
}

char	*gnl_read(int fd, char *stash, char *buffer)
{
	ssize_t	bytes_read;

	if (BUFFER_SIZE <= 0)
		return (gnl_clear(&stash));
	while (!stash || stash[gnl_length(stash, '\n')] != '\n')
	{
		bytes_read = read(fd, buffer, (size_t)BUFFER_SIZE);
		if (bytes_read < 0)
			return (gnl_clear(&stash));
		if (bytes_read == 0)
			break ;
		buffer[bytes_read] = '\0';
		stash = gnl_append(stash, buffer);
		if (!stash)
			return (NULL);
	}
	return (stash);
}

char	*gnl_take(char *stash)
{
	char	*line;
	size_t	length;
	size_t	i;

	length = gnl_length(stash, '\n');
	if (stash[length] == '\n')
		length++;
	line = malloc(length + 1);
	if (!line)
		return (NULL);
	i = 0;
	while (i < length)
	{
		line[i] = stash[i];
		i++;
	}
	line[i] = '\0';
	i = 0;
	while (stash[length + i])
	{
		stash[i] = stash[length + i];
		i++;
	}
	stash[i] = '\0';
	return (line);
}
