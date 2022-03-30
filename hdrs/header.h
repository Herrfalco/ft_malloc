/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   header.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fcadet <fcadet@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/09 08:26:00 by fcadet            #+#    #+#             */
/*   Updated: 2022/03/29 21:10:20 by fcadet           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HEADER_H
#define HEADER_H

#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <pthread.h>

#include "const.h"
#include "struct.h"
#include "proto.h"

extern t_glob		glob;

#endif //HEADER_H
