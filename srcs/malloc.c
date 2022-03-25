/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   malloc.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/03 19:23:04 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/25 16:06:09 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/header.h"

static t_cell_hdr		*space_from_zone(t_zone *zone, size_t size) {
	t_cell_hdr		*cell;
	t_frame_hdr		*frame;

	if (size > SIZE_MAX - (sizeof(t_frame_hdr) + sizeof(t_cell_hdr)))
		return (NULL);
	for (frame = zone->frame; frame; frame = frame->next) {
		for (size_t i = 0; i < zone->cell_nb; ++i) {
			cell = (t_cell_hdr *)((uint8_t *)(frame + 1) + i * zone->cell_sz);
			if (!cell->size) {
				cell->size = size;
				++frame->infill;
				return (cell);
			}
		}
	}
	if ((frame = mmap(NULL, sizeof(t_frame_hdr) + zone->cell_nb * zone->cell_sz,
					PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		return (NULL);
	frame->infill = 1;
	frame->next = zone->frame;
	if (frame->next)
		frame->next->prev = frame;
	zone->frame = frame;
	cell = (t_cell_hdr *)(frame + 1);
	cell->size = size;
	return (cell);
}

static t_big_hdr 	*big_alloc(size_t size) {
	t_big_hdr		*cell;

	if (size > SIZE_MAX - sizeof(t_big_hdr) || (cell = mmap(NULL, size + sizeof(t_big_hdr),
					PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		return (NULL);
	cell->size = size;
	cell->capacity = (size / getpagesize() + !!(size % getpagesize()))
		* getpagesize() - sizeof(t_big_hdr);
	cell->next = glob.big;
	cell->prev = NULL;
	glob.big = cell;
	if (cell->next)
		cell->next->prev = cell;
	return (cell);
}

void		*raw_malloc(size_t size, t_bool with_hdr, t_bool *is_big) {
	t_cell_hdr	*mem;
	t_big_hdr	*big_mem;

	if (!size)
		return (NULL);
	if (is_big)
		*is_big = FALSE;
	if (size <= glob.tiny.cell_sz - sizeof(t_cell_hdr))
		if ((mem = space_from_zone(&glob.tiny, size)))
			return (with_hdr ? mem : mem + 1);
	if (size <= glob.small.cell_sz - sizeof(t_cell_hdr))
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
