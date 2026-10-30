/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kayoshid <kayoshid@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/24 13:52:34 by kayoshid          #+#    #+#             */
/*   Updated: 2026/10/30 20:19:40 by kayoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

/*
For local testing, uncomment these includes and the main function below.
*/
/*
#include <fcntl.h>
#include <stdio.h>
*/

char	*get_next_line(int fd)
{
	static char		*buffer;
	static size_t	buffer_length;
	static size_t	buffer_capacity;
	char			*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, NULL, 0) < 0)
		return (gnl_clear(&buffer, &buffer_length, &buffer_capacity));
	if (!gnl_read(fd, &buffer, &buffer_length, &buffer_capacity)
		|| !buffer_length)
		return (gnl_clear(&buffer, &buffer_length, &buffer_capacity));
	line = gnl_take(buffer, &buffer_length);
	if (!line || !buffer_length)
		gnl_clear(&buffer, &buffer_length, &buffer_capacity);
	return (line);
}

/*
int	main(int argc, char **argv)
{
	int		fd;
	char	*line;

	if (argc > 2)
		return (1);
	fd = STDIN_FILENO;
	if (argc == 2)
		fd = open(argv[1], O_RDONLY);
	if (fd < 0)
		return (1);
	line = get_next_line(fd);
	while (line)
	{
		fputs(line, stdout);
		free(line);
		line = get_next_line(fd);
	}
	if (argc == 2)
		close(fd);
	return (0);
}
*/
