//<LICENSE>

#include "ISSfilterModule.h"

#ifdef _OPENMP
#include <omp.h>
#endif
#include <ParameterHandling.h>

#include <strings/miscstring.h>
#include <math/image_statistics.h>
#include <ISSfilterQ3D.h>
#include <containers/PlotData.h>

ISSfilterModule::ISSfilterModule(kipl::interactors::InteractionBase *interactor) :
KiplProcessModuleBase("ISSfilter", true,interactor),
    m_bAutoScale(true),
	m_fSlope(1.0f),
	m_fIntercept(0.0f),
	m_fTau(0.125f),
	m_fLambda(1.0f),
	m_fAlpha(0.25f),
	m_nIterations(10),
	m_sIterationPath("./issiteration_####.tif"),
    m_bSaveIterations(false),
    m_eRegularization(advancedfilters::RegularizationTV2),
    m_eInitialImage(advancedfilters::InitialImageOriginal)
{
    publications.push_back(Publication(std::vector<string>({"M. Burger","G. Gilboa","S. Osher","J. Xu"}),
                                                "Nonlinear inverse scale space methods",
                                                "Communications in Mathematical Sciences",
                                                2006,
                                                4,
                                                1,
                                                "179-212",
                                                "10.4310/cms.2006.v4.n1.a7"));
}

ISSfilterModule::~ISSfilterModule() {
}


int ISSfilterModule::Configure(KiplProcessConfig m_Config, std::map<std::string, std::string> parameters)
{
    m_fSlope          = GetFloatParameter(parameters,"slope");
    m_fIntercept      = GetFloatParameter(parameters,"intercept");
    m_bAutoScale      = kipl::strings::string2bool(GetStringParameter(parameters,"autoscale"));
    m_fTau            = GetFloatParameter(parameters,"tau");
    m_fLambda         = GetFloatParameter(parameters,"lambda");
    m_fAlpha          = GetFloatParameter(parameters,"alpha");
    m_nIterations     = GetIntParameter(parameters,"iterations");
	m_bSaveIterations = kipl::strings::string2bool(GetStringParameter(parameters,"saveiterations"));
    m_sIterationPath  = parameters["iterationpath"];
    string2enum(GetStringParameter(parameters,"regularization"), m_eRegularization);
    string2enum(GetStringParameter(parameters,"initialimage"),   m_eInitialImage);
    m_bThreading      = kipl::strings::string2bool(GetStringParameter(parameters,"threading"));

	return 0;
}

std::map<std::string, std::string> ISSfilterModule::GetParameters()
{
	std::map<std::string, std::string> parameters;

	parameters["slope"]     = kipl::strings::value2string(m_fSlope);
	parameters["intercept"] = kipl::strings::value2string(m_fIntercept);
	parameters["autoscale"] = kipl::strings::bool2string(m_bAutoScale); 

	parameters["tau"]         = kipl::strings::value2string(m_fTau);
	parameters["lambda"]      = kipl::strings::value2string(m_fLambda);
	parameters["alpha"]       = kipl::strings::value2string(m_fAlpha);
	parameters["iterations"]  = kipl::strings::value2string(m_nIterations);

	parameters["saveiterations"] = kipl::strings::bool2string(m_bSaveIterations);
	parameters["iterationpath"]  = m_sIterationPath;
    parameters["regularization"] = enum2string(m_eRegularization);
    parameters["initialimage"]   = enum2string(m_eInitialImage);
    parameters["threading"]      = kipl::strings::bool2string(m_bThreading);

	return parameters;
}

int ISSfilterModule::ProcessCore(kipl::base::TImage<float,3> & img, std::map<std::string, std::string> & coeff)
{
	ScaleImage(img,true);

    advancedfilters::ISSfilterQ3D<float> filter(m_Interactor);

    filter.setInitialImageType(m_eInitialImage);
    filter.setRegularizationType(m_eRegularization);

    filter.process(img,m_fTau,m_fLambda,m_fAlpha,m_nIterations,m_bSaveIterations,m_sIterationPath);

	ScaleImage(img,false);
    kipl::containers::PlotData<float,float> errplot(m_nIterations,"ISS error");
	for (int i=0; i<m_nIterations; i++) {
		errplot.GetX()[i]=i;
        errplot.GetY()[i]=filter.errors()[i];
	}
	m_PlotList["ErrorPlot"]=errplot;

	return 0;
}

void ISSfilterModule::ScaleImage(kipl::base::TImage<float,3> & img, bool forward)
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
