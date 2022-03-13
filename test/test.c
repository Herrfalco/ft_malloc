/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:38:48 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/13 12:14:14 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../lib.h"

//printf doesn't work ??
//debug file ??
//realloc

/*
**	void		*malloc(size_t size);
**	void		*realloc(void *ptr, size_t size);
**	void		free(void *ptr);
**	void		show_alloc_mem(void);
*/

int			main(void) {
	void	*mem;

	mem = malloc(52);
	((int *)mem)[0] = 99999;
	show_alloc_mem();
	free(mem);
//	malloc(0);
//	free((void *)9999);
//	mem = realloc(NULL, 18);
	return (0);
}
