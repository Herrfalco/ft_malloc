/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:22:53 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/09 08:24:12 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

size_t		free_from_zone(void *ptr, t_zone *zone) {
	t_hdr		*hdr = ((t_hdr *)ptr) - 1;
	size_t		old_size = hdr->size;

	if (!in_zone(ptr, zone))
		return (0);
	hdr->size = 0;
	return (old_size);
}

size_t		free_big(void *ptr) {
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

void		raw_free(void *ptr, t_bool debug) {
	size_t		size;

	if (!ptr)
		return;
	(void)((size = free_from_zone(ptr, &glob.tiny))
		|| (size = free_from_zone(ptr, &glob.small))
		|| (size = free_big(ptr)));
	if (debug)
		show_deb(FREE, !!size, size, 0, ptr, NULL);
}

void		ft_free(void *ptr) {
	pthread_mutex_lock(&glob.mut);
	raw_free(ptr, TRUE);
	pthread_mutex_unlock(&glob.mut);
}
