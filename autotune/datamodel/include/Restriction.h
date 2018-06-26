/**
   @file	Restriction.h
   @ingroup Autotune
   @brief   Tuning Parameter Restrictions
   @author	Houssam Haitof
   @verbatim
   Revision:       $Revision$
   Revision date:  $Date$
   Committed by:   $Author$

   This file is part of the Periscope performance measurement tool.
   See http://www.lrr.in.tum.de/periscope for details.

   Copyright (c) 2005-2012, Technische Universitaet Muenchen, Germany
   See the COPYING file in the base directory of the package for details.
   @endverbatim
 */

#ifndef RESTRICTION_H_
#define RESTRICTION_H_
#include "application.h"
#include <vector>

using namespace std;


class Restriction {
    friend class boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version ) {
        ar& regionDefined;
        ar& region;
        ar& type;
    }
    bool        regionDefined;
    string      region;
    int         type;
    vector<int> elements;

public:


    Restriction();

    virtual ~Restriction();

    void setType( int i ) {
        type = i;
    }

    int getType() {
        return type;
    }

    bool isRegionDefined() const;

    void setRegionDefined( bool regionDefined );

    Region* getRegion();

    void setRegion( Region* region );

    vector<int>getElements() {
        return elements;
    }

    void addElement( int element ) {
        elements.push_back( element );
    }

    bool operator==( const Restriction& in ) const;

    bool operator!=( const Restriction& in ) const;
};

#endif /* RESTRICTION_H_ */
