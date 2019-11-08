/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xtang <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 10:57:06 by xtang             #+#    #+#             */
/*   Updated: 2019/11/08 17:30:25 by xtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	gnl_verify_line(char **stack, char **line)
{
	char	*tmp_stack;
	char	*strchr_stack;
	size_t	i;

	i = 0;
	strchr_stack = *stack;
	while (strchr_stack[i] != '\n')
		if (!strchr_stack[i++])
			return (0);
	tmp_stack = &strchr_stack[i];
	*tmp_stack = '\0';
	*line = ft_strdup(*stack);
	*stack = ft_strdup(tmp_stack + 1);
	free(strchr_stack);
	return (1);
}

/*
** Param. #1: The file descriptor.
** Param. #2: The heap memeory stores reading contents from file.
** Param. #3: It is a static variable. Saving contents from heap and dealing.
** Param. #4: The address of a pointer to be a charactor that will be used to
**             save the line read from the file descriptor.
** Return value: can be  1 - when ret biger than 1, the return of read() is 
**                            ret. continue to read;
**						 0 - Don't read contents, stop to read;
**					    -1 - Reading error. 		
*/

static int	gnl_read_file(int fd, char *heap, char **stack, char **line)
{
	size_t	ret;
	char	*temp_stack;

	while ((ret = read(fd, heap, BUFF_SIZE)) > 0)
	{
		heap[ret] = '\0';
		if (*stack)
		{
			temp_stack = *stack;
			*stack = ft_strjoin(temp_stack, heap);
			free(temp_stack);
			temp_stack = NULL;
		}
		else
			*stack = ft_strdup(heap);
		if (gnl_verify_line(stack, line))
			break ;
	}
	if (ret > 0)
		return (1);
	else
		return (ret);
}

/*
** Param. #1: The file descriptor that will be used to read.
** Param. #2: The address of a pointer to be a charactor that will be used to
**            save the line read from the file descriptor.
** Return Value: can be  1  - a line has been read;
**                       0  - when the reading has been completed;
**                      -1  - an error has happened.
*/

int			get_next_line(const int fd, char **line)
{
	size_t		ret;
	static char	*stack;
	char		*heap;
	size_t		i;

	if (!(heap = (char *)malloc(BUFF_SIZE + 1)))
		return (-1);
	if (stack)
		if (gnl_verify_line(&stack, line))
			return (1);
	i = 0;
//	while (i < BUFF_SIZE)
//		heap[i++] = '\0';
	ret = gnl_read_file(fd, heap, &stack, line);
	free(heap);
	if (ret != 0 || stack == NULL || stack[0] == '\0')
	{
		if (!ret && *line)
			*line = NULL;
		return (ret);
	}
	*line = stack;
	stack = NULL;
	return (1);
}
