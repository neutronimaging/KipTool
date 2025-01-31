//<LICENSE>
#include "morphspotcleanmodule.h"
#include <thread>
#include <cstdlib>
#include "ImagingModules_global.h"
#include "morphspotcleanmodule.h"
#include <ParameterHandling.h>
#include <MorphSpotClean.h>
#include <base/timage.h>
#include <base/textractor.h>
#include <strings/miscstring.h>
#include <strings/string2array.h>
#include <ImagingException.h>
#include <KiplFrameworkException.h>
#include <ModuleException.h>


IMAGINGMODULESSHARED_EXPORT MorphSpotCleanModule::MorphSpotCleanModule(kipl::interactors::InteractionBase *interactor) :
    KiplProcessModuleBase("MorphSpotClean",false, interactor),
    m_eConnectivity(kipl::base::conn4),
    m_eDetectionMethod(ImagingAlgorithms::MorphDetectPeaks),
    m_eCleanMethod(ImagingAlgorithms::MorphCleanReplace),
    m_fThreshold{0.1f,0.1f},
    m_fSigma{0.01f,0.01f},
    m_nEdgeSmoothLength(5),
    m_nMaxArea(30),
    m_bRemoveInfNan(false),
    m_bUseClamping(false),
    m_fMinLevel(-0.1),
    m_fMaxLevel(12),
    m_bThreading(false)
{

}

IMAGINGMODULESSHARED_EXPORT MorphSpotCleanModule::~MorphSpotCleanModule()
{

}

int IMAGINGMODULESSHARED_EXPORT MorphSpotCleanModule::Configure(KiplProcessConfig config, std::map<string, string> parameters)
{
    std::ignore=config;

    string2enum(GetStringParameter(parameters,"connectivity"),m_eConnectivity);
    string2enum(GetStringParameter(parameters,"cleanmethod"),m_eCleanMethod);
    string2enum(GetStringParameter(parameters,"detectionmethod"),m_eDetectionMethod);
    kipl::strings::string2vector(GetStringParameter(parameters,"threshold"),m_fThreshold);
    kipl::strings::string2vector(GetStringParameter(parameters,"sigma"),m_fSigma);
    m_nEdgeSmoothLength = GetIntParameter(parameters,"edgesmooth");
    m_nMaxArea          = GetIntParameter(parameters,"maxarea");
    m_bRemoveInfNan     = kipl::strings::string2bool(GetStringParameter(parameters,"removeinfnan"));
    m_bUseClamping      = kipl::strings::string2bool(GetStringParameter(parameters,"useclamping"));
    m_fMinLevel         = static_cast<double>(GetFloatParameter(parameters,"minlevel"));
    m_fMaxLevel         = static_cast<double>(GetFloatParameter(parameters,"maxlevel"));
    m_bThreading        = kipl::strings::string2bool(GetStringParameter(parameters,"threading"));

    return 0;
}

std::map<string, string> IMAGINGMODULESSHARED_EXPORT MorphSpotCleanModule::GetParameters()
{
    std::map<std::string, std::string> parameters;

    parameters["connectivity"] = enum2string(m_eConnectivity);
    parameters["cleanmethod"]  = enum2string(m_eCleanMethod);
    parameters["detectionmethod"] = enum2string(m_eDetectionMethod);
    parameters["threshold"]    = kipl::strings::Vector2String(m_fThreshold);
    parameters["sigma"]        = kipl::strings::Vector2String(m_fSigma);
    parameters["edgesmooth"]   = kipl::strings::value2string(m_nEdgeSmoothLength);
    parameters["maxarea"]      = kipl::strings::value2string(m_nMaxArea);
    parameters["removeinfnan"] = kipl::strings::bool2string(m_bRemoveInfNan);
    parameters["useclamping"]  = kipl::strings::bool2string(m_bUseClamping);
    parameters["minlevel"]     = kipl::strings::value2string(m_fMinLevel);
    parameters["maxlevel"]     = kipl::strings::value2string(m_fMaxLevel);
    parameters["threading"]    = kipl::strings::bool2string(m_bThreading);

    return parameters;
}

bool IMAGINGMODULESSHARED_EXPORT MorphSpotCleanModule::updateStatus(float val, std::string msg)
{
    if (m_Interactor!=nullptr) {
        return m_Interactor->SetProgress(val,msg);
    }
    return false;
}


int IMAGINGMODULESSHARED_EXPORT MorphSpotCleanModule::ProcessCore(kipl::base::TImage<float, 3> &img, std::map<string, string> &coeff)
{
    std::ignore = coeff;
    
    logger(logger.LogMessage,"ProcessCore");

    size_t Nslices=0;
    Nslices=img.Size(2);

//    switch (plane) {
//    case kipl::base::ImagePlaneXY:
//        Nslices=img.Size(2);
//        break;
//    case kipl::base::ImagePlaneXZ:
//        Nslices=img.Size(1);
//        break;
//    case kipl::base::ImagePlaneYZ:
//        Nslices=img.Size(0);
//        break;
//    default:
//        break;
//    }


    std::ostringstream msg;
    ImagingAlgorithms::MorphSpotClean cleaner;
    cleaner.setCleanMethod(m_eDetectionMethod,m_eCleanMethod);
    cleaner.setConnectivity(m_eConnectivity);
    cleaner.setEdgeConditioning(m_nEdgeSmoothLength);
    cleaner.cleanInfNan();
    cleaner.setLimits(m_bUseClamping,m_fMinLevel,m_fMaxLevel,m_nMaxArea);

    kipl::base::TImage<float,2> slice;

    for (size_t i=0; (i<Nslices && (updateStatus(float(i)/Nslices,"Processing MorphSpot cleaning")==false) ); i++)
    {

        slice=kipl::base::ExtractSlice(img,i,kipl::base::ImagePlaneXY,{});

            try {
                cleaner.process(slice,m_fThreshold, m_fSigma);
            }
            catch (ImagingException & e) {
                msg.str("");
                msg<<"Failed to process data with ImagingException : "<<std::endl<<e.what();
                throw ImagingException(msg.str(),__FILE__,__LINE__);
            }
            catch (kipl::base::KiplException & e) {
                msg.str("");
                msg<<"Failed to process data with KiplException : "<<std::endl<<e.what();
                throw ImagingException(msg.str(),__FILE__,__LINE__);
            }
            catch (std::exception & e) {
                msg.str("");
                msg<<"Failed to process data with STL exception : "<<std::endl<<e.what();
                throw ImagingException(msg.str(),__FILE__,__LINE__);
            }
            catch (...) {


            }
         kipl::base::InsertSlice(slice,img,i,kipl::base::ImagePlaneXY);
    }


    return 0;
}

pair<kipl::base::TImage<float,2>,kipl::base::TImage<float,2>> IMAGINGMODULESSHARED_EXPORT MorphSpotCleanModule::DetectionImage(kipl::base::TImage<float,2> img,
                                                                                             ImagingAlgorithms::eMorphDetectionMethod dm, bool removeBias)
{
    ImagingAlgorithms::MorphSpotClean cleaner;
    cleaner.setCleanMethod(dm,m_eCleanMethod);
    return cleaner.detectionImage(img,removeBias);
}

