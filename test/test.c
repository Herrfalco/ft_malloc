/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:38:48 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/30 08:53:18 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib.h"

/*
**	void		*malloc(size_t size);
**	void		*realloc(void *ptr, size_t size);
**	void		free(void *ptr);
**	void		show_alloc_mem(void);
**	void		show_zone_param(void);
*/

int			main(void) {
	void	*vals[5];

	show_zone_param();
	vals[0] = malloc(10);
	vals[1] = realloc(vals[0], 1000);
	vals[2] = realloc(vals[0], 10000);
	vals[3] = realloc(vals[1], 10000);
	free(vals[0]);
	vals[4] = realloc(NULL, 999);
	vals[0] = realloc(vals[4], 400);
	for (size_t i = 0; i < 255; i += (i + 1))
		((char *)vals[0])[i] = i;
	for (size_t i = 0; i < 50; ++i)
		((char *)vals[0] + 300)[i] = i;
	show_alloc_mem();
	return (0);
}
