/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:36:21 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/30 07:56:11 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/header.h"

static void		*in_place(void *ptr, size_t size, size_t *old_size, t_loc loc,
		t_debug *deb) {
	t_cell_hdr		*cell = ((t_cell_hdr *)ptr) - 1;
	t_big_hdr		*big_hdr = ((t_big_hdr *)ptr) - 1;

	*old_size = cell->size;
	*deb = *old_size < size ? INC : DEC;
	switch (loc) {
		case TINY:
			if (size + sizeof(t_cell_hdr) <= glob.tiny.cell_sz) {
				cell->size = size;
				return (ptr);
			}
			break;
		case SMALL:
			if (size + sizeof(t_cell_hdr) <= glob.small.cell_sz) {
				cell->size = size;
				return (ptr);
			}
			break;
		case BIG:
			*old_size = big_hdr->size;
			*deb = *old_size < size ? INC : DEC;
			if (size <= big_hdr->capacity) {
				big_hdr->size = size;
				return (ptr);
			}
		default:
			break;
	}
	return (NULL);
}

static void		*move_alloc(void *ptr, size_t size, size_t old_size, t_debug *deb) {
	t_bool		is_big;
	t_cell_hdr	*new_cell;
	t_big_hdr	*new_big;

	*deb = REALLOC;
	if (!(new_cell = raw_malloc(size, TRUE, &is_big)))
		return (NULL);
	else if (is_big) {
		new_big = (t_big_hdr *)new_cell;
		copy_n_bytes((uint8_t *)(new_big + 1), ptr, old_size);
		raw_free(ptr);
		return (new_big + 1);
	} else {
		copy_n_bytes((uint8_t *)(new_cell + 1), ptr, old_size);
		raw_free(ptr);
		return (new_cell + 1);
	}
}

static void		*raw_realloc(void *ptr, size_t size, size_t *old_size, t_debug *deb) {
	t_loc			loc;
	void			*res;
	t_frame_hdr		*frame;

	if (!ptr || !size || (loc = wich_loc(ptr, &frame)) == OTHER)
		return (NULL);
	if (!(res = in_place(ptr, size, old_size, loc, deb)))
		res = move_alloc(ptr, size, *old_size, deb);
	return (res);
}

void			*realloc(void *ptr, size_t size) {
	void		*res;
	t_debug		deb = REALLOC;
	size_t		old_size;

	pthread_mutex_lock(&glob.mut);
	res = raw_realloc(ptr, size, &old_size, &deb);
	show_deb(deb, !!res, old_size, size, ptr, res);
	pthread_mutex_unlock(&glob.mut);
	return (res);
}
