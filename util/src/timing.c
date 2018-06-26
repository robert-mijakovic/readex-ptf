/**
   @file    timing.c
   @brief   Periscope timing routines
   @author  Karl Fuerlinger
   @verbatim
    Revision:       $Revision$
    Revision date:  $Date$
    Committed by:   $Author$

    This file is part of the Periscope performance measurement tool.
    See http://www.lrr.in.tum.de/periscope for details.

    Copyright (c) 2005-2011, Technische Universitaet Muenchen, Germany
    See the COPYING file in the base directory of the package for details.
   @endverbatim
 */

#include "timing.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


double psc_start_time;

double psc_wall_time( void ) {
    double current_time;

    PSC_WTIME( current_time );
    return current_time - psc_start_time;
}

void psc_init_start_time( void ) {
    PSC_WTIME( psc_start_time );
}
