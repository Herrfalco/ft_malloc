/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 08:23:38 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/13 08:25:09 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/header.h"

static t_bool		in_zone(void *ptr, t_zone *zone) {
	t_hdr	*hdr = ptr;

	--hdr;
	return (!!(hdr >= zone->mem
				&& hdr < (t_hdr *)((uint8_t *)zone->mem
					+ zone->cell_nb * zone->cell_sz)
				&& !(((uint8_t *)hdr - (uint8_t *)zone->mem)
					% (sizeof(t_hdr) + zone->cell_sz)) ));
}

static t_bool		in_big(void *ptr) {
	t_big_hdr	*hdr = ((t_big_hdr *)ptr) - 1;
	t_big_hdr	*big;

	for (big = glob.big; big && big != hdr; big = big->next);
	return (!!big);
}

t_loc				wich_loc(void *ptr) {
	if (in_zone(ptr, &glob.tiny))
		return (TINY);
	else if (in_zone(ptr, &glob.small))
		return (SMALL);
	else if (in_big(ptr))
		return (BIG);
	return (OTHER);
}
