/**
   @file    EnergyStrategy.h
   @ingroup EnergyStrategy
   @brief   Sandy/Ivy bridge RAPL counter energy search strategy
   @author  Robert Mijakovic
   @verbatim
    Revision:       $Revision$
    Revision date:  $Date$
    Committed by:   $Author$

    This file is part of the Periscope performance measurement tool.
    See http://www.lrr.in.tum.de/periscope for details.

    Copyright (c) 2005-2013, Technische Universitaet Muenchen, Germany
    See the COPYING file in the base directory of the package for details.
   @endverbatim
 */

/**
 * @defgroup EnergyStrategy Energy Analysis Strategy
 * @ingroup Strategies
 */

#ifndef ENERGYSTRATEGY_H_
#define ENERGYSTRATEGY_H_
#include "global.h"
#include "application.h"
#include "Metric.h"
#include "strategy.h"


class EnergyStrategy : public Strategy {
private:
    Region*   phaseRegion;
    Prop_List candProperties;
    Prop_List foundPropertiesLastStep;

    //Only the subregions of one evaluated propertie's context are measured in the
    //next step.  the other evaluated Properties are stocked in this queue or in
    //the call_queue
    Prop_List propsRefineRegionNesting;
    //The list of already checked subroutines
    std::map <Region*, bool> checkedSubs;


public:
    EnergyStrategy( bool pedantic = false ) {
    }
    ~EnergyStrategy() {
    }
    std::list <Property*>create_initial_candidate_properties_set( Region* initial_region );

    std::list <Property*>create_next_candidate_properties_set( std::list< Property* > ev_set );
    std::string name();


    bool reqAndConfigureFirstExperiment( Region* initial_region ); // TRUE can start; FALSE not ready
    bool evaluateAndReqNextExperiment();                           // TRUE requires next step; FALSE if done
    void configureNextExperiment();
};

#endif /*ENERGYSTRATEGY_H_*/
