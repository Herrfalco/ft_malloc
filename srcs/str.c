/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 20:47:56 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/29 20:48:39 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../hdrs/header.h"

size_t		str_len(char *str, char del) {
	size_t		len = 0;
	
	for (len = 0; str[len] && str[len] != del; ++len);
	return (len);
}

void		str_push(char *dst, char *src, t_just just, ssize_t width, ssize_t prec) {
	size_t		size = str_len(src, 0);
	size_t		fill;

	if (prec >= 0 && (size_t)prec < size)
		size = (size_t)prec;
	fill = width >= 0 && size < (size_t)width ? (size_t)width - size : 0;
	for (; *dst; ++dst);
	if (just == RIGHT)
		for (; fill; --fill)
			*(dst++) = ' ';	
	for (; size; --size)
		*(dst++) = *(src++);
	if (just == LEFT)
		for (; fill; --fill)
			*(dst++) = ' ';	
	*dst = '\0';
}

void		str_print(char *str, t_just just, ssize_t width, ssize_t prec) {
	char		buff[BUFF_SIZE] = { 0 };

	str_push(buff, str, just, width, prec);
	write(STDOUT, buff, str_len(buff, 0));
}

static void	rec_push_nb(char **dst, size_t nb, t_bool hex) {
	size_t		base = hex ? 16 : 10;

	if (nb / base)
		rec_push_nb(dst, nb / base, hex);
	**dst = nb % base + '0';
	if (**dst > '9')
		**dst = nb % base - 10 + 'a';
	++(*dst);
}

void		str_push_size(char *dst, size_t nb, t_just just, ssize_t width) {
	char		tmp[BUFF_SIZE] = { 0 };
	char		*ptr = tmp;
	
	rec_push_nb(&ptr, nb, FALSE);	
	str_push(dst, tmp, just, width, -1);
}

void		str_print_size(size_t nb, t_just just, ssize_t width) {
	char		buff[BUFF_SIZE] = { 0 };

	str_push_size(buff, nb, just, width);
	write(STDOUT, buff, str_len(buff, 0));
}

void		str_push_hex(char *dst, size_t nb, t_just just, ssize_t width, ssize_t prec) {
	char		tmp[BUFF_SIZE] = { 0 };
	char		*ptr = tmp;
	size_t		len;
	
	rec_push_nb(&ptr, nb, TRUE);	
	len = str_len(tmp, 0);
	if (prec >= 0 && (size_t)prec > len)
		for (size_t fill = prec - len; fill; --fill)
			str_push(dst, "0", LEFT, -1, -1);
	str_push(dst, tmp, just, width, -1);
}

void		str_print_hex(size_t nb, t_just just, ssize_t width, ssize_t prec) {
	char		buff[BUFF_SIZE] = { 0 };

	str_push_hex(buff, nb, just, width, prec);
	write(STDOUT, buff, str_len(buff, 0));
}
