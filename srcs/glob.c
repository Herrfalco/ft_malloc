/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:39:36 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/28 09:50:28 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../hdrs/header.h"

t_glob			glob;

static void		align_zone(t_zone *zone) {
	size_t		cell_nb = zone->cell_nb;
	size_t		cell_sz = round_nb(zone->cell_sz, sizeof(void *));
	size_t 		zone_sz = sizeof(t_frame_hdr) + cell_sz * cell_nb;
	size_t		page_nb = round_nb(zone_sz, getpagesize()) / getpagesize();

	for (;; ++page_nb) {
		cell_sz = round_nb(zone->cell_sz, sizeof(void *));
		for (;; cell_sz += sizeof(void *)) {
			cell_nb = zone->cell_nb;
			zone_sz = sizeof(t_frame_hdr) + cell_nb * cell_sz;
			if (zone_sz > page_nb * getpagesize())
				break;
			for (;; ++cell_nb) {
				zone_sz = sizeof(t_frame_hdr) + cell_nb * cell_sz;
				if (zone_sz > page_nb * getpagesize())
					break;
				if (!(zone_sz % sizeof(void *)) && zone_sz == page_nb * getpagesize()) {
					zone->cell_sz = cell_sz;
					zone->cell_nb = cell_nb;
					return;
				}
			}
		}
	}
}

static void		glob_init(void) {
	glob.tiny.cell_nb = MIN_CELL_NB;	
	glob.tiny.cell_sz = MIN_TNY_UP_LIM + sizeof(t_cell_hdr);
	glob.tiny.frame = NULL;
	glob.small.cell_nb = MIN_CELL_NB;	
	glob.small.cell_sz = MIN_SML_UP_LIM + sizeof(t_cell_hdr);
	glob.small.frame = NULL;
	glob.big = NULL;
	glob.debug = 0;
	glob.fline = FALSE;
	pthread_mutex_init(&glob.mut, NULL);
}

static void			init(void) __attribute__((constructor));
void				init(void) {
	char	*env_val;

	glob_init();
	if ((env_val = getenv(DEBUG_ENV_VAR)))
		glob.debug = labcmp_icase(env_val, DEBUG_LAB);
	if ((env_val = getenv(SHOW_ENV_VAR)))
		glob.show = labcmp_icase(env_val, SHOW_LAB);
	align_zone(&glob.tiny);
	align_zone(&glob.small);
}

static void			clean_frames(t_frame_hdr *frame) {
	if (!frame)	
		return;
	clean_frames(frame->next);
	munmap(frame, sizeof(t_frame_hdr) + glob.tiny.cell_nb * glob.tiny.cell_sz);
}

static void			clean(void) __attribute__((destructor));
void				clean(void) {
	clean_frames(glob.tiny.frame);
	clean_frames(glob.small.frame);
	pthread_mutex_unlock(&glob.mut);
	pthread_mutex_destroy(&glob.mut);
}
