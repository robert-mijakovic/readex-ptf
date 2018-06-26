/**
   @file    ISearchAlgorithm.h
   @ingroup SearchAlgorithms
   @brief   Search Algorithm interface
   @author  Houssam Haitof
   @verbatim
    Revision:       $Revision$
    Revision date:  $Date$
    Committed by:   $Author$

    This file is part of the Periscope Tuning Framework.
    See http://www.lrr.in.tum.de/periscope for details.

    Copyright (c) 2005-2014, Technische Universitaet Muenchen, Germany
    See the COPYING file in the base directory of the package for details.
   @endverbatim
 */


/**
 * @defgroup SearchAlgorithms Search Algorithms
 * @ingroup Autotune
 */

#ifndef ISEARCHALGORITHM_H_
#define ISEARCHALGORITHM_H_

#include "VariantSpace.h"
#include "Variant.h"
#include "Scenario.h"
#include "selective_debug.h"
#include "psc_errmsg.h"
#include "SearchSpace.h"
#include "Restriction.h"
#include "search_common.h"
#include "DriverContext.h"
#include "ScenarioResultsPool.h"
//#include "frontend.h"

class DriverContext;

// STL includes
#include <stdlib.h>
#include <iostream>
#include <list>
#include <queue>
#include <vector>

// PSC includes
//#include <application.h>

//class DriverContext;
class ProgramSignature;

#define PSC_AUTOTUNE_SEARCH_DEBUG PSC_SELECTIVE_DEBUG_LEVEL( AUTOTUNE_SEARCH )

enum PTF_SEARCH_ERROR {
    PROGRAM_NOT_FOUND,
    INITIALIZATION_ERROR,
    CONNECTION_ERROR,
    INVALID_SEARCH_SPACE
};

class ISearchAlgorithm {
protected:
    int              search_steps;
    map<int, double> path;
    queue<int>       scenarioIds;

public:
    ISearchAlgorithm( void );

    virtual ~ISearchAlgorithm( void );

    virtual void initialize( DriverContext*,
                             ScenarioPoolSet* ) = 0;

    virtual void clear( void ) = 0;

    virtual void addSearchSpace( SearchSpace* ) = 0;

    virtual void addSearchSpaceWithSignature( SearchSpace* ss, ProgramSignature const& ignored ) {
        addSearchSpace( ss );
    }

    virtual void createScenarios( void ) = 0;

    virtual int getOptimum( void ) = 0;

    virtual map<int, double>getSearchPath( void ) = 0;

    virtual bool searchFinished( void ) = 0;

    virtual void terminate( void ) = 0;

    virtual void finalize( void ) = 0;
};

#endif /* ISEARCHALGORITHM_H_ */
