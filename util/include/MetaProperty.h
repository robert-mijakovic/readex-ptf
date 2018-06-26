/**
   @file	MetaProperty.h
   @ingroup AnalysisAgent
   @brief   MetaProperty header
   @author	Ventsislav Petkov
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

#ifndef METAPROPERTY_H_
#define METAPROPERTY_H_

#include <string>
#include <map>
#include <vector>
#include "Region.h"
#include "PropertyPurpose.h"

typedef std::map<std::string, std::string> addInfoType;

struct ExecObjType {
    int process;
    int thread;
};

/**
 * A meta-property can be de-serialized from ANY type of property, which is useful for aggregating
 * values when we don't care about the exact type of property.
 */
class MetaProperty {
private:
    bool                     cluster;
    std::string              id;
    std::string              name;
    int                      fileId;
    std::string              fileName;
    int                      startLine;
    int                      maxProcs, maxThreads;
    RegionType               regionType;
    std::string              regionId;
    double                   confidence, severity;
    int                      purpose;
    addInfoType              addInfo;
    std::vector<ExecObjType> execObjs;

public:
    MetaProperty();
    ~MetaProperty();

    static MetaProperty fromXML( std::string propData );     ///< Create a property from an XML string

    static MetaProperty EmptyProp();                         ///< Create an empty property

    std::string toXML();

    std::string toString();

    bool done;          ///< Done processing this property - required by the clustering alg in HLA

    bool condition() const;

    void setCluster( bool cluster );

    bool getCluster();

    void setId( std::string id );

    std::string getId();


    void setName( std::string name );

    std::string getName();

    void setFileId( int fileId );

    int getFileId();

    int getRFL();

    void setFileName( std::string fileName );

    std::string getFileName();

    void setStartPosition( int startLine );

    int getStartPosition();

    void setConfiguration( std::string config );

    std::string getConfiguration();

    std::string getRegionName();

    void setRegionType( RegionType type );

    RegionType getRegionType() const;

    void setRegionId( std::string regionId );

    std::string getRegionId();

    void setSeverity( double severity );

    double getSeverity();

    void setConfidence( double confidence );

    double getConfidence();

    void setProcess( int proc );

    int getProcess();

    int getMaxProcs();

    void setThread( int thread );

    int getThread();

    int getMaxThreads();

    void addExecObj( int process,
                     int thread );

    void addExecObjs( std::vector<ExecObjType> objs );

    std::vector<ExecObjType>getExecObjs();

    void setPurpose( int purps );

    int getPurpose();

    void addExtraInfo( std::string param,
                       std::string value );

    addInfoType getExtraInfo();
};
#endif /* METAPROPERTY_H_ */
