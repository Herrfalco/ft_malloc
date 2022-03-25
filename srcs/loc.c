/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loc.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/13 08:23:38 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/25 16:08:06 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/header.h"

static t_bool		in_zone(t_cell_hdr *cell, t_zone *zone, t_frame_hdr **frame) {
	t_cell_hdr		*frame_cells;

	for (*frame = zone->frame; *frame; *frame = (*frame)->next) {
		frame_cells = (t_cell_hdr *)(*frame + 1);
		if ((cell >= frame_cells
					&& cell < (t_cell_hdr *)((uint8_t *)frame_cells
						+ zone->cell_nb * zone->cell_sz))
				&& !(((uint8_t *)cell - (uint8_t *)frame_cells) % zone->cell_sz))
			return (TRUE);
	}
	return (FALSE);
}

static t_bool		in_big(void *ptr) {
	t_big_hdr	*cell = ((t_big_hdr *)ptr) - 1;
	t_big_hdr	*big;

	for (big = glob.big; big && big != cell; big = big->next);
	return (!!big);
}

t_loc				wich_loc(void *ptr, t_frame_hdr **frame) {
	t_cell_hdr		*cell = ((t_cell_hdr *)ptr) - 1;

	if (in_zone(cell, &glob.tiny, frame))
		return (cell->size ? TINY : OTHER);
	else if (in_zone(cell, &glob.small, frame))
		return (cell->size ? SMALL : OTHER);
	else if (in_big(ptr))
		return (BIG);
	return (OTHER);
}
