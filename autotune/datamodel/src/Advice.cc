#include "Advice.h"
#include "psc_errmsg.h"
#include <boost/version.hpp>




Advice::Advice() {
}


Advice::Advice( const string& plugin_name,
                Scenario* best_scenario,
                const map<int, double>&                  search_path,
                const string& unit, map<int, Scenario*>* scenarios ) {
    if( !best_scenario ) {
        psc_errmsg( "Error: Attempting to construct advice with NULL best scenario!" );
        abort();
    }

    list<ptree> dataList;

    internalData.put( "PluginName", plugin_name );

    ptree best_scenarios;
    best_scenarios.push_back( make_pair( "ScenarioResult", generate_scenario_result_ptree_enry( best_scenario, search_path, unit ) ) );
    internalData.add_child( "BestScenarios", best_scenarios );

    map<int, Scenario*>::iterator scenario_iter;
    ptree                         search_path_data;
    for( scenario_iter = scenarios->begin(); scenario_iter != scenarios->end(); scenario_iter++ ) {
        if( scenario_iter->second != NULL ) {
            search_path_data.push_back( make_pair( "ScenarioResult", generate_scenario_result_ptree_enry( scenario_iter->second, search_path, unit ) ) );
        }
    }
    internalData.add_child( "SearchPath", search_path_data );

    dataList.push_back( internalData );
    mergeAdvices( dataList, plugin_name );
}


Advice::Advice( const string& plugin_name,
                const list<Scenario*>& best,
                const map<int, double>&                  search_path,
                const string& unit, map<int, Scenario*>* scenarios ) {
    list<ptree> dataList;

    internalData.put( "PluginName", plugin_name );

    ptree                           best_scenarios;
    list<Scenario*>::const_iterator sc_iter;
    for( sc_iter = best.begin(); sc_iter != best.end(); sc_iter++ ) {
        best_scenarios.push_back( make_pair( "ScenarioResult", generate_scenario_result_ptree_enry( ( *sc_iter ), search_path, unit ) ) );
    }

    internalData.add_child( "BestScenarios", best_scenarios );

    map<int, Scenario*>::iterator scenario_iter;
    ptree                         search_path_data;
    for( scenario_iter = scenarios->begin(); scenario_iter != scenarios->end(); scenario_iter++ ) {
        if( scenario_iter->second != NULL ) {
            search_path_data.push_back( make_pair( "ScenarioResult",
                                                   generate_scenario_result_ptree_enry( scenario_iter->second, search_path, unit ) ) );
        }
    }
    internalData.add_child( "SearchPath", search_path_data );

    dataList.push_back( internalData );
    mergeAdvices( dataList, plugin_name );
}


ptree Advice::generate_scenario_result_ptree_enry( Scenario* scenario,
                                                   const map<int, double>& search_path,
                                                   string unit ) {
    ptree scenario_data;
    ptree scenario_region;
    if( scenario->getRegion() != NULL ) {
        scenario_region.put( "FileName", scenario->getRegion()->getFileName() );
        scenario_region.put( "FirstLine", scenario->getRegion()->getFirstLine() );
        scenario_region.put( "LastLine", scenario->getRegion()->getLastLine() );
    }
    ptree                                tuning_specification;
    ptree                                variant;
    ptree                                variant_context;
    ptree                                ranks;
    ptree                                tuning_parameters;
    map<TuningParameter*, int>::iterator iter;
    list<TuningSpecification*>::iterator ts_iter;
    if( scenario != NULL ) {
        if( scenario->getTuningSpecifications() != NULL ) {
            for( ts_iter = scenario->getTuningSpecifications()->begin(); ts_iter != scenario->getTuningSpecifications()->end(); ts_iter++ ) {
                // the variant is a list of parameters and values
                if( *ts_iter != NULL ) {
                    if( ( *ts_iter )->getVariant() != NULL ) {
                        map<TuningParameter*, int> val = ( *ts_iter )->getVariant()->getValue();
                        for( iter = val.begin(); iter != val.end(); iter++ ) {
                            tuning_parameters.put( "Name", iter->first->getName() );
                            tuning_parameters.put( "Value", iter->second );
                            variant.push_back( make_pair( "TuningParameter", tuning_parameters ) );
                        }
                    }

                    // the VariantContext is either the program, a list of regions or a list of files
                    // branch here for the type, then a for loop for the 2 list types
                    int type = ( *ts_iter )->getTypeOfVariantContext();
                    if( type == variant_context_type( PROGRAM ) ) {
                        variant_context.put( "Type", "PROGRAM" );
                    }
                    else if( type == variant_context_type( REGION_LIST ) ) {
                        variant_context.put( "Type", "REGION_LIST" );
                        ptree                  region;
                        list<string>::iterator region_iter;
                        for( region_iter = ( *ts_iter )->getVariantContext().context_union.region_list->begin();
                             region_iter != ( *ts_iter )->getVariantContext().context_union.region_list->end(); region_iter++ ) {
                            Region* const r = Application::instance().getRegionByID( *region_iter, true );
                            if( !r ) {
                                continue;
                            }

                            region.put( "FileName", r->getFileName() );
                            region.put( "FirstLine", r->getFirstLine() );
                            region.put( "LastLine", r->getLastLine() );
                            variant_context.push_back( make_pair( "Region", region ) );
                        }
                    }
                    else if( type == variant_context_type( FILE_LIST ) ) {
                        variant_context.put( "Type", "FILE_LIST" );
                        list<string>::iterator string_iter;
                        for( string_iter = ( *ts_iter )->getVariantContext().context_union.file_list->begin();
                             string_iter != ( *ts_iter )->getVariantContext().context_union.file_list->end(); string_iter++ ) {
                            variant_context.put( "File", ( *string_iter ) );
                        }
                    }
                    else {
                        perror( "invalid type of VariantContext found when iterating on the TuningSpecification\n" );
                        throw 0;
                    }

                    // the Rank is either all, a list of ranks or a list of ranges
                    // again, branch here for the type, then a for loop for the 2 list types
                    type = ( *ts_iter )->getTypeOfRanks();
                    if( type == ranks_type( ALL ) ) {
                        ranks.put( "Type", "ALL" );
                    }
                    else if( type == ranks_type( RANK_LIST ) ) {
                        ranks.put( "Type", "RANK_LIST" );
                        list<unsigned int>::iterator rank_iter;
                        for( rank_iter = ( *ts_iter )->getRanks().ranks_union.rank_list->begin(); rank_iter != ( *ts_iter )->getRanks().ranks_union.rank_list->end();
                             rank_iter++ ) {
                            ranks.put( "Rank", ( *rank_iter ) );
                        }
                    }
                    else if( type == ranks_type( RANGE_LIST ) ) {
                        ranks.put( "Type", "RANGE_LIST" );
                        ptree                 range;
                        list<Range>::iterator rank_iter;
                        for( rank_iter = ( *ts_iter )->getRanks().ranks_union.range_list->begin(); rank_iter != ( *ts_iter )->getRanks().ranks_union.range_list->end();
                             rank_iter++ ) {
                            range.put( "Start", ( *rank_iter ).start );
                            range.put( "End", ( *rank_iter ).end );
                            ranks.push_back( make_pair( "Range", range ) );
                        }
                    }
                    else {
                        psc_abort( "invalid type of VariantContext found when iterating on the TuningSpecification\n" );
                    }
                }

                tuning_specification.push_back( make_pair( "Variant", variant ) );
                tuning_specification.push_back( make_pair( "VariantContext", variant_context ) );
                tuning_specification.push_back( make_pair( "Ranks", ranks ) );

                scenario_data.put( "ID", scenario->getID() );
                scenario_data.put( "Description", scenario->getDescription() );
                scenario_data.push_back( make_pair( "Region", scenario_region ) );
                scenario_data.push_back( make_pair( "TuningSpecification", tuning_specification ) );
            }
        }
    }
    ptree scenario_result;
    scenario_result.push_back( make_pair( "Scenario", scenario_data ) );
    map<string, double> results;
    if( scenario != NULL ) {
        results = scenario->getResults();
    }
    map<string, double>::iterator res_iter;

    for( res_iter = results.begin(); res_iter != results.end(); res_iter++ ) {
        ptree result;
        result.put( "Description", res_iter->first );
        result.put( "Value", res_iter->second );
        scenario_result.push_back( make_pair( "Result", result ) );
    }

    return scenario_result;
}

const ptree& Advice::getInternalAdviceData() const {
    return internalData;
}

void Advice::mergeAdvices( list<ptree>   dataList,
                           const string& plugin_name ) {
    list<ptree>::iterator data_it;

    for( data_it = dataList.begin(); data_it != dataList.end(); data_it++ ) {
        XMLOutputData.add_child( "Advices.Advice", ( *data_it ) );
    }
}

void Advice::toXML( const string& filename ) {
#if BOOST_VERSION >= 105600
    write_xml( filename, XMLOutputData, locale(), xml_writer_settings<ptree::key_type>( ' ', 4 ) );
#else
    write_xml( filename, XMLOutputData, locale(), xml_writer_settings<char>( ' ', 4 ) );
#endif
}
