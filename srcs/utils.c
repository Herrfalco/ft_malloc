/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:24:17 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/29 20:48:16 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/header.h"

char		sw_case(char c) {
	if (c >= 'a' && c <= 'z')
		return (c - ('a' - 'A'));
	else if (c >= 'A' && c <= 'Z')
		return (c + ('a' - 'A'));
	return (c);
}

t_lab_idx	labcmp_icase(char *val, char *labs) {
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
		if (!labs[i] || labs[i] == ',') {
			if (!diff)
				return(i_sav);
			if (!labs[i])
				return (DEFAULT);
			beg = TRUE;
		} else if (labs[i] != val[val_i] && labs[i] != sw_case(val[val_i]))
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

size_t		round_nb(size_t nb, size_t delta) {
	return ((nb / delta + !!(nb % delta)) * delta);
}
