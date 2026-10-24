/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kayoshid <kayoshid@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/24 13:52:34 by kayoshid          #+#    #+#             */
/*   Updated: 2026/10/24 20:08:19 by kayoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

typedef struct s_stash
{
	char	*data;
	size_t	length;
	size_t	capacity;
	size_t	scan;
}	t_stash;

char	*get_next_line(int fd);
char	*gnl_clear(t_stash *stash);
void	gnl_copy(char *dest, const char *src, size_t length);
int		gnl_reserve(t_stash *stash, size_t extra);
int		gnl_read(int fd, t_stash *stash);
char	*gnl_take(t_stash *stash);

#endif
