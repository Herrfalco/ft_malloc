/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   glob.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:39:36 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/09 08:40:01 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

t_glob		glob = {
	.tiny = {
		.cell_nb = MIN_CELL_NB,
		.cell_sz = MIN_TNY_UP_LIM + sizeof(t_hdr),
		.mem = NULL,
	},
	.small = {
		.cell_nb = MIN_CELL_NB,
		.cell_sz = MIN_SML_UP_LIM + sizeof(t_hdr),
		.mem = NULL,
	},
	.big = NULL,
	.init = FALSE,
	.debug = 0,
	.debug_out = NULL,
	.mut = PTHREAD_MUTEX_INITIALIZER,
};

void		align_zone(t_zone *zone) {
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

t_bool		init_glob(void) {
	char	*env_val;

	if ((env_val = getenv(DEBUG_ENV_VAR)))
		glob.debug = labcmp_icase(env_val);
	if ((env_val = getenv(DEBUG_ENV_FILE))) {
		glob.debug_out = fopen(env_val, "w");
		fclose(glob.debug_out);
		glob.debug_out = fopen(env_val, "a");
	} else
		glob.debug_out = stderr;
	align_zone(&glob.tiny);
	align_zone(&glob.small);
	if ((glob.tiny.mem = mmap(NULL, glob.tiny.cell_nb * glob.tiny.cell_sz,
					PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED
			|| (glob.small.mem = mmap(NULL, glob.small.cell_nb * glob.small.cell_sz,
					PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0)) == MAP_FAILED)
		return (FALSE);
	if (glob.debug > NO) {
		fprintf(glob.debug_out, DELIMITER);
		show_zone_param("TINY", &glob.tiny);
		show_zone_param("SMALL", &glob.small);
		fprintf(glob.debug_out, DELIMITER);
	}
	glob.init = TRUE;
	return (TRUE);
}
