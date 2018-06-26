/**
   @file    search_common.h
   @ingroup SearchAlgorithms
   @brief   Common functionality used by all Search Algorithms header
   @author  Isaias Compres
   @verbatim
    Revision:       $Revision$
    Revision date:  $Date$
    Committed by:   $Author$

    This file is part of the Periscope performance measurement tool.
    See http://www.lrr.in.tum.de/periscope for details.

    Copyright (c) 2005-2014, Technische Universitaet Muenchen, Germany
    See the COPYING file in the base directory of the package for details.
   @endverbatim
 */

#ifndef SEARCH_COMMON_H_
#define SEARCH_COMMON_H_

#include <list>
#include "ISearchAlgorithm.h"
#include "ScenarioResultsPool.h"
#include "ScenarioPoolSet.h"

extern "C" {
void setObjectiveFunction( double ( * f )( int, ScenarioResultsPool* ) );

//double objectiveFunction(list<MetaProperty*>);
double objectiveFunction( int scenario_id,
                          ScenarioResultsPool* );
}

double ptf_min( int scenario_id,
                ScenarioResultsPool* );

double ptf_max( int scenario_id,
                ScenarioResultsPool* );

double Objective_Function_InverseSpeedup( int scenario_id,
                                          ScenarioResultsPool* );

#endif
