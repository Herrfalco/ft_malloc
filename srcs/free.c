/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:22:53 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/13 14:45:25 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/header.h"

static size_t		free_from_zone(void *ptr) {
	t_hdr		*hdr = ((t_hdr *)ptr) - 1;
	size_t		old_size = hdr->size;

	hdr->size = 0;
	return (old_size);
}

static size_t		free_big(void *ptr) {
	t_big_hdr	*hdr = ((t_big_hdr *)ptr) - 1;
	size_t		old_size = hdr->size;

	if (hdr->prev)
		hdr->prev->next = hdr->next;
	else
		glob.big = hdr->next;
	if (hdr->next)
		hdr->next->prev = hdr->prev;
	return (munmap(hdr, hdr->size + sizeof(t_big_hdr)) ? 0 : old_size);
}

size_t		raw_free(void *ptr) {
	size_t		size;
	t_loc		loc;

	if (!ptr || !glob.init || (loc = wich_loc(ptr)) == OTHER)
		return(0);
	if (loc == TINY || loc == SMALL)
		size = free_from_zone(ptr);
	else
		size = free_big(ptr);
	return (size);
}

void		free(void *ptr) {
	size_t		size;

	pthread_mutex_lock(&glob.mut);
	size = raw_free(ptr);
	show_deb(FREE, !!size, size, 0, ptr, NULL);
	pthread_mutex_unlock(&glob.mut);
}
