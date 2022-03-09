/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:24:17 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/09 08:34:47 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

size_t		str_len(char *str, char del) {
	size_t		len = 0;
	
	for (len = 0; str[len] && str[len] != del; ++len);
	return (len);
}

char		sw_case(char c) {
	if (c >= 'a' && c <= 'z')
		return (c - ('a' - 'A'));
	else if (c >= 'A' && c <= 'Z')
		return (c + ('a' - 'A'));
	return (c);
}

t_lab_idx	labcmp_icase(char *val) {
	t_bool		beg = TRUE;
	t_bool		diff;
	size_t		i_sav;
	size_t		val_i;

	for (size_t i = 0;; ++i, ++val_i) {
		if (beg) {
			beg = FALSE;
			diff = FALSE;
			i_sav = i;
			val_i = 0;
		}
		if (!DEBUG_LAB[i] || DEBUG_LAB[i] == ',') {
			if (!diff)
				return(i_sav);
			if (!DEBUG_LAB[i])
				return (NO);
			beg = TRUE;
		} else if (DEBUG_LAB[i] != val[val_i] && DEBUG_LAB[i] != sw_case(val[val_i]))
			diff = TRUE;
	}
}

void		copy_n_bytes(uint8_t *dst, uint8_t *src, size_t n) {
	size_t		mod;

	if (!dst || !src)
		return;
	for (; n; n -= mod, dst += mod, src += mod) {
		if (n >= sizeof(uint64_t)) {
			*((uint64_t *)dst) = *((uint64_t *)src);
			mod = sizeof(uint64_t);
		} else if (n >= sizeof(uint32_t)) {
			*((uint32_t *)dst) = *((uint32_t *)src);
			mod = sizeof(uint32_t);
		} else if (n >= sizeof(uint16_t)) {
			*((uint16_t *)dst) = *((uint16_t *)src);
			mod = sizeof(uint16_t);
		} else if (n >= sizeof(uint8_t)) {
			*((uint8_t *)dst) = *((uint8_t *)src);
			mod = sizeof(uint8_t);
		}
	}
}

t_bool		in_zone(void *ptr, t_zone *zone) {
	return (ptr >= (void *)zone->mem && (uint8_t *)ptr
			< (uint8_t *)zone->mem + zone->cell_nb * zone->cell_sz);
}
