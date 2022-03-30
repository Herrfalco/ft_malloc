/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:35:26 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/30 08:47:16 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/header.h"

static void		zone_param_iter(char *name, t_zone *zone, size_t state) {
	char		buff[BUFF_SIZE] = { 0 };

	switch (state) {
		case 0:
			str_push(buff, name, LEFT, -1, -1);
			str_push(buff, " zones composition:", LEFT, -1, -1);
			break;
		case 1:
			str_push(buff, LAYOUT_PAD + ZONE, LEFT, -1, str_len(LAYOUT_PAD + ZONE, ','));
			str_push_size(buff, sizeof(t_frame_hdr), LEFT, -1);
			str_push(buff, " bytes frame header", LEFT, -1, -1);
			break;
		case 2:
			str_push(buff, LAYOUT_PAD + NODE, LEFT, -1, str_len(LAYOUT_PAD + NODE, ','));
			str_push_size(buff, zone->cell_nb, LEFT, -1);
			str_push(buff, " cells:", LEFT, -1, -1);
			break;
		case 3:
			str_push(buff, LAYOUT_PAD + INDT, LEFT, -1, str_len(LAYOUT_PAD + INDT, ','));
			str_push_size(buff, sizeof(t_cell_hdr), LEFT, -1);
			str_push(buff, " bytes cell header", LEFT, -1, -1);
			break;
		case 4:
			str_push(buff, LAYOUT_PAD + UNODE, LEFT, -1, str_len(LAYOUT_PAD + UNODE, ','));
			str_push_size(buff, zone->cell_sz - sizeof(t_cell_hdr), LEFT, -1);
			str_push(buff, " bytes of data", LEFT, -1, -1);
			break;
		default:
			str_push(buff, LAYOUT_PAD + TOTAL, LEFT, -1, str_len(LAYOUT_PAD + TOTAL, ','));
			str_push_size(buff, sizeof(t_frame_hdr) + zone->cell_nb * zone->cell_sz, LEFT, -1);
			str_push(buff, " octets (", LEFT, -1, -1);
			str_push_size(buff, round_nb(zone->cell_nb * zone->cell_sz, getpagesize())
					/ getpagesize(), LEFT, -1);
			str_push(buff, " pages of ", LEFT, -1, -1);
			str_push_size(buff, getpagesize(), LEFT, -1);
	}
	str_print(buff, LEFT, 42, -1);
}

void		show_zone_param(void) {
	if (glob.show == NO)
		return;
	if (!glob.fline) {
		glob.fline = TRUE;
		str_print(DELIMITER, LEFT, -1, -1);
	}
	for (size_t i = 0; i < ZONE_LINES; ++i) {
		zone_param_iter("TINY", &glob.tiny, i);
		str_print(LAYOUT_PAD + ZONE, LEFT, -1, str_len(LAYOUT_PAD + ZONE, ','));
		zone_param_iter("SMALL", &glob.small, i);
		str_print("\n", LEFT, -1, -1);
	}
	str_print(DELIMITER, LEFT, -1, -1);
}

static void		show_dump(void *ptr, size_t size) {
	t_bool		init = TRUE;
	size_t		zero = 0;
	size_t		line = 0;
	uint8_t		byte;
	char		*pad = LAYOUT_PAD + DUMP;

	if (glob.debug != FULL)
		return;
	for (size_t i = 0; i < size; ++i) {
		byte = ((uint8_t *)ptr)[i];
		if (init || line == 16 || (zero && byte)) {
			if (init)
				init = FALSE;
			else {
				if (zero && byte) {
					str_print(".. [", LEFT, -1, -1);
					str_print_size(zero, LEFT, -1);
					str_print(" x 00]", LEFT, -1, -1);
					zero = 0;
				}
				str_print("\n", LEFT, -1, -1);
			}
			str_print(pad, LEFT, -1, str_len(pad, ','));
			str_print_size(i, LEFT, 5);
			str_print("  ", LEFT, -1, -1);
			line = 0;
		}
		if (byte) {
			++line;
			str_print_hex(byte, RIGHT, -1, 2);
			str_print("  ", LEFT, -1, -1);
		} else
			zero += 1;
	}
	if (zero || (!byte && size == 1)) {
		str_print(".. [", LEFT, -1, -1);
		str_print_size(zero, LEFT, -1);
		str_print(" x 00]", LEFT, -1, -1);
	}
	str_print("\n", LEFT, -1, -1);
}

static size_t		show_frame(char *title, t_zone *zone, t_frame_hdr *frame) {
	t_cell_hdr	*cell;
	uint8_t		*data;
	size_t		total = 0;
	char		*pad = LAYOUT_PAD + MEM;
	char		*padz = LAYOUT_PAD + ZONE;

	str_print(padz, LEFT, -1, str_len(padz, ','));
	str_print(title, LEFT, 5, -1);
	str_print(" : 0x", LEFT, -1, -1);
	str_print_hex((size_t)frame, LEFT, -1, -1);
	str_print("\n", LEFT, -1, -1);
	for (size_t i = 0; i < zone->cell_nb; ++i) {
		cell = (t_cell_hdr *)((uint8_t *)(frame + 1) + i * zone->cell_sz);
		if (cell->size) {
			data = (uint8_t *)(cell + 1);
			str_print(pad, LEFT, -1, str_len(pad, ','));
			str_print("0x", LEFT, -1, -1);
			str_print_hex((size_t)data, LEFT, -1, -1);
			str_print(" - 0x", LEFT, -1, -1);
			str_print_hex((size_t)(data + cell->size), LEFT, -1, -1);
			str_print(" : ", LEFT, -1, -1);
			str_print_size(cell->size, LEFT, -1);
			str_print(" octets\n", LEFT, -1, -1);
			show_dump(data, cell->size);	
			total += cell->size;
		}
	}
	return (total);
}

static size_t		show_big(t_big_hdr *new_max) {
	char			*pad = LAYOUT_PAD + MEM;

	str_print(pad, LEFT, -1, str_len(pad, ','));
	str_print("0x", LEFT, -1, -1);
	str_print_hex((size_t)(new_max + 1), LEFT, -1, -1);
	str_print(" - 0x", LEFT, -1, -1);
	str_print_hex((size_t)((uint8_t *)(new_max + 1) + new_max->size), LEFT, -1, -1);
	str_print(" : ", LEFT, -1, -1);
	str_print_size(new_max->size, LEFT, -1);
	str_print(" octets\n", LEFT, -1, -1);
	show_dump(new_max + 1, new_max->size);	
	return (new_max->size);
}

static size_t		rec_show_big(t_big_hdr *max) {
	char			*padz = LAYOUT_PAD + ZONE;
	t_big_hdr		*new_max = NULL;

	for (t_big_hdr *big = glob.big; big; big = big->next)
		if ((!max || big < max) && big > new_max)
			new_max = big;
	if (!new_max)
		return (0);
	if (!max) {
		str_print(padz, LEFT, -1, str_len(padz, ','));
		str_print("LARGE : 0x", LEFT, -1, -1);
		str_print_hex((size_t)glob.big, LEFT, -1, -1);
		str_print("\n", LEFT, -1, -1);
	}
	return (rec_show_big(new_max) + show_big(new_max));
}

static size_t		rec_show_frames(t_frame_hdr *max) {
	char			*titles[2] = { "TINY", "SMALL" };
	t_zone			*zones[2] = { &glob.tiny, &glob.small };
	t_frame_hdr		*new_max = NULL;
	size_t			z_idx = 0;

	for (size_t i = 0; i < 2; ++i) {
		for (t_frame_hdr *frame = zones[i]->frame; frame; frame = frame->next) {
			if ((!max || frame < max) && frame > new_max) {
				new_max = frame;
				z_idx = i;
			}
		}
	}
	return (new_max ? rec_show_frames(new_max)
		+ show_frame(titles[z_idx], zones[z_idx], new_max) : 0);
}

static void			show_layout(t_debug deb) {
	char			*padt = LAYOUT_PAD + TOTAL;
	size_t			total = 0;

	if (glob.debug == MINIMAL && deb != DISP)
		return;
	total += rec_show_frames(NULL);
	total += rec_show_big(NULL);
	str_print(padt, LEFT, -1, str_len(padt, ','));
	str_print("Total = ", LEFT, -1, -1);
	str_print_size(total, LEFT, -1);
	str_print(" octets\n", LEFT, -1, -1);
}

void			show_deb(t_debug deb, t_bool res, size_t s1, size_t s2, void *p1, void *p2) {
	char		buff[BUFF_SIZE] = { 0 };;

	if (glob.show == NO || (glob.show == MANUAL && deb != DISP))
		return;
	if (!glob.fline) {
		glob.fline = TRUE;
		str_print(DELIMITER, LEFT, -1, -1);
	}
	str_print(DEBUG_OP + deb, LEFT, 15, str_len(DEBUG_OP + deb, ','));
	if (res) {
		if (deb == ALLOC || deb == FREE) {
			str_push_size(buff, s1, LEFT, -1);
			str_push(buff, " byte(s)", LEFT, -1, -1);
		} else if (deb != DISP) {
			str_push_size(buff, s1, LEFT, -1);
			str_push(buff, " -> ", LEFT, -1, -1);
			str_push_size(buff, s2, LEFT, -1);
			str_push(buff, " byte(s)", LEFT, -1, -1);
		}
		str_print(buff, RIGHT, 33, -1);
		buff[0] = '\0';
		if (deb == REALLOC) {
			str_push(buff, "0x", LEFT, -1, -1);
			str_push_hex(buff, (size_t)p1, LEFT, -1, -1);
			str_push(buff, " -> 0x", LEFT, -1, -1);
			str_push_hex(buff, (size_t)p2, LEFT, -1, -1);
		} else if (deb != DISP) {
			str_push(buff, "0x", LEFT, -1, -1);
			str_push_hex(buff, (size_t)p1, LEFT, -1, -1);
		}
		str_print(buff, RIGHT, 40, -1);
		str_print("\n", LEFT, -1, -1);
		show_layout(deb);
	} else {
		str_push(buff, "FAILED", LEFT, -1, -1);
		str_print(buff, RIGHT, 33, -1);
		str_print("\n", LEFT, -1, -1);
	}
	str_print(DELIMITER, LEFT, -1, -1);
}

void			show_alloc_mem(void) {
	show_deb(DISP, TRUE, 0, 0, NULL, NULL);
}
