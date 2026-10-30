/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kayoshid <kayoshid@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/30 23:17:06 by kayoshid          #+#    #+#             */
/*   Updated: 2026/10/31 01:51:49 by kayoshid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <stdlib.h>
# include <unistd.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

char		*get_next_line(int fd);
char		*read_to_str(int fd, char *str);
char		*strcut_fr(char *str);
char		*strcut_lst(char *str);

size_t		ft_strlen(const char *str);
char		*ft_strchr(const char *s, int c);
int			is_new_line(char *str);
char		*ft_strdup(const char *s);
char		*ft_strjoin(char const *s1, char const *s2);

#endif
