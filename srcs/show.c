/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:35:26 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/25 20:22:12 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/header.h"

static void		zone_param_iter(char *name, t_zone *zone, size_t state) {
	char		buff[BUFF_SIZE];

	switch (state) {
		case 0:
			sprintf(buff, "%s zones composition:", name);
			break;
		case 1:
			sprintf(buff, "%.*s%lu bytes frame header",
				(int)str_len(LAYOUT_PAD + ZONE, ','), LAYOUT_PAD + ZONE, sizeof(t_frame_hdr));
			break;
		case 2:
			sprintf(buff, "%.*s%lu cells:",
				(int)str_len(LAYOUT_PAD + NODE, ','), LAYOUT_PAD + NODE, zone->cell_nb);
			break;
		case 3:
			sprintf(buff, "%.*s%lu bytes cell header",
				(int)str_len(LAYOUT_PAD + INDT, ','), LAYOUT_PAD + INDT, sizeof(t_cell_hdr));
			break;
		case 4:
			sprintf(buff, "%.*s%lu bytes of data",
				(int)str_len(LAYOUT_PAD + UNODE, ','), LAYOUT_PAD + UNODE,
				zone->cell_sz - sizeof(t_cell_hdr));
			break;
		default:
			sprintf(buff, "%.*s%lu octets (%lu pages of %d bytes)",
					(int)str_len(LAYOUT_PAD + TOTAL, ','), LAYOUT_PAD + TOTAL,
					sizeof(t_frame_hdr) + zone->cell_nb * zone->cell_sz,
					zone->cell_nb * zone->cell_sz / getpagesize()
					+ !!(zone->cell_nb * zone->cell_sz % getpagesize()),
					getpagesize());
	}
	fprintf(STDOUT, "%-42s", buff);
}

void		show_zone_param(void) {
	if (!glob.fline) {
		glob.fline = TRUE;
		fprintf(STDOUT, DELIMITER);
	}
	for (size_t i = 0; i < ZONE_LINES; ++i) {
		zone_param_iter("TINY", &glob.tiny, i);
		fprintf(STDOUT, "|  ");
		zone_param_iter("SMALL", &glob.small, i);
		fprintf(STDOUT, "\n");
	}
	fprintf(STDOUT, DELIMITER);
}

static void		show_dump(void *ptr, size_t size) {
	t_bool		init = TRUE;
	size_t		zero = 0;
	size_t		line;
	uint8_t		byte;
	char		*pad = LAYOUT_PAD + DUMP;

	if (glob.debug != FULL)
		return;
	for (size_t i = 0, line = 0; i < size; ++i) {
		byte = ((uint8_t *)ptr)[i];
		if (init || line == 24 || (zero > 1 && byte)) {
			if (init)
				init = FALSE;
			else {
				if (zero > 1 && byte) {
					fprintf(STDOUT, ".. [%lu x 00]", zero);
					zero = 0;
				}
				fprintf(STDOUT, "\n");
			}
			fprintf(STDOUT, "%.*s%-5lu  ", (int)str_len(pad, ','), pad, i);
			line = 0;
		}
		if (byte) {
			++line;
			fprintf(STDOUT, "%02x ", byte);
		} else
			zero += 1;
	}
	if (zero || (!byte && size == 1))
		fprintf(STDOUT, ".. [%lu x 00]", zero);
	fprintf(STDOUT, "\n");
	(void)line;
}

static size_t		show_frame(t_zone *zone, t_frame_hdr *frame) {
	t_cell_hdr	*cell;
	uint8_t		*data;
	size_t		total = 0;
	char		*pad = LAYOUT_PAD + MEM;

	for (size_t i = 0; i < zone->cell_nb; ++i) {
		cell = (t_cell_hdr *)((uint8_t *)(frame + 1) + i * zone->cell_sz);
		if (cell->size) {
			data = (uint8_t *)(cell + 1);
			fprintf(STDOUT, "%.*s%p - %p : %lu octets\n", (int)str_len(pad, ','),
				pad, data, data + cell->size, cell->size);
			show_dump(data, cell->size);	
			total += cell->size;
		}
	}
	return (total);
}

static size_t		show_big_alloc(void) {
	size_t		total = 0;
	char		*pad = LAYOUT_PAD + MEM;

	for (t_big_hdr *ptr = glob.big; ptr; ptr = ptr->next) {
		fprintf(STDOUT, "%.*s%p - %p : %lu octets\n",
			(int)str_len(pad, ','), pad, ptr + 1, ptr + 1 + ptr->size, ptr->size);
		show_dump(ptr + 1, ptr->size);	
		total += ptr->size;
	}
	return (total);
}

static void		show_layout(void) {
	size_t			total = 0;
	char			*padz = LAYOUT_PAD + ZONE;
	char			*padt = LAYOUT_PAD + TOTAL;
	int				padz_prec = (int)str_len(padz, ',');

	if (glob.debug == MINIMAL)
		return;
	for (t_frame_hdr *frame = glob.tiny.frame; frame; frame = frame->next) {
		fprintf(STDOUT, "%.*sTINY  : %p\n", padz_prec, padz, frame);
		total += show_frame(&glob.tiny, frame);
	}
	for (t_frame_hdr *frame = glob.small.frame; frame; frame = frame->next) {
		fprintf(STDOUT, "%.*sSMALL : %p\n", padz_prec, padz, frame);
		total += show_frame(&glob.small, frame);
	}
	if (glob.big) {
		fprintf(STDOUT, "%.*sLARGE : %p\n", padz_prec, padz, glob.big);
		total += show_big_alloc();
	}
	fprintf(STDOUT, "%.*sTotal = %lu octets\n",
			(int)str_len(padt, ','), padt, total);
}

void		show_deb(t_debug deb, t_bool res, size_t s1, size_t s2, void *p1, void *p2) {
	char		buff[BUFF_SIZE];

	if (glob.show == MANUAL && deb != DISP)
		return;
	if (!glob.fline) {
		glob.fline = TRUE;
		fprintf(STDOUT, DELIMITER);
	}
	fprintf(STDOUT, "%-15.*s", (int)str_len(DEBUG_OP + deb, ','), DEBUG_OP + deb);
	if (res) {
		if (deb == ALLOC || deb == FREE)
			sprintf(buff, "%ld byte(s)", s1);
		else if (deb == DISP)
			buff[0] = '\0';
		else
			sprintf(buff, "%ld -> %ld byte(s)", s1, s2);
		fprintf(STDOUT, "%33s", buff);
		if (deb == REALLOC)
			sprintf(buff, "%p -> %p", p1, p2);
		else if (deb == DISP)
			buff[0] = '\0';
		else
			sprintf(buff, "%p", p1);
		fprintf(STDOUT, "%40s\n", buff);
		show_layout();
	} else {
		sprintf(buff, "FAILED");
		fprintf(STDOUT, "%33s\n", buff);
	}
	if (glob.debug != MINIMAL)
		fprintf(STDOUT, DELIMITER);
}

void		show_alloc_mem(void) {
	show_deb(DISP, TRUE, 0, 0, NULL, NULL);
}
