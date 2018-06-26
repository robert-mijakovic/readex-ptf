#ifndef PROPERTY_REQUEST_H_
#define PROPERTY_REQUEST_H_

#include "Ranks.h"
#include "Region.h"
#include "PropertyID.h"
#include "PropertyConfiguration.h"

#include <stdio.h>
#include <list>
#include <sstream>
#include <boost/serialization/list.hpp>
using namespace std;

class PropertyRequest {
    friend class boost::serialization::access;
    template<class Archive>
    void serialize( Archive& ar, const unsigned int version ) {
        ar& property_ids;
        ar& ranks;
        ar& regions;
    }
private:
    list<int>*    property_ids;
    Ranks         ranks;
    list<string>* regions;

public:
    // TODO make default const. private in all serialized classes
    PropertyRequest();     // should only be used for serialization

    PropertyRequest( list<int>* );

    PropertyRequest( list<int>*,
                     list<unsigned int>* );

    PropertyRequest( list<int>*,
                     list<Range>* );

    virtual ~PropertyRequest();

    list<int>* getPropertyIDs();

    list<string>* getRegions();

    int getTypeOfRanks();

    const Ranks getRanks();

    void addPropertyID( int );

    void addRegion( Region* );

    void addRegion( const std::string& id );

    void addSingleProcess( int );

    void addAllProcesses();

    void addProcessRange( int from,
                          int to );

    void setSingleRank( int rank );

    string toString( int    ident,
                     string indentation_character );

    void print();
};

bool mpiProfilingRequired( list<PropertyRequest*>* requests );

#endif
