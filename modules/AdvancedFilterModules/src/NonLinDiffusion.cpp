//<LICENSE>

#include "stdafx.h"
#include "NonLinDiffusion.h"

#ifdef _OPENMP
#include <omp.h>
#endif
#include <ParameterHandling.h>

#include <strings/miscstring.h>
#include <math/image_statistics.h>
#include <scalespace/NonLinDiffAOS.h>
#include <containers/PlotData.h>

NonLinDiffusionModule::NonLinDiffusionModule() :
KiplProcessModuleBase("NonLinDiffusionModule", true),
    m_bAutoScale(false),
    m_fSlope(1.0f),
    m_fIntercept(0.0f),
    m_fTau(0.125f),
    m_fSigma(1.5f),
    m_fLambda(1.0f),
    m_nIterations(10),
    m_sIterationPath("./issiteration_####.tif"),
    m_bSaveIterations(false)
{

}

NonLinDiffusionModule::~NonLinDiffusionModule() {
}


int NonLinDiffusionModule::Configure(std::map<std::string, std::string> parameters)
{
    m_bAutoScale      = kipl::strings::string2bool(GetStringParameter(parameters,"autoscale"));;
    m_fSlope          = GetFloatParameter(parameters,"slope") ;
    m_fIntercept      = GetFloatParameter(parameters,"intercept")  ;

    m_fTau            = GetFloatParameter(parameters,"tau");
    m_nIterations     = GetIntParameter(parameters,"iterations");
    m_bSaveIterations = kipl::strings::string2bool(GetStringParameter(parameters,"saveiterations"));
    m_sIterationPath  = parameters["iterationpath"];

    return 0;
}

std::map<std::string, std::string> NonLinDiffusionModule::GetParameters()
{
    std::map<std::string, std::string> parameters;

    parameters["autoscale"]   = kipl::strings::value2string(m_bAutoScale);
    parameters["slope"]       = kipl::strings::value2string(m_fSlope);
    parameters["intercept"]   = kipl::strings::value2string(m_fIntercept);

    parameters["tau"]         = kipl::strings::value2string(m_fTau);
    parameters["iterations"]  = kipl::strings::value2string(m_nIterations);

    parameters["saveiterations"] = kipl::strings::bool2string(m_bSaveIterations);
    parameters["iterationpath"]  = m_sIterationPath;

    return parameters;
}

int NonLinDiffusionModule::ProcessCore(kipl::base::TImage<float,3> & img, std::map<std::string, std::string> & coeff)
{
    logger(kipl::logging::Logger::LogMessage,"Processing");

    akipl::scalespace::NonLinDiffusionFilter<float,3> nld(m_fSigma, m_fTau, m_fLambda, m_nIterations=10);;

    nld(img);
//    ScaleImage(img,true);

//    akipl::scalespace::ISSfilterQ3D<float> filter;

//    filter.eInitialImage = m_eInitialImage;
//    filter.m_eRegularization = m_eRegularization;

//    filter.Process(img,m_fTau,m_fLambda,m_fAlpha,m_nIterations,m_bSaveIterations,m_sIterationPath);

//    ScaleImage(img,false);
//    kipl::containers::PlotData<float,float> errplot(m_nIterations);
//    for (int i=0; i<m_nIterations; i++) {
//        errplot.GetX()[i]=i;
//        errplot.GetY()[i]=filter.GetErrorArray()[i];
//    }
//    m_PlotList["ErrorPlot"]=errplot;

    return 0;
}

void NonLinDiffusionModule::ScaleImage(kipl::base::TImage<float,3> & img, bool forward)
{
    float slope=1.0f, intercept=0.0f;

    std::ostringstream msg;

    if (forward) {
        if (m_bAutoScale) {
            std::pair<double,double> stats=kipl::math::statistics(img.GetDataPtr(),img.Size());

            m_fIntercept=static_cast<float>(stats.first);
            m_fSlope=1.0f/static_cast<float>(stats.second);
        }

        msg<<"Scaling image with slope="<<m_fSlope<<" and intercept="<<m_fIntercept;
        logger(kipl::logging::Logger::LogMessage,msg.str());
        #pragma omp parallel
        {
            float *pImg = img.GetDataPtr();
            ptrdiff_t N=static_cast<ptrdiff_t>(img.Size());

            #pragma omp for
            for (ptrdiff_t i=0; i<N; i++) {
                pImg[i]=m_fSlope*(pImg[i]-m_fIntercept);
            }
        }

    }
    else {
        slope=1.0f/m_fSlope;
        intercept=m_fIntercept;

        #pragma omp parallel
        {
            float *pImg = img.GetDataPtr();
            ptrdiff_t N=static_cast<ptrdiff_t>(img.Size());

            #pragma omp for
            for (ptrdiff_t i=0; i<N; i++) {
                pImg[i]=slope*pImg[i]+intercept;
            }
        }

    }
}
