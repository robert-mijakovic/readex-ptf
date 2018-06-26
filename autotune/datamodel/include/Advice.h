/**
   @file    Advice.h
   @ingroup Autotune
   @brief   Advice returned by plugins
   @author  Isaias Compres
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

#ifndef PSC_ADVICE_H_
#define PSC_ADVICE_H_

#include <list>
#include <map>
#include <vector>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/detail/xml_parser_writer_settings.hpp>
#include "Scenario.h"

using boost::property_tree::ptree;
using boost::property_tree::xml_writer_settings;




class Advice {
public:
    Advice();

    Advice( const std::string& plugin_name,
            Scenario* best_scenario,
            const std::map<int, double>& search_path,
            const std::string&           unit,
            std::map<int, Scenario*>* scenarios );

    Advice( const std::string& plugin_name,
            const std::list<Scenario*>& best_scenarios,
            const std::map<int, double>& search_path,
            const std::string&           unit,
            std::map<int, Scenario*>* scenarios );

    const ptree& getInternalAdviceData() const;

    void mergeAdvices( std::              list<ptree>,
                       const std::string& plugin_name );

    void toXML( const std::string& );

private:
    ptree internalData;
    ptree XMLOutputData;

    ptree generate_scenario_result_ptree_enry( Scenario* scenario,
                                               const std::map<int, double>& search_path,
                                               std::string unit );
};

#endif /* PSC_ADVICE_H_ */
