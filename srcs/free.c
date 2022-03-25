/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:22:53 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/25 16:10:42 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/header.h"

static size_t		free_from_loc(void *ptr, t_loc loc, t_frame_hdr *frame) {
	t_cell_hdr		*cell = ((t_cell_hdr *)ptr) - 1;
	t_big_hdr		*big_hdr = ((t_big_hdr *)ptr) - 1;
	size_t			old_size = cell->size;
	t_zone			*zone;

	if ((loc == TINY || loc == SMALL) && cell->size) {
			cell->size = 0;
			--frame->infill;
			if (!frame->infill) {
				zone = loc == TINY ? &glob.tiny : &glob.small;
				if (frame->prev)
					frame->prev->next = frame->next;
				else 
					zone->frame = frame->next;	
				if (frame->next)
					frame->next->prev = frame->prev;
				return (munmap(frame, sizeof(t_frame_hdr) + zone->cell_nb * zone->cell_sz)
							? 0 : old_size);
			} else
				return (old_size);
	} else if (loc == BIG) {
		old_size = big_hdr->size;
		if (big_hdr->prev)
			big_hdr->prev->next = big_hdr->next;
		else
			glob.big = big_hdr->next;
		if (big_hdr->next)
			big_hdr->next->prev = big_hdr->prev;
		return (munmap(big_hdr, big_hdr->size + sizeof(t_big_hdr)) ? 0 : old_size);
	}
	return (0);
}

size_t		raw_free(void *ptr) {
	t_loc			loc;
	t_frame_hdr		*frame;

	if (!ptr || (loc = wich_loc(ptr, &frame)) == OTHER)
		return(0);
	return (free_from_loc(ptr, loc, frame));
}

void		free(void *ptr) {
	size_t		size;

	pthread_mutex_lock(&glob.mut);
	size = raw_free(ptr);
	show_deb(FREE, !!size, size, 0, ptr, NULL);
	pthread_mutex_unlock(&glob.mut);
}
