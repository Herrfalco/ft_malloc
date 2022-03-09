/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   const.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:27:01 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/09 08:27:33 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONST_H
#define CONST_H

#define MIN_CELL_NB			100
#define MIN_TNY_UP_LIM		128
#define MIN_SML_UP_LIM		4096
#define DELIMITER			"--------------------------------------------" \
							"--------------------------------------------\n"
#define DEBUG_ENV_VAR		"M_DEBUG"
#define DEBUG_ENV_FILE		"M_DEBUG_FILE"
#define DEBUG_LAB			"no," \
							"minimal," \
							"layout," \
							"full"
#define DEBUG_OP			"(+) Allocate," \
							"(-) Free," \
							"(^) Increase," \
							"(v) Decrease," \
							"(&) Reallocate"
#define BUFF_SIZE			128

typedef enum				e_lab_idx {
	NO = 0,
	MINIMAL = 3,
	LAYOUT = 11,
	FULL = 18,
}							t_lab_idx;

typedef enum				e_debug {
	ALLOC = 0,
	FREE = 13,
	INC = 22,
	DEC = 35,
	REALLOC = 48,
}							t_debug;

typedef enum				e_bool {
	FALSE,
	TRUE,
}							t_bool;

#endif //CONST_H
