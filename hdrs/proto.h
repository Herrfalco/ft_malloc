/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proto.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:31:48 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/09 08:35:02 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTO_H
#define PROTO_H

///////////////// Utils.h //////////////////

size_t		str_len(char *str, char del);
char		sw_case(char c);
t_lab_idx	labcmp_icase(char *val);
void		copy_n_bytes(uint8_t *dst, uint8_t *src, size_t n);
t_bool		in_zone(void *ptr, t_zone *zone);

#endif //PROTO_H
