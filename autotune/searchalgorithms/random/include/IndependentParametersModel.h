#ifndef INDEPENDENTPARAMETERSMODEL_H_
#define INDEPENDENTPARAMETERSMODEL_H_

#include "RandomSearch.h"
#include "TuningConfiguration.h"

#include <ctime>

#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_01.hpp>

class IndependentParametersModel : public ProbabilityModel {
private:
    typedef std::map< TuningParameter*, std::vector<double> > TuningParameterToCDF;
    TuningParameterToCDF paramToCDF;

    std::list<Region*>*                       regionList;
    mutable boost::uniform_01<boost::mt19937> random01;

public:
    template <typename Iterator>IndependentParametersModel( Iterator first,
                                                            Iterator afterLast );

    TuningSpecification* sample() const;

    void addParameterWithCDF( TuningParameter*           param,
                              const std::vector<double>& CDF );

    void addParameterWithPDF( TuningParameter*           param,
                              const std::vector<double>& PDF );

    void addParameterWithSampleCount( TuningParameter*        param,
                                      const std::vector<int>& sampleCount );

    static IndependentParametersModel* fromConfigurationSamples( SearchSpace*                            ss,
                                                                 std::vector<TuningConfiguration> const& tcs,
                                                                 int                                     smoothness = 1 );

    static IndependentParametersModel* learnLinearSVM( SearchSpace*            ss,
                                                       ProgramSignature const& signature );
};

template <typename Iterator>IndependentParametersModel::IndependentParametersModel( Iterator first, Iterator afterLast )
    : regionList( new std::list<Region*>( first, afterLast ) ), random01( boost::mt19937() ) {
    boost::mt19937 rng;
    rng.seed( std::time( 0 ) );
    random01 = boost::uniform_01<boost::mt19937>( rng );
}

#endif /* INDEPENDENTPARAMETERSMODEL_H_ */
