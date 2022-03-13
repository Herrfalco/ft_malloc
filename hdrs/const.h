/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   const.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:27:01 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/13 14:57:16 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONST_H
#define CONST_H

#define MIN_CELL_NB			100
#define MIN_TNY_UP_LIM		128
#define MIN_SML_UP_LIM		4096
#define DELIMITER			"--------------------------------------------" \
							"--------------------------------------------\n"
#define LAYOUT_PAD			" |  ," \
							"    -  ," \
							"        #," \
							" >  "
#define DEBUG_ENV_VAR		"M_DEBUG"
#define DEBUG_LAB			"no," \
							"minimal," \
							"layout," \
							"full"
#define DEBUG_OP			"(+) Allocate," \
							"(-) Free," \
							"(^) Increase," \
							"(v) Decrease," \
							"(&) Reallocate,"\
							"(*) Display"
#define BUFF_SIZE			128
#define STDOUT				stderr

typedef enum				e_loc {
	OTHER,
	TINY,
	SMALL,
	BIG,
}							t_loc;

typedef enum				e_pad {
	ZONE = 0,
	MEM = 5,	
	DUMP = 13,
	TOTAL = 23,
}							t_pad;

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
	DISP = 63,
}							t_debug;

typedef enum				e_bool {
	FALSE,
	TRUE,
}							t_bool;

#endif //CONST_H
