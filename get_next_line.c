/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kayoshid <kayoshid@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 23:19:14 by kayoshid          #+#    #+#             */
/*   Updated: 2026/10/31 01:51:30 by kayoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*strcut_fr(char *str)
{
	char	*dest;
	size_t	i;

	if (!str || !*str)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	dest = (char *)malloc(sizeof(char) * (i + 1));
	if (!dest)
		return (NULL);
	i = 0;
	while (str[i] && str[i] != '\n')
	{
		dest[i] = str[i];
		i++;
	}
	if (str[i] == '\n')
		dest[i++] = '\n';
	dest[i] = '\0';
	return (dest);
}

char	*strcut_lst(char *str)
{
	char	*dest;
	size_t	i;
	size_t	j;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (!str[i] || !str[i + 1])
	{
		free(str);
		return (NULL);
	}
	i++;
	dest = (char *)malloc(sizeof(char) * (ft_strlen(str + i) + 1));
	if (!dest)
	{
		free(str);
		return (NULL);
	}
	j = 0;
	while (str[i])
		dest[j++] = str[i++];
	dest[j] = '\0';
	free(str);
	return (dest);
}

static char	*divide_line(char **str)
{
	char	*ret;

	ret = strcut_fr(*str);
	if (!ret)
	{
		free(*str);
		*str = NULL;
		return (NULL);
	}
	*str = strcut_lst(*str);
	return (ret);
}

char	*read_to_str(int fd, char *str)
{
	ssize_t	bytes_read;
	char	*buffer;
	char	*tmp;

	buffer = (char *)malloc(sizeof(char) * (BUFFER_SIZE + 1));
	if (!buffer)
		return (NULL);
	bytes_read = 1;
	while (!is_new_line(str) && bytes_read > 0)
	{
		bytes_read = read(fd, buffer, BUFFER_SIZE);
		if (bytes_read == -1)
		{
			free(buffer);
			free(str);
			return (NULL);
		}
		buffer[bytes_read] = '\0';
		tmp = ft_strjoin(str, buffer);
		free(str);
		str = tmp;
	}
	free(buffer);
	return (str);
}

char	*get_next_line(int fd)
{
	static char	*str;
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	str = read_to_str(fd, str);
	if (!str || *str == '\0')
	{
		free(str);
		str = NULL;
		return (NULL);
	}
	line = divide_line(&str);
	return (line);
}
