/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kayoshid <kayoshid@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/24 13:52:34 by kayoshid          #+#    #+#             */
/*   Updated: 2026/10/30 17:51:30 by kayoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*gnl_clear(char **buffer, size_t *buffer_length,
			size_t *buffer_capacity)
{
	free(*buffer);
	*buffer = NULL;
	*buffer_length = 0;
	*buffer_capacity = 0;
	return (NULL);
}

void	gnl_copy(char *destination, const char *source, size_t byte_count)
{
	size_t	index;

	index = 0;
	while (index < byte_count)
	{
		destination[index] = source[index];
		index++;
	}
}

int	gnl_reserve(char **buffer, size_t buffer_length,
		size_t *buffer_capacity, size_t read_size)
{
	char	*new_buffer;
	size_t	required_capacity;
	size_t	new_capacity;

	if (read_size > (size_t)-1 - buffer_length - 1)
		return (0);
	required_capacity = buffer_length + read_size + 1;
	if (required_capacity <= *buffer_capacity)
		return (1);
	new_capacity = required_capacity;
	if (*buffer_capacity <= (size_t)-1 / 2
		&& *buffer_capacity * 2 > required_capacity)
		new_capacity = *buffer_capacity * 2;
	new_buffer = malloc(new_capacity);
	if (!new_buffer)
		return (0);
	gnl_copy(new_buffer, *buffer, buffer_length);
	free(*buffer);
	*buffer = new_buffer;
	*buffer_capacity = new_capacity;
	return (1);
}

int	gnl_read(int fd, char **buffer, size_t *buffer_length,
		size_t *buffer_capacity)
{
	ssize_t	bytes_read;
	size_t	search_pos;

	if (BUFFER_SIZE <= 0)
		return (0);
	search_pos = 0;
	while (1)
	{
		while (search_pos < *buffer_length && (*buffer)[search_pos] != '\n')
			search_pos++;
		if (search_pos < *buffer_length)
			return (1);
		if (!gnl_reserve(buffer, *buffer_length, buffer_capacity, BUFFER_SIZE))
			return (0);
		bytes_read = read(fd, *buffer + *buffer_length, BUFFER_SIZE);
		if (bytes_read < 0)
			return (0);
		if (bytes_read == 0)
			return (1);
		*buffer_length += bytes_read;
	}
}

char	*gnl_take(char *buffer, size_t *buffer_length)
{
	char	*line;
	size_t	line_length;

	line_length = 0;
	while (line_length < *buffer_length && buffer[line_length] != '\n')
		line_length++;
	if (line_length < *buffer_length)
		line_length++;
	line = malloc(line_length + 1);
	if (!line)
		return (NULL);
	gnl_copy(line, buffer, line_length);
	line[line_length] = '\0';
	*buffer_length -= line_length;
	gnl_copy(buffer, buffer + line_length, *buffer_length);
	return (line);
}
