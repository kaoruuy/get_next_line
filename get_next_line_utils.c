/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kayoshid <kayoshid@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/24 13:52:34 by kayoshid          #+#    #+#             */
/*   Updated: 2026/10/24 20:08:19 by kayoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*gnl_clear(t_stash *stash)
{
	free(stash->data);
	stash->data = NULL;
	stash->length = 0;
	stash->capacity = 0;
	stash->scan = 0;
	return (NULL);
}

void	gnl_copy(char *dest, const char *src, size_t length)
{
	size_t	i;

	i = 0;
	while (i < length)
	{
		dest[i] = src[i];
		i++;
	}
}

int	gnl_reserve(t_stash *stash, size_t extra)
{
	char	*data;
	size_t	needed;
	size_t	capacity;

	if (extra > (size_t)-1 - stash->length - 1)
		return (0);
	needed = stash->length + extra + 1;
	if (needed <= stash->capacity)
		return (1);
	capacity = needed;
	if (stash->capacity <= (size_t)-1 / 2
		&& stash->capacity * 2 > needed)
		capacity = stash->capacity * 2;
	data = malloc(capacity);
	if (!data)
		return (0);
	gnl_copy(data, stash->data, stash->length);
	free(stash->data);
	stash->data = data;
	stash->capacity = capacity;
	return (1);
}

int	gnl_read(int fd, t_stash *stash)
{
	ssize_t	bytes_read;

	if (BUFFER_SIZE <= 0)
		return (0);
	while (1)
	{
		while (stash->scan < stash->length
			&& stash->data[stash->scan] != '\n')
			stash->scan++;
		if (stash->scan < stash->length)
			return (1);
		if (!gnl_reserve(stash, (size_t)BUFFER_SIZE))
			return (0);
		bytes_read = read(fd, stash->data + stash->length, (size_t)BUFFER_SIZE);
		if (bytes_read < 0)
			return (0);
		if (bytes_read == 0)
			return (1);
		stash->length += bytes_read;
	}
}

char	*gnl_take(t_stash *stash)
{
	char	*line;
	size_t	length;

	length = stash->scan;
	if (length < stash->length)
		length++;
	line = malloc(length + 1);
	if (!line)
		return (NULL);
	gnl_copy(line, stash->data, length);
	line[length] = '\0';
	stash->length -= length;
	gnl_copy(stash->data, stash->data + length, stash->length);
	stash->scan = 0;
	return (line);
}
