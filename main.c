#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "get_next_line.h"

int	main(int argc, char **argv)
{
	int		fd;
	char	*line;
	int		line_num;

	if (argc < 2)
	{
		printf("標準入力モード\n");
		fd = 0;
	}
	else
	{
		fd = open(argv[1], O_RDONLY);
		if (fd < 0)
		{
			perror("Error opening file");
			return (1);
		}
	}

	line_num = 1;
	// get_next_line が NULL を返すまでループ
	while ((line = get_next_line(fd)) != NULL)
	{
		printf("[%d]: %s", line_num++, line);
		free(line); // 呼び出し側で free
	}

	if (fd != 0)
		close(fd);

	return (0);
}
