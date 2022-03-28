/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:38:48 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/28 11:51:22 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib.h"

#include <stdio.h>

//wrong input

/*
**	void		*malloc(size_t size);
**	void		*realloc(void *ptr, size_t size);
**	void		free(void *ptr);
**	void		show_alloc_mem(void);
**	void		show_zone_param(void);
*/

int			main(void) {
	int			*t0;
	int			*t1;
	int			*t2;
	
	t0 = malloc(sizeof(int) * 200);
	t1 = malloc(sizeof(int) * 10);
	t1 = realloc(t1, sizeof(int) * 200);
	t2 = malloc(sizeof(int) * 10);
	show_alloc_mem();
	return (0);
}
