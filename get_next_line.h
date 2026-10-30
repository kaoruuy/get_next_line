/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kayoshid <kayoshid@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/24 13:52:34 by kayoshid          #+#    #+#             */
/*   Updated: 2026/10/30 17:51:30 by kayoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char	*get_next_line(int fd);
char	*gnl_clear(char **buffer, size_t *buffer_length,
			size_t *buffer_capacity);
void	gnl_copy(char *destination, const char *source, size_t byte_count);
int		gnl_reserve(char **buffer, size_t buffer_length,
			size_t *buffer_capacity, size_t read_size);
int		gnl_read(int fd, char **buffer, size_t *buffer_length,
			size_t *buffer_capacity);
char	*gnl_take(char *buffer, size_t *buffer_length);

#endif
