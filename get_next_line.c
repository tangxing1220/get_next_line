/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: xtang <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/11/06 10:57:06 by xtang             #+#    #+#             */
/*   Updated: 2019/11/07 18:57:08 by xtang            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

static int	gnl_verify_line(char **stack, char **line)
{
	char	*tmp_stack;
	char	*strchr_stack;
	int		i;

	i = 0;
	strchr_stack = *stack;
	while (strchr_stack[i] != '\n')
		if (!strchr_stack[i++])
			return (0);
	tmp_stack = &strchr_stack[i];
	*tmp_stack = '\0';
	*line = ft_strdup(*stack);
//	free(*stack);
//	*stack = NULL;
	*stack = ft_strdup(tmp_stack + 1);
//	free(tmp_stack);
//	tmp_stack = NULL; 
	free(strchr_stack);
	return (1);
}

static int	gnl_read_file(int fd, char *heap, char **stack, char **line)
{
	int		ret;
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

int			get_next_line(const int fd, char **line)
{
	int			ret;
	static char	*stack;
	char		*heap;
	int			i;

	if (!(heap = (char *)malloc(sizeof(char) * (BUFF_SIZE + 1))))
		return (-1);
	if (stack)
		if (gnl_verify_line(&stack, line))
			return (1);
	i = 0;
	while (i < BUFF_SIZE)
		heap[i++] = '\0';
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
