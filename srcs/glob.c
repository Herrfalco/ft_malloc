/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:39:36 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/13 14:44:21 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "../hdrs/header.h"

t_glob			glob;

static void		align_zone(t_zone *zone) {
	size_t		cell_nb = zone->cell_nb;
	size_t		cell_sz = zone->cell_sz;
	size_t 		zone_sz = cell_sz * cell_nb;
	size_t		page_nb = zone_sz / getpagesize() + !!(zone_sz % getpagesize());

	for (;; ++page_nb) {
		cell_sz = zone->cell_sz;
		for (;; ++cell_sz) {
			cell_nb = zone->cell_nb;
			zone_sz = cell_nb * cell_sz;
			if (zone_sz > page_nb * getpagesize())
				break;
			for (;; ++cell_nb) {
				zone_sz = cell_nb * cell_sz;
				if (zone_sz > page_nb * getpagesize())
					break;
				if (!(zone_sz % sizeof(size_t)) && zone_sz == page_nb * getpagesize()) {
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
	glob.tiny.cell_sz = MIN_TNY_UP_LIM + sizeof(t_hdr);
	glob.tiny.mem = NULL;
	glob.small.cell_nb = MIN_CELL_NB;	
	glob.small.cell_sz = MIN_SML_UP_LIM + sizeof(t_hdr);
	glob.small.mem = NULL;
	glob.big = NULL;
	glob.debug = 0;
	glob.init = FALSE;
	pthread_mutex_init(&glob.mut, NULL);
}

static void			init(void) __attribute__((constructor));
void				init(void) {
	char	*env_val;

	glob_init();
	if ((env_val = getenv(DEBUG_ENV_VAR)))
		glob.debug = labcmp_icase(env_val);
	align_zone(&glob.tiny);
	align_zone(&glob.small);
	if ((glob.tiny.mem = mmap(NULL, glob.tiny.cell_nb * glob.tiny.cell_sz,
					PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED
			|| (glob.small.mem = mmap(NULL, glob.small.cell_nb * glob.small.cell_sz,
					PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		return;
	if (glob.debug > NO) {
		fprintf(STDOUT, DELIMITER);
		show_zone_param("TINY", &glob.tiny);
		show_zone_param("SMALL", &glob.small);
		fprintf(STDOUT, DELIMITER);
	}
	glob.init = TRUE;
}

static void			clean(void) __attribute__((destructor));
void				clean(void) {
	munmap(glob.tiny.mem, glob.tiny.cell_nb * glob.tiny.cell_sz);		
	munmap(glob.small.mem, glob.small.cell_nb * glob.small.cell_sz);		
	pthread_mutex_unlock(&glob.mut);
	pthread_mutex_destroy(&glob.mut);
}
