/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 08:23:38 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/13 14:29:34 by fcadet           ###   ########.fr       */
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
					% zone->cell_sz)));
}

static t_bool		in_big(void *ptr) {
	t_big_hdr	*hdr = ((t_big_hdr *)ptr) - 1;
	t_big_hdr	*big;

	for (big = glob.big; big && big != hdr; big = big->next);
	return (!!big);
}

t_loc				wich_loc(void *ptr) {
	t_hdr		*hdr = ((t_hdr *)ptr) - 1;

/*
	fprintf(STDOUT, "%s\n", in_zone(ptr, &glob.tiny) ? "in_tiny" : "not_in_tiny");
	fprintf(STDOUT, "%s\n", in_zone(ptr, &glob.small) ? "in_small" : "not_in_small");
	*/
	if (in_zone(ptr, &glob.tiny))
		return (hdr->size ? TINY : OTHER);
	else if (in_zone(ptr, &glob.small))
		return (hdr->size ? SMALL : OTHER);
	else if (in_big(ptr))
		return (BIG);
	return (OTHER);
}
