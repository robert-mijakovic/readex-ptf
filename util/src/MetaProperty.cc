#include "MetaProperty.h"
#include "psc_errmsg.h"
#include "stringutil.h"
#include "xml_psc_tags.h"
#include <boost/foreach.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/property_tree/ptree.hpp>
#include <iomanip>
#include <string>
#include <sstream>




MetaProperty::MetaProperty() :
    cluster( false ),
    id( "" ),
    name( "" ),
    fileId( 0 ),
    fileName( "" ),
    startLine( 0 ),
    maxProcs( 1 ),
    maxThreads( 1 ),
    regionType( UNKNOWN ),
    regionId( "" ),
    confidence( 0.0 ),
    severity( 0.0 ),
    done( false ),
    purpose( 0 ) {
}

MetaProperty::~MetaProperty() {
}

bool MetaProperty::condition() const {
    return true;
}

void MetaProperty::setCluster( bool cluster ) {
    this->cluster = cluster;
}

bool MetaProperty::getCluster() {
    return this->cluster;
}

void MetaProperty::setId( std::string id ) {
//    psc_dbgmsg( 10, "Set property ID to %s\n", id.c_str() );
    this->id = id;
}
std::string MetaProperty::getId() {
    return this->id;
}

void MetaProperty::setName( std::string name ) {
    this->name = name;
}

std::string MetaProperty::getName() {
    return this->name;
}

void MetaProperty::setFileId( int fileId ) {
    this->fileId = fileId;
}

int MetaProperty::getFileId() {
    return this->fileId;
}

int MetaProperty::getRFL() {
    return getStartPosition();
}

void MetaProperty::setFileName( std::string fileName ) {
    this->fileName = fileName;
}

std::string MetaProperty::getFileName() {
    return this->fileName;
}

void MetaProperty::setStartPosition( int startLine ) {
    this->startLine = startLine;
}
int MetaProperty::getStartPosition() {
    return this->startLine;
}

void MetaProperty::setConfiguration( std::string config ) {
    char* conf = ( char* )config.data();
    this->maxProcs = atoi( strtok( conf, "x" ) );

    char* threads = strtok( NULL, "x" );
    if( threads != 0 ) {
        this->maxThreads = atoi( threads );
    }
}

std::string MetaProperty::getConfiguration() {
    std::stringstream tmp;
    tmp << this->maxProcs << "x" << this->maxThreads;
    return tmp.str();
}

std::string MetaProperty::getRegionName() {
    std::vector<std::string> tokens;
    int                      pos   = 0;
    std::string              delim = "*";

    while( pos != std::string::npos ) {
        std::string token_str;
        pos = get_token( regionId, pos, delim, token_str );
        tokens.push_back( token_str );
    }

    if( tokens.size() < 3 ) {
        psc_errmsg( "Region_id parsing failed: %s , number of tokens: %d \n",
                    regionId.c_str(), tokens.size() );
        return NULL;
    }

    std::vector<std::string>::iterator it = tokens.begin();
    ++it;
    return *it;
}

void MetaProperty::setRegionType( RegionType type ) {
    this->regionType = type;
}

RegionType MetaProperty::getRegionType() const {
    return this->regionType;
}

void MetaProperty::setRegionId( std::string regionId ) {
    if( regionId.length() == 0 ) {
        std::stringstream tmp;
        tmp << fileId << "-" << startLine;
        regionId = tmp.str();
    }
//    psc_dbgmsg( 10, "Set region ID to %s!\n", regionId.c_str() );
    this->regionId = regionId;
}

std::string MetaProperty::getRegionId() {
    return this->regionId;
}

void MetaProperty::setSeverity( double severity ) {
    this->severity = severity;
}

double MetaProperty::getSeverity() {
    return this->severity;
}

void MetaProperty::setConfidence( double confidence ) {
    this->confidence = confidence;
}

double MetaProperty::getConfidence() {
    return this->confidence;
}

void MetaProperty::setProcess( int proc ) {
    ExecObjType execObj;

    if( execObjs.size() != 0 ) {
        execObj = execObjs.at( 0 );
        execObjs.clear();
    }
    execObj.process = proc;
    execObjs.push_back( execObj );
}

int MetaProperty::getProcess() {
    return execObjs.at( 0 ).process;
}

int MetaProperty::getMaxProcs() {
    return maxProcs;
}

void MetaProperty::setThread( int thread ) {
    ExecObjType execObj;

    if( execObjs.size() != 0 ) {
        execObj = execObjs.at( 0 );
        execObjs.clear();
    }
    execObj.thread = thread;
    execObjs.push_back( execObj );
}

int MetaProperty::getThread() {
    return execObjs.at( 0 ).thread;
}

int MetaProperty::getMaxThreads() {
    return maxThreads;
}

void MetaProperty::addExecObj( int process, int thread ) {
    ExecObjType execObj;
    execObj.process = process;
    execObj.thread  = thread;
    execObjs.push_back( execObj );
}

void MetaProperty::addExecObjs( std::vector<ExecObjType> objs ) {
    execObjs.insert( execObjs.end(), objs.begin(), objs.end() );
}

std::vector<ExecObjType> MetaProperty::getExecObjs() {
    return execObjs;
}

void MetaProperty::setPurpose( int purps ) {
    purpose = purps;
}

int MetaProperty::getPurpose( void ) {
    return purpose;
}

void MetaProperty::addExtraInfo( std::string param, std::string value ) {
//    psc_dbgmsg( 10, "Added extra info: %s - %s\n", param.c_str(), value.c_str() );
    addInfo.insert( std::pair<std::string, std::string>( param, value ) );
}

addInfoType MetaProperty::getExtraInfo( void ) {
    return addInfo;
}

MetaProperty MetaProperty::EmptyProp( void ) {
    return MetaProperty();
}

MetaProperty MetaProperty::fromXML( std::string propData ) {
    MetaProperty result;

    try {
        using boost::property_tree::ptree;

        // parse the XML
        const std::string  ATTRIBUTE = "<xmlattr>.";
        ptree              tree;
        std::istringstream stream( propData );
        read_xml( stream, tree );

        // read the property attributes
        const ptree& property = tree.get_child( XML_PSC_PROP_TAG );
        result.setId( property.get<std::string>( ATTRIBUTE + XML_PSC_PROP_ID_TAG ) );
        std::string cluster = property.get<std::string>( ATTRIBUTE + XML_PSC_PROP_CLUSTER_TAG );
        result.setCluster( cluster == "true" );
        result.setName( property.get<std::string>( XML_PSC_PROP_NAME_TAG ) );
        result.setSeverity( property.get<double>( XML_PSC_PROP_SEVERITY_TAG ) );
        result.setConfidence( property.get<double>( XML_PSC_PROP_CONFIDENCE_TAG ) );
        result.setPurpose( property.get<int>( XML_PSC_PROP_PURPOSE_TAG ) );

        // read the context information
        const ptree& context = property.get_child( XML_PSC_PROP_CONTEXT_TAG );
        result.setFileId( context.get<int>( ATTRIBUTE + XML_PSC_PROP_CONTEXT_FILEID_TAG ) );
        result.setFileName( context.get<std::string>( ATTRIBUTE + XML_PSC_PROP_CONTEXT_FILENAME_TAG ) );
        result.setStartPosition( context.get<int>( ATTRIBUTE + XML_PSC_PROP_CONTEXT_RFL_TAG ) );
        result.setRegionType( static_cast<RegionType>( context.get<int>( ATTRIBUTE + XML_PSC_PROP_CONTEXT_REGION_TYPE_TAG ) ) );
        result.setConfiguration( context.get<std::string>( ATTRIBUTE + XML_PSC_PROP_CONTEXT_CONFIG_TAG ) );
        boost::optional<std::string> regionId = context.get_optional<std::string>( ATTRIBUTE + XML_PSC_PROP_CONTEXT_REGIONID_TAG );
        result.setRegionId( regionId  ?  *regionId : "" );

        // read all execution objects from the context
        BOOST_FOREACH( const ptree::value_type & execObj, context ) {
            if( execObj.first != XML_PSC_PROP_EXECOBJ_TAG ) {
                continue;
            }

            int process = execObj.second.get<int>( ATTRIBUTE + XML_PSC_PROP_EXECOBJ_PROC_TAG );
            int thread  = execObj.second.get<int>( ATTRIBUTE + XML_PSC_PROP_EXECOBJ_THREAD_TAG );
            result.addExecObj( process, thread );
        }

        // read all additional information
        boost::optional<const ptree&> addInfo = property.get_child_optional( XML_PSC_PROP_ADDINFO_TAG );
        if( addInfo ) {
            BOOST_FOREACH( const ptree::value_type & info, *addInfo ) {
                std::string value = info.second.get_value<std::string>();
                result.addExtraInfo( info.first, value );
            }
        }
    }
    catch( std::exception& ex ) {
        psc_errmsg( "Unable to parse the property XML: %s\n", ex.what() );
        abort();
    }

    return result;
}

std::string MetaProperty::toXML() {
    std::stringstream xmlData;

    // Property: <property cluster="false" ID="1" >
    xmlData << "  <" << XML_PSC_PROP_TAG << " " << XML_PSC_PROP_CLUSTER_TAG << "=\"" << std::boolalpha << this->getCluster() << "\" ";
    xmlData << XML_PSC_PROP_ID_TAG << "=\"" << this->getId() << "\" >" << std::endl;

    // Property name: <name>IA64 Pipeline Stall Cycles</name>
    xmlData << "\t<" << XML_PSC_PROP_NAME_TAG << ">" << this->getName() << "</" << XML_PSC_PROP_NAME_TAG << ">" << std::endl;

    // Context: <context FileID="42" FileName="add.f90" RFL="2" Region="USER_REGION">
    xmlData << "\t<" << XML_PSC_PROP_CONTEXT_TAG << " "
    XML_PSC_PROP_CONTEXT_FILEID_TAG << "=\"" << this->getFileId() << "\" " <<
    XML_PSC_PROP_CONTEXT_FILENAME_TAG << "=\"" << this->getFileName() << "\" " <<
    XML_PSC_PROP_CONTEXT_RFL_TAG << "=\"" << this->getStartPosition() << "\" " <<
    XML_PSC_PROP_CONTEXT_CONFIG_TAG    << "=\"" << this->getConfiguration() << "\" " <<
    XML_PSC_PROP_CONTEXT_REGION_TYPE_TAG << "=\"" << static_cast<int>( this->getRegionType() ) << "\" " <<
    XML_PSC_PROP_CONTEXT_REGIONID_TAG << "=\"" << this->getRegionId() << "\" >" << std::endl;


    // Executing object: <execObj process="0" thread="0" />
/*
    // Debug output - add the total number of entries
    xmlData << "\t\t<" << XML_PSC_PROP_EXECOBJ_TAG << " " <<
        XML_PSC_PROP_EXECOBJ_PROC_TAG   << "=\"" << execObjs.size() << "\" " <<
        XML_PSC_PROP_EXECOBJ_THREAD_TAG << "=\"" << "-1" << "\"/>" << std::endl;
 */
    std::vector<ExecObjType>::const_iterator cii;
    for( cii = execObjs.begin(); cii != execObjs.end(); cii++ ) {
        xmlData << "\t\t<" << XML_PSC_PROP_EXECOBJ_TAG << " " <<
        XML_PSC_PROP_EXECOBJ_PROC_TAG   << "=\"" << ( *cii ).process << "\" " <<
        XML_PSC_PROP_EXECOBJ_THREAD_TAG << "=\"" << ( *cii ).thread << "\"/>" << std::endl;
    }

    xmlData << "\t</" << XML_PSC_PROP_CONTEXT_TAG << ">" << std::endl;

    //xmlData.precision(2);	// Set 2 digits after the decimal point
    //xmlData << std::fixed;	// Use a fixed point floating point format or std::scientific

    // Severity: <severity>52.30</severity>
    xmlData << "\t<" << XML_PSC_PROP_SEVERITY_TAG << ">" << this->getSeverity() << "</" << XML_PSC_PROP_SEVERITY_TAG << ">" << std::endl;

    // Confidence: <confidence>0.99</confidence>
    xmlData << "\t<" << XML_PSC_PROP_CONFIDENCE_TAG << ">" << this->getConfidence() << "</" << XML_PSC_PROP_CONFIDENCE_TAG << ">" << std::endl;

    // Information about the purpose of the property, can be analysis or tuning.
    // Purpose: <purpose>0</purpose>
    xmlData << "\t<" << XML_PSC_PROP_PURPOSE_TAG << ">" << this->getPurpose()
            << "</" << XML_PSC_PROP_PURPOSE_TAG << ">" << std::endl;

    // Additional information
    xmlData << "\t<" << XML_PSC_PROP_ADDINFO_TAG << ">" << std::endl;
    for( addInfoType::const_iterator it = addInfo.begin(); it != addInfo.end(); ++it ) {
        xmlData << "\t\t<" << it->first << ">";
        xmlData << it->second;
        xmlData << "</" << it->first << ">" << std::endl;
    }

    xmlData << "\t</" << XML_PSC_PROP_ADDINFO_TAG << ">" << std::endl;

    // End of the Property
    xmlData << "  </" << XML_PSC_PROP_TAG << ">" << std::endl;

    //std::cout << "Serialized a property: " << xmlData.str() << std::endl;
    return xmlData.str();
}

std::string MetaProperty::toString() {
    std::stringstream strData;

    if( cluster && execObjs.size() <= 20 ) {
        std::vector<ExecObjType>::const_iterator cii;
        std::stringstream                        procsStream, threadsStream;

        cii = execObjs.begin();
        while( cii != execObjs.end() ) {
            procsStream << ( *cii ).process;
            threadsStream << ( *cii ).thread;
            cii++;
            if( cii != execObjs.end() ) {
                procsStream << ",";
                threadsStream << ",";
            }
        }
//        strData << "P(" << procsStream.str() << ");\t(" << threadsStream.str() << ");" << std::endl;
//        strData << std::setw( 5 )<< " " << "\t"<< std::setw( 5 )<< " " << "\t" ;
        strData << "P(" << procsStream.str() << ");" << std::endl;
        strData << std::setw( 5 ) << " " << "\t";
    }
    else if( execObjs.size() > 20 ) {
        strData << "P(...); \t";
    }
    else {
//        strData  << "P" << std::setw( 4 ) << getProcess() << "; \t"<< std::setw( 4 ) << getThread()<< "; \t" ;
        strData << "P" << std::setw( 4 ) << getProcess() << "; \t";
    }

    strData << std::setw( 12 ) <<  getRegionName() << "; \t";
    strData << std::setw( 12 ) <<  getRegionType() << "; \t";
    strData << std::setw( 10 ) << getFileName() << ":" << getStartPosition() << ";\t";
    strData << std::setprecision( 3 ) << std::setw( 8 ) << std::fixed << getSeverity() << ";\t";
    strData << getName();

    return strData.str();
}
