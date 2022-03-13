/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:38:48 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/13 14:59:09 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include "../lib.h"

//realloc

/*
**	void		*malloc(size_t size);
**	void		*realloc(void *ptr, size_t size);
**	void		free(void *ptr);
**	void		show_alloc_mem(void);
*/

int			main(void) {
	void	*mem[10];

	mem[0] = malloc(0);
	mem[1] = malloc(20);
	mem[2] = malloc(200);
	mem[3] = malloc(2000);
	mem[4] = malloc(20000);
	mem[5] = (void *)1989;
	mem[6] = malloc(20000000000);
	mem[7] = malloc(20000000);
	mem[7] = realloc(mem[7], 10);
	mem[1] = realloc(mem[1], 3000);
	mem[1] = realloc(mem[1], 30000);
	free(mem[0]);
	free(mem[1]);
	free(mem[2]);
	free(mem[3]);
	free(mem[4]);
	free(mem[5]);
	free(mem[6]);
	free(mem[7]);
	return (0);
}
