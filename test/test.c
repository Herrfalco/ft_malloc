/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:38:48 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/25 20:27:05 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../lib.h"

#include <stdio.h>

//display order
//wrong input
//wrong define

/*
**	void		*malloc(size_t size);
**	void		*realloc(void *ptr, size_t size);
**	void		free(void *ptr);
**	void		show_alloc_mem(void);
**	void		show_zone_param(void);
*/

int			main(void) {
	show_zone_param();

	char		*arr[10] = { NULL };

	for (int i = 0; i < 10; ++i)
		arr[i] = malloc(i + 1);

	for (int i = 0; i < 10; ++i)
		*(arr[i]) = i + 1;

	show_alloc_mem();

	for (int i = 0; i < 10; ++i)
		arr[i] = realloc(arr[i], i + 500);

	for (int i = 0; i < 10; ++i)
		arr[i] = realloc(arr[i], i + 5000);

	((char *)arr[9])[0] = 0;
	((char *)arr[9])[1] = 0;
	((char *)arr[9])[2] = 0;
	((char *)arr[9])[3] = 0xbb;
	((char *)arr[9])[4] = 0x42;
	((char *)arr[9])[5] = 0;
	((char *)arr[9])[6] = 0;
	((char *)arr[9])[7] = 0;
	((char *)arr[9])[8] = 0;
	((char *)arr[9])[9] = 0x19;

	show_alloc_mem();

	for (int i = 0; i < 10; ++i)
		free(arr[i]);
	return (0);
}
