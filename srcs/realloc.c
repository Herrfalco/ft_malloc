/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   realloc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:36:21 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/09 10:54:56 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/header.h"

t_bool		in_place(void *ptr, size_t size, size_t	*old_size) {
	t_hdr		*hdr = ((t_hdr *)ptr) - 1;
	t_big_hdr	*big_hdr = ((t_big_hdr *)ptr) - 1;

	if (in_zone(ptr, &glob.tiny)) {
		*old_size = hdr->size;
		if (size + sizeof(t_hdr) <= glob.tiny.cell_sz) {
			hdr->size = size;
			return (TRUE);
		}
	} else if (in_zone(ptr, &glob.small)) {
		*old_size = hdr->size;
		if (size + sizeof(t_hdr) <= glob.small.cell_sz) {
			hdr->size = size;
			return (TRUE);
		}
	} else {
		*old_size = big_hdr->size;
		if (size + sizeof(t_big_hdr)
				<= ((big_hdr->size + sizeof(t_big_hdr)) / getpagesize()
					+ !!((big_hdr->size + sizeof(t_big_hdr)) % getpagesize()))
				* getpagesize()) {
			big_hdr->size = size;
			return (TRUE);
		}
	}
	return (FALSE);
}

void		*realloc(void *ptr, size_t size) {
	t_hdr		*new_hdr;
	t_big_hdr	*new_big;
	t_bool		is_big;
	size_t		old_size;

	pthread_mutex_lock(&glob.mut);
	if (in_place(ptr, size, &old_size)) {
		show_deb(old_size < size ? INC : DEC, !!ptr, old_size, size,
				ptr, NULL);
		pthread_mutex_unlock(&glob.mut);
		return (ptr);
	}
	new_hdr = raw_malloc(size, TRUE, &is_big);
	if (is_big) {
		new_big = (t_big_hdr *)new_hdr;
		new_big->size = size;
		copy_n_bytes((uint8_t *)(new_big + 1), ptr, old_size);
		raw_free(ptr, FALSE);
		show_deb(REALLOC, !!new_big, old_size, size, ptr, new_big + 1);
		pthread_mutex_unlock(&glob.mut);
		return (new_big + 1);
	} else {
		new_hdr->size = size;
		copy_n_bytes((uint8_t *)(new_hdr + 1), ptr, old_size);
		raw_free(ptr, FALSE);
		show_deb(REALLOC, !!new_hdr, old_size, size, ptr, new_hdr + 1);
		pthread_mutex_unlock(&glob.mut);
		return (new_hdr + 1);
	}
}
