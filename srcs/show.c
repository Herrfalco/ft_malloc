/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   show.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:35:26 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/13 14:02:49 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/header.h"

void		show_zone_param(char *name, t_zone *zone) {
	char		buff[BUFF_SIZE];

	sprintf(buff, "%s:", name);
	fprintf(STDOUT, "%-7s", buff);
	sprintf(buff, "%lu cells of %lu octets (%lu + %lu)", zone->cell_nb, zone->cell_sz,
			sizeof(t_hdr), zone->cell_sz - sizeof(t_hdr));
	fprintf(STDOUT, "%-40s", buff);
	sprintf(buff, "%lu octets (%lu pages of %d)", zone->cell_nb * zone->cell_sz,
			zone->cell_nb * zone->cell_sz / getpagesize(), getpagesize());
	fprintf(STDOUT, "=%40s\n", buff);
}

static void		show_dump(void *ptr, size_t size) {
	t_bool		init = TRUE;
	size_t		zero = 0;
	size_t		line;
	uint8_t		byte;
	char		*pad = LAYOUT_PAD + DUMP;

	if (glob.debug < FULL)
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
		if (!byte)
			zero += 1;
		else {
			++line;
			fprintf(STDOUT, "%02x ", byte);
		}
	}
	if (zero > 1)
		fprintf(STDOUT, ".. [%lu x 00]", zero);
	fprintf(STDOUT, "\n");
	(void)line;
}

static size_t		show_alloc_zone(t_zone *zone) {
	t_hdr		*hdr;
	uint8_t		*data;
	size_t		size;
	size_t		total = 0;
	char		*pad = LAYOUT_PAD + MEM;

	for (size_t i = 0; i < zone->cell_nb; ++i) {
		hdr = (t_hdr *)(((uint8_t *)zone->mem) + i * zone->cell_sz);
		size = ((t_hdr *)hdr)->size;
		if (size) {
			data = (uint8_t *)(hdr + 1);
			fprintf(STDOUT, "%.*s%p - %p : %lu octets\n",
				(int)str_len(pad, ','), pad, data, data + size, size);
			show_dump(data, size);	
			total += size;
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
	size_t		total = 0;
	char		*padz = LAYOUT_PAD + ZONE;
	char		*padt = LAYOUT_PAD + TOTAL;
	int			padz_prec = (int)str_len(padz, ',');

	if (glob.debug < LAYOUT)
		return;
	fprintf(STDOUT, "%.*sTINY  : %p\n", padz_prec, padz, glob.tiny.mem);
	if (glob.tiny.mem)
		total += show_alloc_zone(&glob.tiny);
	fprintf(STDOUT, "%.*sSMALL : %p\n", padz_prec, padz, glob.small.mem);
	if (glob.small.mem)
		total += show_alloc_zone(&glob.small);
	fprintf(STDOUT, "%.*sLARGE : %p\n", padz_prec, padz, glob.big);
	if (glob.big)
		total += show_big_alloc();
	fprintf(STDOUT, "%.*sTotal = %lu octets\n",
			(int)str_len(padt, ','), padt, total);
}

void		show_deb(t_debug deb, t_bool res, size_t s1, size_t s2, void *p1, void *p2) {
	char		buff[BUFF_SIZE];

	if (glob.debug < MINIMAL)
		return;
	fprintf(STDOUT, "%-15.*s", (int)str_len(DEBUG_OP + deb, ','), DEBUG_OP + deb);
	if (res) {
		if (deb == ALLOC || deb == FREE)
			sprintf(buff, "%ld byte(s)", s1);
		else if (deb == REALLOC)
			sprintf(buff, "%ld -> %ld byte(s)", s1, s2);
		else
			buff[0] = '\0';
		fprintf(STDOUT, "%33s", buff);
		if (deb == ALLOC || deb == FREE)
			sprintf(buff, "%p", p1);
		else if (deb == REALLOC)
			sprintf(buff, "%p -> %p", p1, p2);
		else
			buff[0] = '\0';
		fprintf(STDOUT, "%40s\n", buff);
		show_layout();
	} else {
		sprintf(buff, "FAILED");
		fprintf(STDOUT, "%33s\n", buff);
	}
	if (glob.debug > MINIMAL)
		fprintf(STDOUT, DELIMITER);
}

void		show_alloc_mem(void) {
	show_deb(DISP, TRUE, 0, 0, NULL, NULL);
}
