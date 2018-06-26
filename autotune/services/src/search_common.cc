/**
   @file    search_common.cc
   @ingroup SearchAlgorithms
   @brief   Common functionality used by all Search Algorithms
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

#include "search_common.h"

double ( * ptfObjectiveFunction )( int, ScenarioResultsPool* );

double ptf_min( int                  scenario_id,
                ScenarioResultsPool* srp ) {
    double minimum = 0.0;
    try {
        list<MetaProperty> properties = srp->getScenarioResultsByID( scenario_id );
        double             current;
        minimum = properties.front().getSeverity();

        std::list<MetaProperty>::iterator iterator;
        for( iterator = properties.begin(); iterator != properties.end(); ++iterator ) {
            current = iterator->getSeverity();
            if( minimum > current ) {
                minimum = current;
            }
            psc_dbgmsg( PSC_SELECTIVE_DEBUG_LEVEL( AutotuneSearch ), "ptf_min: %f; current: %f;\n", minimum, current );
        }
    }
    catch( const std::out_of_range& oor ) {
        psc_dbgmsg( 0, "ptf_min: no properties\n" );
    }
    return minimum;
}

double ptf_max( int                  scenario_id,
                ScenarioResultsPool* srp ) {
    double maximum = 0.0;
    try {
        list<MetaProperty> properties = srp->getScenarioResultsByID( scenario_id );
        double             current;
        maximum = properties.front().getSeverity();

        psc_dbgmsg( PSC_SELECTIVE_DEBUG_LEVEL( AutotuneSearch ), "ptf_min: %f; current: %f;\n", maximum, current );

        std::list<MetaProperty>::iterator iterator;
        for( iterator = properties.begin(); iterator != properties.end(); ++iterator ) {
            current = iterator->getSeverity();
            if( maximum < current ) {
                maximum = current;
            }
            psc_dbgmsg( PSC_SELECTIVE_DEBUG_LEVEL( AutotuneSearch ), "ptf_min: %f; current: %f;\n", maximum, current );
        }
    }
    catch( const std::out_of_range& oor ) {
        psc_dbgmsg( 0, "ptf_max: no properties\n" );
    }
    return maximum;
}

//! [Objective_Function_InverseSpeedup]
double Objective_Function_InverseSpeedup( int                  scenario_id,
                                          ScenarioResultsPool* srp ) {
    list<MetaProperty> properties      = srp->getScenarioResultsByID( scenario_id );
    list<MetaProperty> base_properties = srp->getScenarioResultsByID( 0 );

    // base execution time for single thread on every process
    vector<double> base_ExecTime;
    double         ExecTime   = 1.0;
    double         invSpeedup = 0.0;

    // since we pushed a single property , the value will be execTime
    for( std::list<MetaProperty>::iterator iterator = base_properties.begin(),
         end = base_properties.end(); iterator != end; ++iterator ) {
        base_ExecTime.push_back( ( iterator )->getSeverity() );
    }

    if( scenario_id == 0 ) {
        invSpeedup = 1.0;
    }
    else {
        // execution time for current scenario
        for( std::list<MetaProperty>::iterator iterator = properties.begin(),
             end = properties.end(); iterator != end; ++iterator ) {
            ExecTime = iterator->getSeverity();
        }
        invSpeedup = ExecTime / base_ExecTime[ properties.front().getProcess() ];
    }
    psc_dbgmsg( PSC_SELECTIVE_DEBUG_LEVEL( AutotuneSearch ), "InverseSpeedup: %f;\n", invSpeedup );

    return invSpeedup; // returning inverse speedup
}
//! [Objective_Function_InverseSpeedup]

void setObjectiveFunction( double ( * f )( int, ScenarioResultsPool* ) ) {
    ptfObjectiveFunction = f;
}

double objectiveFunction( int                  scenario_id,
                          ScenarioResultsPool* srp ) {
    if( ptfObjectiveFunction == NULL ) {
        perror( "objectiveFunction() not set\n" );
        throw 0;
    }
    return ( *ptfObjectiveFunction )( scenario_id, srp );
}

ISearchAlgorithm::ISearchAlgorithm() : search_steps( 0 ) {
    setObjectiveFunction( ptf_min );
}

ISearchAlgorithm::~ISearchAlgorithm() {
}
