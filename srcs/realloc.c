/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:36:21 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/13 12:09:23 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/header.h"

static void		*in_place(void *ptr, size_t size, size_t *old_size, t_loc loc,
		t_debug *deb) {
	t_hdr		*hdr = ((t_hdr *)ptr) - 1;
	t_big_hdr	*big_hdr = ((t_big_hdr *)ptr) - 1;

	*old_size = hdr->size;
	*deb = *old_size < size ? INC : DEC;
	switch (loc) {
		case TINY:
			if (size + sizeof(t_hdr) <= glob.tiny.cell_sz) {
				hdr->size = size;
				return (ptr);
			}
			break;
		case SMALL:
			if (size + sizeof(t_hdr) <= glob.small.cell_sz) {
				hdr->size = size;
				return (ptr);
			}
			break;
		case BIG:
			*old_size = big_hdr->size;
			*deb = *old_size < size ? INC : DEC;
			if (size + sizeof(t_big_hdr)
					<= ((big_hdr->size + sizeof(t_big_hdr)) / getpagesize()
						+ !!((big_hdr->size + sizeof(t_big_hdr)) % getpagesize()))
					* getpagesize()) {
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
	t_hdr		*new_hdr;
	t_big_hdr	*new_big;

	*deb = REALLOC;
	if (!(new_hdr = raw_malloc(size, TRUE, &is_big)))
		return (NULL);
	else if (is_big) {
		new_big = (t_big_hdr *)new_hdr;
		copy_n_bytes((uint8_t *)(new_big + 1), ptr, old_size);
		raw_free(ptr);
		return (new_big + 1);
	} else {
		copy_n_bytes((uint8_t *)(new_hdr + 1), ptr, old_size);
		raw_free(ptr);
		return (new_hdr + 1);
	}
}

static void		*raw_realloc(void *ptr, size_t size, size_t *old_size, t_debug *deb) {
	t_loc		loc;
	void		*res;

	if (!ptr || !glob.init || !size || (loc = wich_loc(ptr)) == OTHER)
		return (NULL);
	if (!(res = in_place(ptr, size, old_size, loc, deb)))
		res = move_alloc(ptr, size, *old_size, deb);
	return (res);
}

void		*realloc(void *ptr, size_t size) {
	void		*res;
	t_debug		deb;
	size_t		old_size;

//	pthread_mutex_lock(&glob.mut);
	res = raw_realloc(ptr, size, &old_size, &deb);
	show_deb(deb, !!res, old_size, size, ptr, res);
//	pthread_mutex_unlock(&glob.mut);
	return (res);
}
