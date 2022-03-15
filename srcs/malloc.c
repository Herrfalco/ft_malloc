/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 19:23:04 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/15 15:09:37 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/header.h"

static t_hdr		*space_from_zone(t_zone *zone, uint16_t size) {
	t_hdr		*cell;

	for (size_t i = 0; i < zone->cell_nb; ++i) {
		cell = (t_hdr *)(((uint8_t *)zone->mem) + i * zone->cell_sz);
		if (!cell->size) {
			cell->size = size;
			return (cell);
		}
	}
	return (NULL);
}

static t_big_hdr 	*big_alloc(size_t size) {
	t_big_hdr		*cell;
	t_big_hdr		*ptr;

	if (size > SIZE_MAX - sizeof(t_big_hdr) || (cell = mmap(NULL, size + sizeof(t_big_hdr),
					PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		return (NULL);
	cell->size = size;
	cell->capacity = (size / getpagesize() + !!(size % getpagesize()))
		* getpagesize() - sizeof(t_big_hdr);
	if (!glob.big || glob.big > cell) {
		cell->next = glob.big;
		cell->prev = NULL;
		glob.big = cell;
	} else {
		for (ptr = glob.big; ptr->next && ptr->next < cell; ptr = ptr->next);
		cell->next = ptr->next;
		cell->prev = ptr;
		ptr->next = cell;
	}
	if (cell->next)
		cell->next->prev = cell;
	return (cell);
}

void		*raw_malloc(size_t size, t_bool with_hdr, t_bool *is_big) {
	t_hdr		*mem;
	t_big_hdr	*big_mem;

	if (!glob.init || !size)
		return (NULL);
	if (is_big)
		*is_big = FALSE;
	if (size <= glob.tiny.cell_sz - sizeof(t_hdr))
		if ((mem = space_from_zone(&glob.tiny, size)))
			return (with_hdr ? mem : mem + 1);
	if (size <= glob.small.cell_sz - sizeof(t_hdr))
		if ((mem = space_from_zone(&glob.small, size)))
			return (with_hdr ? mem : mem + 1);
	if ((big_mem = big_alloc(size))) {
		if (is_big)
			*is_big = TRUE;
		return (with_hdr ? big_mem : big_mem + 1);
	}
	return (NULL);
}

void		*malloc(size_t size) {
	void	*mem;
	
	pthread_mutex_lock(&glob.mut);
	mem = raw_malloc(size, FALSE, NULL);
	show_deb(ALLOC, !!mem, size, 0, mem, NULL);
	pthread_mutex_unlock(&glob.mut);
	return (mem);
}
