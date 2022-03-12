/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:35:26 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/12 20:13:18 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/header.h"

void		show_zone_param(char *name, t_zone *zone) {
	char		buff[BUFF_SIZE];

	sprintf(buff, "%s:", name);
	fprintf(glob.debug_out, "%-7s", buff);
	sprintf(buff, "%lu cells of %lu octets (%lu + %lu)", zone->cell_nb, zone->cell_sz,
			sizeof(t_hdr), zone->cell_sz - sizeof(t_hdr));
	fprintf(glob.debug_out, "%-40s", buff);
	sprintf(buff, "%lu octets (%lu pages of %d)", zone->cell_nb * zone->cell_sz,
			zone->cell_nb * zone->cell_sz / getpagesize(), getpagesize());
	fprintf(glob.debug_out, "=%40s\n", buff);
}

void		show_dump(void *ptr, size_t size) {
	t_bool		init = TRUE;
	size_t		zero = 0;
	size_t		line;
	uint8_t		byte;
	char		*pad = LAYOUT_PAD + DUMP;
	int			pad_prec = (int)str_len(pad, ',');

	if (glob.debug < FULL)
		return;
	for (size_t i = 0, line = 0; i < size; ++i) {
		byte = ((uint8_t *)ptr)[i];
		if (init || line == 24 || (zero > 1 && byte)) {
			if (init)
				init = FALSE;
			else {
				if (zero > 1 && byte) {
					fprintf(glob.debug_out, ".. [%lu x 00]", zero);
					zero = 0;
				}
				fprintf(glob.debug_out, "\n");
			}
			fprintf(glob.debug_out, "%s%-5lu  ", pad, i);
			line = 0;
		}
		if (!byte)
			zero += 1;
		else {
			++line;
			fprintf(glob.debug_out, "%02x ", byte);
		}
	}
	if (zero > 1)
		fprintf(glob.debug_out, ".. [%lu x 00]", zero);
	fprintf(glob.debug_out, "\n");
	(void)line;
}

size_t		show_alloc_zone(t_zone *zone, t_bool dump) {
	t_hdr		*hdr;
	uint8_t		*data;
	size_t		size;
	size_t		total = 0;
	char		*pad = LAYOUT_PAD + MEM;
	int			pad_prec = (int)str_len(pad, ',');

	for (size_t i = 0; i < zone->cell_nb; ++i) {
		hdr = (t_hdr *)(((uint8_t *)zone->mem) + i * zone->cell_sz);
		size = ((t_hdr *)hdr)->size;
		if (size) {
			data = (uint8_t *)(hdr + 1);
			fprintf(glob.debug_out, "%.*s%p - %p : %lu octets\n", pad_prec, pad,
				data, data + size, size);
			if (dump)
				show_dump(data, size);	
			total += size;
		}
	}
	return (total);
}

size_t		show_big_alloc(t_bool dump) {
	size_t		total = 0;
	char		*pad = LAYOUT_PAD + MEM;
	int			pad_prec = (int)str_len(pad, ',');

	for (t_big_hdr *ptr = glob.big; ptr; ptr = ptr->next) {
		fprintf(glob.debug_out, "%.*ss%p - %p : %lu octets\n", pad_prec, pad, ptr + 1,
			ptr + 1 + ptr->size, ptr->size);
		if (dump)
			show_dump(ptr + 1, ptr->size);	
		total += ptr->size;
	}
	return (total);
}

void		show_layout(t_bool dump) {
	size_t		total = 0;
	char		*padz = LAYOUT_PAD + ZONE;
	char		*padt = LAYOUT_PAD + TOTAL;
	int			padz_prec = (int)str_len(padz, ',');
	int			padt_prec = (int)str_len(padt, ',');

	if (glob.debug < LAYOUT)
		return;
	fprintf(glob.debug_out, "%.*sTINY  : %p\n", padz_prec, padz, glob.tiny.mem);
	if (glob.tiny.mem)
		total += show_alloc_zone(&glob.tiny, dump);
	fprintf(glob.debug_out, "%.*sSMALL : %p\n", padz_prec, padz, glob.small.mem);
	if (glob.small.mem)
		total += show_alloc_zone(&glob.small, dump);
	fprintf(glob.debug_out, "%.*sLARGE : %p\n", padz_prec, padz, glob.big);
	if (glob.big)
		total += show_big_alloc(dump);
	fprintf(glob.debug_out, "%.*sTotal = %lu octets\n", padt_prec, padt, total);
}

void		show_alloc_mem(void) {
	show_layout(FALSE);
}

void		show_deb(t_debug deb, t_bool res, size_t s1, size_t s2, void *p1, void *p2) {
	char		buff[BUFF_SIZE];

	if (glob.debug < MINIMAL)
		return;
	fprintf(glob.debug_out, "%-15.*s", (int)str_len(DEBUG_OP + deb, ','), DEBUG_OP + deb);
	if (!res)
		return ((void)fprintf(glob.debug_out, "FAILED\n"));
	if (deb == ALLOC || deb == FREE)
		sprintf(buff, "%ld byte(s)", s1);
	else
		sprintf(buff, "%ld -> %ld byte(s)", s1, s2);
	fprintf(glob.debug_out, "%33s", buff);
	if (deb == REALLOC)
		sprintf(buff, "%p -> %p", p1, p2);
	else
		sprintf(buff, "%p", p1);
	fprintf(glob.debug_out, "%40s\n", buff);
	show_layout(TRUE);
	fprintf(glob.debug_out, DELIMITER);
}
