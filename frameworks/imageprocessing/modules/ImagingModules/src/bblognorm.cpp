//<LICENSE>

#include <filesystem>
namespace fs = std::filesystem;
#include <algorithm>

#include "ImagingModules_global.h"
#include <strings/miscstring.h>

#include <math/image_statistics.h>
#include <math/median.h>
#include <base/textractor.h>
#include <strings/miscstring.h>
#include <strings/string2array.h>
#include <strings/filenames.h>

#include <ParameterHandling.h>
#include <ModuleException.h>

#include <imagereader.h>
#include <io/io_tiff.h>

#include "bblognorm.h"
#include "../include/ImagingException.h"
// #include <qdebug.h>
// #include <QFile>

IMAGINGMODULESSHARED_EXPORT BBLogNorm::BBLogNorm(kipl::interactors::InteractionBase *interactor) : KiplProcessModuleBase("BBLogNorm", false, interactor),
    // to check which one do i need: to be removed: m_nWindow and bUseWeightedMean
    m_Config(""),
    nBBextCount(1),
    nBBextFirstIndex(0),
    nOBCount(0),
    nOBFirstIndex(1),
    nDCCount(0),
    nDCFirstIndex(1),
    nBBCount(0),
    nBBFirstIndex(1),
    nBBSampleCount(0),
    nBBSampleFirstIndex(1),
    fFlatDose(1.0f),
    fDarkDose(0.0f),
    fBlackDose(1.0f),
    fdarkBBdose(0.0f),
    fFlatBBdose(1.0f),
    tau(0.97f),
    thresh(0.5f),
    bUseNormROI(true),
    bUseLUT(false),
    bUseWeightedMean(false),
    bUseBB(false),
    bUseExternalBB(false),
    bUseNormROIBB(false),
    bSameMask(true),
    bUseManualThresh(false),
    bPBvariante(true),
    bSaveBG(false),
    bExtSingleFile(true),
    nNormRegion(4,0UL),
    nOriginalNormRegion(4,0UL),
    BBroi(4,0UL),
    doseBBroi(4,0UL),
    dose_roi(4,0UL),
    radius(2),
    min_area(20),
    ferror(0.0f),
    ffirstAngle(0.0f),
    flastAngle(360.0f),
    fScanArc({0.0f,360.0f}),
    m_nWindow(5),
    m_ReferenceAverageMethod(ImagingAlgorithms::AverageImage::ImageWeightedAverage),
    m_ReferenceMethod(ImagingAlgorithms::ReferenceImageCorrection::ReferenceLogNorm),
    m_BBOptions(ImagingAlgorithms::ReferenceImageCorrection::Interpolate),
    m_xInterpOrder(ImagingAlgorithms::ReferenceImageCorrection::SecondOrder_x),
    m_yInterpOrder(ImagingAlgorithms::ReferenceImageCorrection::SecondOrder_y),
    m_InterpMethod(ImagingAlgorithms::ReferenceImageCorrection::Polynomial),
    m_maskCreationMethod(ImagingAlgorithms::ReferenceImageCorrection::otsuMask)
{
    blackbodyname = "./";
    blackbodysamplename = "./";
    blackbodyexternalname = "./";
    blackbodysampleexternalname = "./";
    blackbodyexternalmaskname = "./";
    path="./";
    flatname="./";
    darkname="./";
    pathBG="./";
    flatname_BG="flat_background.tif";
    filemask_BG="sample_background_####.tif";

    publications.push_back(Publication({"C. Carminati","P. Boillat","F. Schmid",
                                        "P. Vontobel","J. Hovind","M. Morgano",
                                        "M. Raventos","M. Siegwart","D. Mannes",
                                        "C. Gruenzweig","P. Trtik","E. Lehmann",
                                        "M. Strobl","A. Kaestner"},
                                       "Implementation and assessment of the black body bias correction in quantitative neutron imaging",
                                       "PLOS ONE",
                                       2019,
                                       14,
                                       1,
                                       "e0210300",
                                       "10.1371/journal.pone.0210300"
                                       ));

    publications.push_back(Publication({"P. Boillat","C. Carminati","F. Schmid",
                                        "C. Gruenzweig","J. Hovind","A. Kaestner",
                                        "D. Mannes","M. Morgano","M. Siegwart",
                                        "P. Trtik","P. Vontobel","E.H. Lehmann"},
                                     "Chasing quantitative biases in neutron imaging with scintillator-camera detectors: a practical method with black body grids",
                                     "Optics Express",
                                     2018,
                                     26,
                                     12,
                                     "15769",
                                     "10.1364/oe.26.015769"
                               ));


}

IMAGINGMODULESSHARED_EXPORT BBLogNorm::~BBLogNorm()
{

}

bool IMAGINGMODULESSHARED_EXPORT  BBLogNorm::updateStatus(float val, string msg){

    if (m_Interactor!=nullptr) {
        return m_Interactor->SetProgress(val,msg);
    }
    return false;

}

int IMAGINGMODULESSHARED_EXPORT BBLogNorm::Configure(KiplProcessConfig config, std::map<std::string, std::string> parameters)
{

    std::stringstream msg;
    msg<<"Configuring BBLogNorm::Configure";
    logger(kipl::logging::Logger::LogDebug,msg.str());

    m_Config    = config;

    m_nWindow = GetIntParameter(parameters,"window");
    string2enum(GetStringParameter(parameters,"avgmethod"),m_ReferenceAverageMethod);
    string2enum(GetStringParameter(parameters,"refmethod"), m_ReferenceMethod);
    string2enum(GetStringParameter(parameters,"BBOption"), m_BBOptions);
    string2enum(GetStringParameter(parameters, "X_InterpOrder"), m_xInterpOrder);
    string2enum(GetStringParameter(parameters, "Y_InterpOrder"), m_yInterpOrder);
    string2enum(GetStringParameter(parameters,"InterpolationMethod"), m_InterpMethod);
    string2enum(GetStringParameter(parameters,"MaskCreationMethod"), m_maskCreationMethod);

    path = GetStringParameter(parameters,"path"); // not sure it is used
    flatname = GetStringParameter(parameters, "OB_PATH");
    darkname = GetStringParameter(parameters, "DC_PATH");
    blackbodyname = GetStringParameter(parameters,"BB_OB_name");
    blackbodysamplename = GetStringParameter(parameters,"BB_samplename");
    blackbodyexternalname = GetStringParameter(parameters, "BB_OB_ext_name");
    blackbodysampleexternalname = GetStringParameter(parameters, "BB_sample_ext_name");
    blackbodyexternalmaskname = GetStringParameter(parameters, "BB_mask_ext_name");

    nBBextCount = GetIntParameter(parameters, "BB_ext_samplecounts");
    nBBextFirstIndex = GetIntParameter(parameters, "BB_ext_firstindex");
    nBBCount = GetIntParameter(parameters,"BB_counts");
    nBBFirstIndex = GetIntParameter(parameters, "BB_first_index");
    nBBSampleFirstIndex = GetIntParameter(parameters, "BB_sample_firstindex");
    nBBSampleCount = GetIntParameter(parameters,"BB_samplecounts");
    radius = GetIntParameter(parameters, "radius");
    min_area = GetIntParameter(parameters, "min_area");
    nOBFirstIndex = GetIntParameter(parameters, "OB_first_index");
    nOBCount = GetIntParameter(parameters,"OB_counts");
    nDCFirstIndex = GetIntParameter(parameters, "DC_first_index");
    nDCCount = GetIntParameter(parameters,"DC_counts");

    tau = GetFloatParameter(parameters, "tau");
    ffirstAngle = GetFloatParameter(parameters, "firstAngle");
    flastAngle = GetFloatParameter(parameters, "lastAngle");
    thresh = GetFloatParameter(parameters,"thresh");

    GetUIntParameterVector(parameters, "BBroi", BBroi, 4);
    GetUIntParameterVector(parameters, "doseBBroi", doseBBroi, 4);
    GetUIntParameterVector(parameters, "dose_roi", dose_roi, 4);
    GetFloatParameterVector(parameters, "fScanArc", fScanArc, 2);

    bSameMask = kipl::strings::string2bool(GetStringParameter(parameters,"SameMask"));
//    bUseManualThresh = kipl::strings::string2bool(GetStringParameter(parameters,"ManualThreshold"));
    bExtSingleFile = kipl::strings::string2bool(GetStringParameter(parameters, "singleBBext"));
    bPBvariante = kipl::strings::string2bool(GetStringParameter(parameters,"PBvariante"));
    bSaveBG = kipl::strings::string2bool(GetStringParameter(parameters,"SaveBG"));

    nOriginalNormRegion = dose_roi;

    if ( m_Config.mImageInformation.bUseROI) {
        msg<<"using image roi";
        logger(kipl::logging::Logger::LogDebug,msg.str());
    }


    //check on dose BB roi size
    if ((doseBBroi[2]-doseBBroi[0])<=0 || (doseBBroi[3]-doseBBroi[1])<=0){
        bUseNormROIBB=false;
        throw ImagingException("No roi is selected on BB images for dose correction. This is necessary for accurate BB referencing",__FILE__, __LINE__);
    }
    else {
        bUseNormROIBB = true;
    }

    if ((dose_roi[2]-dose_roi[0])<=0 || (dose_roi[3]-dose_roi[1])<=0 ){
        bUseNormROI=false;
        throw ImagingException("No roi is selected for dose correction. This is necessary for accurate BB referencing",__FILE__, __LINE__);
    }
    else{
        bUseNormROI=true;
    }

    if (enum2string(m_ReferenceMethod)=="LogNorm")
    {
        m_corrector.SetComputeMinusLog(true);
    }

    if (enum2string(m_ReferenceMethod)=="Norm")
    {
        m_corrector.SetComputeMinusLog(false);
    }

    switch (m_BBOptions){
    case (ImagingAlgorithms::ReferenceImageCorrection::noBB): {
        bUseBB = false;
        bUseExternalBB = false;
        break;
    }
    case (ImagingAlgorithms::ReferenceImageCorrection::Interpolate): {
        bUseBB = true;
        bUseExternalBB = false;
        break;
    }
    case (ImagingAlgorithms::ReferenceImageCorrection::Average): {
        bUseBB = true;
        bUseExternalBB = false;
        break;
    }
    case (ImagingAlgorithms::ReferenceImageCorrection::OneToOne): {
        bUseBB = true;
        bUseExternalBB = false;
        break;
    }
    case (ImagingAlgorithms::ReferenceImageCorrection::ExternalBB): {
        bUseBB = false; // to evaluate
        bUseExternalBB = true;
        break;
    }
    default: throw ImagingException("Unknown BBOption method in BBLogNorm::Configure",__FILE__,__LINE__);

    }

    if (bSaveBG){
        m_corrector.SaveBG(bSaveBG,pathBG,flatname_BG,filemask_BG);
    }

    switch (m_maskCreationMethod){
    case(ImagingAlgorithms::ReferenceImageCorrection::otsuMask): {
        bUseManualThresh = false;
        break;
    }
    case(ImagingAlgorithms::ReferenceImageCorrection::manuallyThresholdedMask): {
        bUseManualThresh = true;
        m_corrector.SetManualThreshold(thresh);
        break;
    }
    case(ImagingAlgorithms::ReferenceImageCorrection::userDefinedMask): {
        bUseManualThresh = false;
//        BBroi = m_Config.mImageInformation.nROI; // set BBroi to image roi
        break;
    }
    case (ImagingAlgorithms::ReferenceImageCorrection::referenceFreeMask): {
        bUseManualThresh = false;
        break;
    }
    default: throw ImagingException("Unknown m_maskCreationMethod method in BBLogNorm::Configure",__FILE__,__LINE__);
    }

    // for all cases except user provided mask, check on existence of BB roi
    if (m_maskCreationMethod != ImagingAlgorithms::ReferenceImageCorrection::userDefinedMask)
    {
        size_t roi_bb_x= BBroi[2]-BBroi[0];
        size_t roi_bb_y = BBroi[3]-BBroi[1];

        if (roi_bb_x>0 && roi_bb_y>0) {}
        else {
            throw ImagingException("Please select a ROI including BBs. This is necessary for creating the BB mask.",__FILE__, __LINE__);
        }
    }

    msg<<"Configuring done";
    logger(kipl::logging::Logger::LogDebug,msg.str());
    return 1;
}


int IMAGINGMODULESSHARED_EXPORT BBLogNorm::ConfigureDLG(KiplProcessConfig config, std::map<std::string, std::string> parameters)
{
    // config paramters
    m_Config    = config;
    path        = config.mImageInformation.sSourcePath;

    // string parameters
    flatname    = GetStringParameter(parameters, "OB_PATH");
    darkname    = GetStringParameter(parameters, "DC_PATH");
    blackbodyexternalmaskname = GetStringParameter(parameters, "BB_mask_ext_name");
    blackbodyname = GetStringParameter(parameters,"BB_OB_name");
    blackbodysamplename = GetStringParameter(parameters,"BB_samplename");
    blackbodyexternalname = GetStringParameter(parameters, "BB_OB_ext_name");
    blackbodysampleexternalname = GetStringParameter(parameters, "BB_sample_ext_name");
    flatname_BG=GetStringParameter(parameters,"flatname_BG");
    filemask_BG=GetStringParameter(parameters,"filemask_BG");
    pathBG = GetStringParameter(parameters,"path_BG");

    // int parameters
    nOBCount      = GetIntParameter(parameters,"OB_counts");
    nOBFirstIndex = GetIntParameter(parameters, "OB_first_index");
    nDCCount      = GetIntParameter(parameters,"DC_counts");
    nDCFirstIndex = GetIntParameter(parameters, "DC_first_index");
    m_nWindow = GetIntParameter(parameters,"window");
    nBBCount = GetIntParameter(parameters,"BB_counts");
    nBBFirstIndex = GetIntParameter(parameters, "BB_first_index");
    nBBSampleFirstIndex = GetIntParameter(parameters, "BB_sample_firstindex");
    nBBSampleCount = GetIntParameter(parameters,"BB_samplecounts");
    nBBextCount = GetIntParameter(parameters, "BB_ext_samplecounts");
    nBBextFirstIndex = GetIntParameter(parameters, "BB_ext_firstindex");
    radius = GetIntParameter(parameters, "radius");
    min_area = GetIntParameter(parameters, "min_area");

    // float paramters
    tau = GetFloatParameter(parameters, "tau");
    ffirstAngle = GetFloatParameter(parameters, "firstAngle");
    flastAngle = GetFloatParameter(parameters, "lastAngle");
    thresh = GetFloatParameter(parameters,"thresh");

    // vector paramters
    GetUIntParameterVector(parameters, "dose_roi", dose_roi, 4);
    GetUIntParameterVector(parameters, "BBroi", BBroi, 4);
    GetUIntParameterVector(parameters, "doseBBroi", doseBBroi, 4);
    GetFloatParameterVector(parameters, "fScanArc", fScanArc, 2);

    // enum paramters
    string2enum(GetStringParameter(parameters,"avgmethod"),m_ReferenceAverageMethod);
    string2enum(GetStringParameter(parameters,"refmethod"), m_ReferenceMethod);
    string2enum(GetStringParameter(parameters,"BBOption"), m_BBOptions);
    string2enum(GetStringParameter(parameters, "X_InterpOrder"), m_xInterpOrder);
    string2enum(GetStringParameter(parameters, "Y_InterpOrder"), m_yInterpOrder);
    string2enum(GetStringParameter(parameters,"InterpolationMethod"), m_InterpMethod);
    string2enum(GetStringParameter(parameters,"MaskCreationMethod"), m_maskCreationMethod);


    // boolean parameters
    bPBvariante = kipl::strings::string2bool(GetStringParameter(parameters,"PBvariante"));
    bSameMask = kipl::strings::string2bool(GetStringParameter(parameters,"SameMask"));
    bExtSingleFile = kipl::strings::string2bool(GetStringParameter(parameters, "singleBBext"));
    bSaveBG = kipl::strings::string2bool(GetStringParameter(parameters,"SaveBG"));

   // removed parameters
//    bUseManualThresh = kipl::strings::string2bool(GetStringParameter(parameters,"ManualThreshold"));

   nOriginalNormRegion = dose_roi;

    // settigs and controls
    //check on dose BB roi size
    if ((doseBBroi[2]-doseBBroi[0])<=0 || (doseBBroi[3]-doseBBroi[1])<=0){
        bUseNormROIBB=false;
    }
    else {
        bUseNormROIBB = true;
    }

    if ((dose_roi[2]-dose_roi[0])<=0 || (dose_roi[3]-dose_roi[1])<=0 ){
        bUseNormROI=false;
        throw ImagingException("No roi is selected for dose correction. This is necessary for accurate BB referencing",__FILE__, __LINE__);
    }
    else{
        bUseNormROI=true;
    }

    if (enum2string(m_ReferenceMethod)=="LogNorm"){
        m_corrector.SetComputeMinusLog(true);
    }

    if (enum2string(m_ReferenceMethod)=="Norm") {
        m_corrector.SetComputeMinusLog(false);
    }

    switch (m_BBOptions){
    case (ImagingAlgorithms::ReferenceImageCorrection::noBB): {
        bUseBB = false;
        bUseExternalBB = false;
        break;
    }
    case (ImagingAlgorithms::ReferenceImageCorrection::Interpolate): {
        bUseBB = true;
        bUseExternalBB = false;
        break;
    }
    case (ImagingAlgorithms::ReferenceImageCorrection::Average): {
        bUseBB = true;
        bUseExternalBB = false;
        break;
    }
    case (ImagingAlgorithms::ReferenceImageCorrection::OneToOne): {
        bUseBB = true;
        bUseExternalBB = false;
        break;
    }
    case (ImagingAlgorithms::ReferenceImageCorrection::ExternalBB): {
        bUseBB = false; // to evaluate
        bUseExternalBB = true;
        break;
    }
    default: throw ImagingException("Unknown BBOption method in BBLogNorm::Configure",__FILE__,__LINE__);

    }

    switch (m_maskCreationMethod){
    case(ImagingAlgorithms::ReferenceImageCorrection::otsuMask): {
        bUseManualThresh = false;
        break;
    }
    case(ImagingAlgorithms::ReferenceImageCorrection::manuallyThresholdedMask): {
        bUseManualThresh = true;
        break;
    }
    case(ImagingAlgorithms::ReferenceImageCorrection::userDefinedMask): {
        bUseManualThresh = false;
        break;
    }
    case (ImagingAlgorithms::ReferenceImageCorrection::referenceFreeMask): {
        bUseManualThresh = false;
        break;
    }
    default: throw ImagingException("Unknown m_maskCreationMethod method in BBLogNorm::Configure",__FILE__,__LINE__);
    }

    m_corrector.SetManualThreshold(thresh);

    return 1;
}

bool IMAGINGMODULESSHARED_EXPORT BBLogNorm::SetROI(const std::vector<size_t> &roi)
{

    std::stringstream msg;
    if (!roi.empty())
    {
        msg<<"ROI=["<<roi[0]<<" "<<roi[1]<<" "<<roi[2]<<" "<<roi[3]<<"]";
        logger(kipl::logging::Logger::LogDebug,msg.str());
    }

    nNormRegion = nOriginalNormRegion; //nNormRegion seems not used

    if (m_Config.mImageInformation.bUseROI)
    {
        LoadReferenceImages(roi);
    }
    else
    {
        LoadReferenceImages({});
    }
    return true;
}


std::map<std::string, std::string> IMAGINGMODULESSHARED_EXPORT   BBLogNorm::GetParameters() {

    std::map<std::string, std::string> parameters;

    std::stringstream msg;
    msg<<"Getting Parameters for BBLogNorm";
    logger(kipl::logging::Logger::LogDebug,msg.str());

    parameters["window"] = kipl::strings::value2string(m_nWindow);
    parameters["avgmethod"] = enum2string(m_ReferenceAverageMethod);
    parameters["refmethod"] = enum2string(m_ReferenceMethod);
    parameters["BB_OB_name"] = blackbodyname;
    parameters["BB_counts"] = kipl::strings::value2string(nBBCount);
    parameters["BB_first_index"] = kipl::strings::value2string(nBBFirstIndex);
    parameters["BB_samplename"] = blackbodysamplename;
    parameters["BB_samplecounts"] = kipl::strings::value2string(nBBSampleCount);
    parameters["BB_sample_firstindex"] = kipl::strings::value2string(nBBSampleFirstIndex);
    parameters["tau"] = kipl::strings::value2string(tau);
    parameters["radius"] = kipl::strings::value2string(radius);
    parameters["BBroi"] = kipl::strings::value2string(BBroi[0])+" "+kipl::strings::value2string(BBroi[1])+" "+kipl::strings::value2string(BBroi[2])+" "+kipl::strings::value2string(BBroi[3]);
    parameters["doseBBroi"] = kipl::strings::value2string(doseBBroi[0])+" "+kipl::strings::value2string(doseBBroi[1])+" "+kipl::strings::value2string(doseBBroi[2])+" "+kipl::strings::value2string(doseBBroi[3]);
    parameters["PBvariante"] = kipl::strings::bool2string(bPBvariante);
    parameters["BBOption"] = enum2string(m_BBOptions);
    parameters["firstAngle"] = kipl::strings::value2string(ffirstAngle);
    parameters["lastAngle"] = kipl::strings::value2string(flastAngle);
    parameters["X_InterpOrder"] = enum2string(m_xInterpOrder);
    parameters["Y_InterpOrder"] = enum2string(m_yInterpOrder);
    parameters["InterpolationMethod"] = enum2string(m_InterpMethod);
    parameters["BB_OB_ext_name"] = blackbodyexternalname;
    parameters["BB_sample_ext_name"] = blackbodysampleexternalname;
    parameters["BB_ext_samplecounts"] = kipl::strings::value2string(nBBextCount);
    parameters["BB_ext_firstindex"] = kipl::strings::value2string(nBBextFirstIndex);
    parameters["SameMask"] = kipl::strings::bool2string(bSameMask);
    parameters["min_area"] = kipl::strings::value2string(min_area);
//    parameters["ManualThreshold"] = kipl::strings::bool2string(bUseManualThresh);
    parameters["thresh"]= kipl::strings::value2string(thresh);

    parameters["BB_mask_ext_name"] = blackbodyexternalmaskname;
    parameters["MaskCreationMethod"] = enum2string(m_maskCreationMethod);

    parameters["OB_PATH"]= flatname;
    parameters["DC_PATH"]= darkname;
    parameters["OB_first_index"] = kipl::strings::value2string(nOBFirstIndex);
    parameters["OB_counts"] = kipl::strings::value2string(nOBCount);
    parameters["DC_counts"] = kipl::strings::value2string(nDCCount);
    parameters["DC_first_index"] = kipl::strings::value2string(nDCFirstIndex);
    parameters["dose_roi"] =  kipl::strings::value2string(dose_roi[0])+" "+kipl::strings::value2string(dose_roi[1])+" "+kipl::strings::value2string(dose_roi[2])+" "+kipl::strings::value2string(dose_roi[3]);
    parameters["fScanArc"] =  kipl::strings::value2string(fScanArc[0])+" "+kipl::strings::value2string(fScanArc[1]);
    parameters["path"]= path;
    parameters["SaveBG"] = kipl::strings::bool2string(bSaveBG);
    parameters["path_BG"] = pathBG;
    parameters["flatname_BG"] = flatname_BG;
    parameters["filemask_BG"] = filemask_BG;
    parameters["singleBBext"] = kipl::strings::bool2string(bExtSingleFile);

    msg<<"end of BBLogNorm::GetParameters";
    logger(kipl::logging::Logger::LogDebug,msg.str());

    return parameters;
}

void IMAGINGMODULESSHARED_EXPORT  BBLogNorm::LoadReferenceImages(const std::vector<size_t> &roi)
{

    std::stringstream msg;
    if (!roi.empty())
    {
        msg<<"Loading reference images with roi: "<< roi[0] << " " << roi[1] << " " << roi[2] <<" " << roi[3];
        logger(kipl::logging::Logger::LogDebug,msg.str());
    }


    if (flatname.empty() && nOBCount!=0)
        throw ImagingException("The flat field image mask is empty",__FILE__,__LINE__);
    if (darkname.empty() && nDCCount!=0)
        throw ImagingException("The dark current image mask is empty",__FILE__,__LINE__);


    kipl::base::TImage<float,2> myflat, mydark;

    fDarkDose=0.0f;
    fFlatDose=1.0f;
    std::string flatmask=flatname;
    std::string darkmask=darkname;

    myflat = ReferenceLoader(flatmask,nOBFirstIndex,nOBCount,roi,1.0f,0.0f,m_Config, fFlatDose); // i don't use the bias.. beacuse i think i use it later on
    mydark = ReferenceLoader(darkmask,nDCFirstIndex,nDCCount,roi,0.0f,0.0f,m_Config, fDarkDose);
    SetReferenceImages(mydark,myflat);

    if (bUseExternalBB && nBBextCount!=0)
    {
        try
        {
            LoadExternalBBData(roi); // they must be ready for SetReferenceImages
        }
        catch (...)
        {
            throw ImagingException("Error while loading external BB data", __FILE__, __LINE__);

        }
    }

    m_corrector.SetReferenceImages(&mflat,
                                   &mdark,
                                   (bUseBB && nBBCount!=0 && nBBSampleCount!=0),
                                   (bUseExternalBB && nBBextCount!=0),
                                   bExtSingleFile,
                                   fFlatDose,
                                   fDarkDose,
                                   (bUseNormROIBB && bUseNormROI),
                                   roi,
                                   dose_roi);

     msg<<"References loaded";
     logger(kipl::logging::Logger::LogDebug,msg.str());

}

void IMAGINGMODULESSHARED_EXPORT  BBLogNorm::LoadExternalBBData(const std::vector<size_t> &roi)
{
    if (blackbodyexternalname.empty())
        throw ImagingException("The pre-processed open beam with BB image mask is empty", __FILE__, __LINE__);
    if (blackbodysampleexternalname.empty() || nBBextCount==0)
        throw ImagingException("The pre-processed sample with BB image mask is empty", __FILE__, __LINE__);

    kipl::base::TImage<float,2> bb_ext;
    // kipl::base::TImage<float,3> bb_sample_ext;
    float dose;

    bb_ext = BBExternalLoader(blackbodyexternalname, roi, dose);

    if (bExtSingleFile)
    {
        kipl::base::TImage<float,2> bb_sample_ext2;
        float dose_s;
        bb_sample_ext2 = BBExternalLoader(blackbodysampleexternalname, roi, dose_s);
        m_corrector.SetExternalBBimages(bb_ext, bb_sample_ext2, dose, dose_s);

    }
    else
    {
        kipl::base::TImage<float,3> bb_sample_ext3;
        std::vector<float> doselist;
        bb_sample_ext3 = BBExternalLoader(blackbodysampleexternalname,
                                         nBBextCount,
                                         roi,
                                         nBBextFirstIndex,
                                         doselist);
        m_corrector.SetExternalBBimages(bb_ext, bb_sample_ext3, dose, doselist);
    }


}

void IMAGINGMODULESSHARED_EXPORT  BBLogNorm::PrepareBBData(){

    logger(kipl::logging::Logger::LogDebug,"PrepareBBData begin--");

    if (flatname.empty() && nOBCount!=0)
        throw ImagingException("The flat field image mask is empty",__FILE__,__LINE__);
    if (darkname.empty() && nDCCount!=0)
        throw ImagingException("The dark current image mask is empty",__FILE__,__LINE__);

    if (blackbodyname.empty() && nBBCount!=0)
        throw ImagingException("The open beam image with BBs image mask is empty",__FILE__,__LINE__);
    if (blackbodysamplename.empty() && nBBSampleCount!=0)
        throw ImagingException("The sample image with BBs image mask is empty", __FILE__,__LINE__);

    if (m_maskCreationMethod != ImagingAlgorithms::ReferenceImageCorrection::userDefinedMask)
    {
        std::vector<int> diffroi(BBroi.begin(), BBroi.end());
         m_corrector.setDiffRoi(diffroi);
    }
    else
    {
        std::vector<int> diffroi(4, 0);
        m_corrector.setDiffRoi(diffroi);
    }


    m_corrector.SetRadius(radius);
    m_corrector.SetTau(tau);
    m_corrector.SetPBvariante(bPBvariante);
    m_corrector.SetInterpolationOrderX(m_xInterpOrder);
    m_corrector.SetInterpolationOrderY(m_yInterpOrder);
    m_corrector.SetMinArea(min_area);
    m_corrector.SetInterpolationMethod(m_InterpMethod);

    size_t nProj=(m_Config.mImageInformation.nLastFileIndex-m_Config.mImageInformation.nFirstFileIndex+1); // here I only need the number of images loaded.. and not these computation relevant to tomography

    switch (m_InterpMethod) {
        case (ImagingAlgorithms::ReferenceImageCorrection::Polynomial): {

            PreparePolynomialInterpolationParameters();

            if (nBBCount!=0 && nBBSampleCount!=0) {
                 m_corrector.SetInterpParameters(ob_bb_param, sample_bb_param,nBBSampleCount, nProj, m_BBOptions);
            }

            break;
        }
        case(ImagingAlgorithms::ReferenceImageCorrection::ThinPlateSplines):{
             logger(kipl::logging::Logger::LogDebug,"ThinPlateSplines");

            int nBBs = PrepareSplinesInterpolationParameters();

             logger(kipl::logging::Logger::LogDebug,"After PrepareSplinesInterpolationParameters ");

            if (nBBCount!=0 && nBBSampleCount!=0) {
                m_corrector.SetSplinesParameters(ob_bb_param, sample_bb_param, nBBSampleCount, nProj, m_BBOptions, nBBs); // i also need the coordinates.
            }
            break;
        }
        default: throw ImagingException("Unknown m_InterpMethod in BBLogNorm::PrepareBBData()", __FILE__, __LINE__);
        }
}

void IMAGINGMODULESSHARED_EXPORT  BBLogNorm::PreparePolynomialInterpolationParameters()
{

    kipl::base::TImage<float,2> flat, dark, bb, sample, samplebb;
    std::vector<float> bb_ob_param(6);
    std::vector<float> bb_sample_parameters;

    std::string flatmask=flatname;
    std::string darkmask=darkname;

    fdarkBBdose=0.0f;
    fFlatBBdose=1.0f;

    // reload the OB and DC into the BBroi and doseBBroi
    dark = BBLoader(darkmask,nDCFirstIndex,nDCCount,0.0f,0.0f, fdarkBBdose);
    flat = BBLoader(flatmask,nOBFirstIndex,nOBCount,1.0f,0.0f, fFlatBBdose); // to check if i have to use dosebias to remove the fdarkBBdose

    // load OB image with BBs
    bb = BBLoader(blackbodyname,nBBFirstIndex,nBBCount,1.0f,fdarkBBdose, fBlackDose); // this is for mask computation and dose correction (small roi)

    kipl::base::TImage<float,2> obmask(bb.dims());
    std::stringstream msg;


    try {

        // compute mask for different cases
        switch (m_maskCreationMethod) {
        case (ImagingAlgorithms::ReferenceImageCorrection::otsuMask): {
            bb_ob_param = m_corrector.PrepareBlackBodyImage(flat,dark,bb, obmask, ferror);
            break;
            }
        case (ImagingAlgorithms::ReferenceImageCorrection::manuallyThresholdedMask): {
            bb_ob_param = m_corrector.PrepareBlackBodyImage(flat,dark,bb, obmask, ferror);
            // write mask here to debug
//            kipl::io::WriteTIFF(obmask,"mask_inthresh.tiff",kipl::base::Float32);
            break;
            }
        case (ImagingAlgorithms::ReferenceImageCorrection::userDefinedMask): {
            obmask = LoadUserDefinedMask();
            bb_ob_param = m_corrector.PrepareBlackBodyImagewithMask(dark,bb, obmask);
            break;
        }
        case (ImagingAlgorithms::ReferenceImageCorrection::referenceFreeMask): {
            // to be implemented
            throw ImagingException("referenceFreeMask method under development, please choose another method",__FILE__,__LINE__);
            break;
        }

        default: throw ImagingException("trying to switch to unknown m_maskCreationMethod in PrepareBBdata",__FILE__,__LINE__);
        }

    }
    catch (ModuleException &e) {
        msg.str(""); msg<<"Failed to compute bb_ob_parameters. Try to change maskCreationMethod. " << e.what();
        logger(kipl::logging::Logger::LogDebug,msg.str());
        throw ModuleException("Failed to compute bb_ob_parameters. Try to change maskCreationMethod. ", __FILE__,__LINE__);
    }

    catch(ImagingException &e){
        msg.str(""); msg<<"Failed to compute bb_ob_parameters. Try to change maskCreationMethod. " << e.what();
        logger(kipl::logging::Logger::LogDebug,msg.str());
        throw ImagingException("Failed to compute bb_ob_parameters. Try to change maskCreationMethod. ", __FILE__,__LINE__);
    }

    catch(kipl::base::KiplException &e){
        msg.str(""); msg<<"Failed to compute bb_ob_parameters. Try to change maskCreationMethod. " << e.what();
        logger(kipl::logging::Logger::LogDebug,msg.str());
        throw kipl::base::KiplException("Failed to compute bb_ob_parameters. Try to change maskCreationMethod. ", __FILE__,__LINE__);
    }


    if (bPBvariante) {
     kipl::base::TImage<float,2> mybb = m_corrector.InterpolateBlackBodyImage(bb_ob_param,doseBBroi);
     float mydose = computedose(mybb);
     fBlackDose = fBlackDose + ((1.0/tau-1.0)*mydose);
    }

    if(bUseNormROI && bUseNormROIBB){
     fBlackDose = fBlackDose<1 ? 1.0f : fBlackDose;
     for (size_t i=0; i<6; i++){
         bb_ob_param[i]= bb_ob_param[i]*(fFlatBBdose-fdarkBBdose);
         bb_ob_param[i]= bb_ob_param[i]/(fBlackDose*tau); // now the dose is already computed for I_OB_BB
     }
    }

    // ob_bb_param.resize(6)
    ob_bb_param = bb_ob_param;
    // memcpy(ob_bb_param, bb_ob_param, sizeof(float)*6);

    // load sample images with BBs and sample images
    std::vector<float> temp_parameters;
    size_t nProj=(m_Config.mImageInformation.nLastFileIndex-m_Config.mImageInformation.nFirstFileIndex+1);
    float angles[4] = {fScanArc[0], fScanArc[1], ffirstAngle, flastAngle};
    m_corrector.SetAngles(angles, nProj, nBBSampleCount);

    std::vector<float> doselist(nProj,0.0f);
    for (size_t i=0; (i<nProj && (updateStatus(float(i)/nProj,"Loading dose for BB images")==false) ) ; i++)
    {
        doselist[i] = DoseBBLoader(m_Config.mImageInformation.sSourceFileMask, m_Config.mImageInformation.nFirstFileIndex+i, 1.0f, fdarkBBdose); // D(I*n-Idc) in the doseBBroi
    }

    m_corrector.SetDoseList(doselist);

     switch (m_BBOptions)
     {

         case (ImagingAlgorithms::ReferenceImageCorrection::Interpolate): 
         {

         //bb_sample_parameters = new float[6*nBBSampleCount];
         //sample_bb_param = new float[6*nBBSampleCount];
        //  temp_parameters = new float[6];

         if (nBBSampleCount!=0) {

             logger(kipl::logging::Logger::LogDebug,"Loading sample images with BB");

             for (size_t i=0; (i<nBBSampleCount && (updateStatus(float(i)/nBBSampleCount,"Calculating Polynomial interpolation")==false)); i++) {
                 samplebb = BBLoader(blackbodysamplename,i+nBBSampleFirstIndex,1,1.0f,fdarkBBdose, fBlackDoseSample);

                 float dosesample; // used for the correct dose roi computation (doseBBroi)
                 float current_dose; // current dose for sample with BBs
                 int index;

                 index = GetnProjwithAngle((angles[2]+(angles[3]-angles[2])/(nBBSampleCount-1)*i));

                 // in the first case compute the mask again (should not be necessary in well acquired datasets) however this requires that the first image (or at least one) of sample image with BB has the same angle of the sample image without BB
                 if (i==0)
                 {
                     if (!bSameMask && m_maskCreationMethod!=ImagingAlgorithms::ReferenceImageCorrection::userDefinedMask)
                     {
                         sample = BBLoader(m_Config.mImageInformation.sSourceFileMask, m_Config.mImageInformation.nFirstFileIndex+index, 1, 1.0f,fdarkBBdose, dosesample);
                         kipl::base::TImage<float,2> mask(sample.dims());
                         mask = 0.0f;
                         temp_parameters = m_corrector.PrepareBlackBodyImage(sample,dark,samplebb, mask);
                         mMaskBB = mask; // or memcpy

                     }
                     else
                     {
                         mMaskBB = obmask;
                         temp_parameters = m_corrector.PrepareBlackBodyImagewithMask(dark,samplebb, mMaskBB);
                     }

//                             if (bSameMask){
//                                mMaskBB = obmask;
//                                temp_parameters = m_corrector.PrepareBlackBodyImagewithMask(dark,samplebb, mMaskBB);
//                             }
//                             else {
//                                 sample = BBLoader(m_Config.mImageInformation.sSourceFileMask, m_Config.mImageInformation.nFirstFileIndex+index, 1, 1.0f,fdarkBBdose, dosesample);
//                                 kipl::base::TImage<float,2> mask(sample.dims());
//                                 mask = 0.0f;
//                                 temp_parameters = m_corrector.PrepareBlackBodyImage(sample,dark,samplebb, mask);
//                                 mMaskBB = mask; // or memcpy
//                             }

                     if (bUseNormROIBB && bUseNormROI){
//                     prenormalize interpolation parameters with dose
                         current_dose = fBlackDoseSample;
                         if (bPBvariante)
                         {
                             kipl::base::TImage<float,2> mybb = m_corrector.InterpolateBlackBodyImage(temp_parameters,doseBBroi);
                             float mydose = computedose(mybb);
                             current_dose = current_dose + ((1.0/tau-1.0)*mydose);
                         }

                         current_dose = current_dose<1 ? 1.0f : current_dose;

                         for(size_t j=0; j<6; j++)
                         {
                             temp_parameters[j]/=(current_dose);
                         }
                     }

                     bb_sample_parameters = temp_parameters;
                    //  memcpy(bb_sample_parameters, temp_parameters, sizeof(float)*6);
                 }
                 else
                 {
                     temp_parameters = m_corrector.PrepareBlackBodyImagewithMask(dark,samplebb,mMaskBB);
                     current_dose = fBlackDoseSample;

                     if (bPBvariante) {
                             kipl::base::TImage<float,2> mybb = m_corrector.InterpolateBlackBodyImage(temp_parameters,doseBBroi);
                             float mydose = computedose(mybb);
                             current_dose = current_dose + ((1.0/tau-1.0)*mydose);
                          }

                     current_dose = current_dose<1 ? 1.0f : current_dose;

                     for(size_t j=0; j<6; j++) 
                     {
                      temp_parameters[j]/=(current_dose);
                     }

                     std::copy_n(temp_parameters.begin(),6,bb_sample_parameters.begin()+i*6);
                    //  memcpy(bb_sample_parameters+i*6, temp_parameters, sizeof(float)*6);
                 }
         }
         }

        sample_bb_param = bb_sample_parameters;
        // memcpy(sample_bb_param, bb_sample_parameters, sizeof(float)*6*nBBSampleCount);
        break;
        }

         case (ImagingAlgorithms::ReferenceImageCorrection::Average): {

        //  bb_sample_parameters = new float[6];
        //  sample_bb_param = new float[6];
        //  temp_parameters = new float[6];
         std::vector<float> mask_parameters(6);

         kipl::base::TImage<float,2> samplebb_temp;
         float dose_temp;
         float dosesample; // used for the correct dose roi computation (doseBBroi)
         float current_dose;
         int index;

         samplebb_temp = BBLoader(blackbodysamplename, nBBSampleFirstIndex,1,1.0f,0.0f,  dose_temp);
         samplebb = BBLoader(blackbodysamplename, nBBSampleFirstIndex,nBBSampleCount,1.0f,fdarkBBdose, fBlackDoseSample);
         index = GetnProjwithAngle(ffirstAngle);
         sample = BBLoader(m_Config.mImageInformation.sSourceFileMask,
                           m_Config.mImageInformation.nFirstFileIndex +index, 1, 1.0f,fdarkBBdose, dosesample); // load the projection with angle closest to the first BB sample data

         if (!bSameMask && m_maskCreationMethod!=ImagingAlgorithms::ReferenceImageCorrection::userDefinedMask)
         {
             kipl::base::TImage<float,2> mask(sample.dims());
             mask = 0.0f;
             temp_parameters = m_corrector.PrepareBlackBodyImage(sample,dark,samplebb, mask);
             mMaskBB = mask; // or memcpy

         }
         else
         {
             mMaskBB = obmask;
             temp_parameters = m_corrector.PrepareBlackBodyImagewithMask(dark,samplebb, mMaskBB);
         }

         //  prenormalize interpolation parameters with dose
         if (bUseNormROIBB && bUseNormROI){

             current_dose = fBlackDoseSample;

             if (bPBvariante)
             {
                 kipl::base::TImage<float,2> mybb = m_corrector.InterpolateBlackBodyImage(temp_parameters,doseBBroi);
                 float mydose = computedose(mybb);
                 current_dose+= ((1.0/tau-1.0)*mydose);
              }

             current_dose = current_dose<1 ? 1.0f : current_dose;

             for(size_t j=0; j<6; j++)
             {
                 temp_parameters[j]/=current_dose;
             }
         }

        sample_bb_param = temp_parameters;
        // memcpy(sample_bb_param, temp_parameters, sizeof(float)*6);
        //  delete [] mask_parameters;
         break;
         }

        case (ImagingAlgorithms::ReferenceImageCorrection::OneToOne): {

        bb_sample_parameters.resize(6*nBBSampleCount);
        sample_bb_param.resize(6*nBBSampleCount);
        // bb_sample_parameters = new float[6*nBBSampleCount];
        // sample_bb_param = new float[6*nBBSampleCount];
        // temp_parameters = new float[6];
        float dosesample;
        float current_dose;

        for (size_t i=0; (i<nBBSampleCount && (updateStatus(float(i)/nBBSampleCount,"Calculating Polynomial interpolation")==false)); i++)
        {
            samplebb = BBLoader(blackbodysamplename,i+nBBSampleFirstIndex,1,1.0f,fdarkBBdose, fBlackDoseSample);
            sample = BBLoader(m_Config.mImageInformation.sSourceFileMask, m_Config.mImageInformation.nFirstFileIndex, 1, 1.0f,fdarkBBdose, dosesample);

            // compute the mask again only for the first sample image, than assume BBs do not move during experiment
            if (i==0)
            {
                if (!bSameMask && m_maskCreationMethod!=ImagingAlgorithms::ReferenceImageCorrection::userDefinedMask)
                {
                    kipl::base::TImage<float,2> mask(sample.dims());
                    mask = 0.0f;
                    temp_parameters = m_corrector.PrepareBlackBodyImage(sample,dark,samplebb, mask);
                    mMaskBB = mask; // or memcpy

                }
                else
                {
                    mMaskBB = obmask;
                    temp_parameters = m_corrector.PrepareBlackBodyImagewithMask(dark,samplebb, mMaskBB);
                }

                if (bUseNormROIBB && bUseNormROI)
                {
    //                     prenormalize interpolation parameters with dose
                    current_dose= fBlackDoseSample;
                    if (bPBvariante)
                    {
                        kipl::base::TImage<float,2> mybb = m_corrector.InterpolateBlackBodyImage(temp_parameters,doseBBroi); // previously doseBBroi
                        float mydose = computedose(mybb);
                        current_dose += ((1.0/tau-1.0)*mydose);
                    }
                    current_dose = current_dose<1 ? 1.0f : current_dose;

                    for(size_t j=0; j<6; j++)
                    {
                        temp_parameters[j]/=current_dose;
                    }
                }

                bb_sample_parameters=temp_parameters;
                // memcpy(bb_sample_parameters, temp_parameters, sizeof(float)*6);
            }
            else
            {
                temp_parameters = m_corrector.PrepareBlackBodyImagewithMask(dark,samplebb,mMaskBB);
                current_dose= fBlackDoseSample;

                if (bPBvariante)
                {
                    kipl::base::TImage<float,2> mybb = m_corrector.InterpolateBlackBodyImage(temp_parameters,doseBBroi); // previously doseBBroi
                    float mydose = computedose(mybb);
                    current_dose+= ((1.0/tau-1.0)*mydose);
                }

                current_dose = current_dose<1 ? 1.0f : current_dose;
                for(size_t j=0; j<6; j++)
                {
                     temp_parameters[j]/=current_dose;
                }

                std::copy_n(temp_parameters.begin(),6,bb_sample_parameters.begin()+i*6);
                // memcpy(bb_sample_parameters+i*6, temp_parameters, sizeof(float)*6);
            }
        }

        sample_bb_param=bb_sample_parameters;
        // memcpy(sample_bb_param, bb_sample_parameters, sizeof(float)*6*nBBSampleCount);
        break;
        }

         case (ImagingAlgorithms::ReferenceImageCorrection::noBB): 
         {
               throw ImagingException("trying to switch to case ImagingAlgorithms::ReferenceImageCorrection::noBB in PrepareBBdata",__FILE__,__LINE__);
               break;
         }
         case (ImagingAlgorithms::ReferenceImageCorrection::ExternalBB) : 
         {
             throw ImagingException("trying to switch to case ImagingAlgorithms::ReferenceImageCorrection::ExternalBB in PrepareBBdata",__FILE__,__LINE__);
         }

         default: 
            throw ImagingException("trying to switch to unknown BBOption in PrepareBBdata",__FILE__,__LINE__);

    }
}

int IMAGINGMODULESSHARED_EXPORT  BBLogNorm::PrepareSplinesInterpolationParameters()
{
    kipl::base::TImage<float,2> flat, dark, bb, sample, samplebb;
    std::string flatmask=flatname;
    std::string darkmask=darkname;

    fdarkBBdose=0.0f;
    fFlatBBdose=1.0f;

    // reload the OB and DC into the BBroi and doseBBroi
    dark = BBLoader(darkmask,nDCFirstIndex,nDCCount,0.0f,0.0f,fdarkBBdose);
    flat = BBLoader(flatmask,nOBFirstIndex,nOBCount,1.0f,0.0f,fFlatBBdose); //
    bb = BBLoader(blackbodyname,nBBFirstIndex,nBBCount,1.0f,fdarkBBdose,fBlackDose);

    kipl::base::TImage<float,2> obmask(bb.dims());

    std::vector<float> bb_ob_param;
    std::vector<float> bb_sample_parameters;

    std::map<std::pair<int, int>, float> values;
    std::map<std::pair<int, int>, float> values_bb;

    std::ostringstream msg;

    try {
        // compute mask for different cases
        switch (m_maskCreationMethod) {
        case (ImagingAlgorithms::ReferenceImageCorrection::otsuMask): {
            bb_ob_param = m_corrector.PrepareBlackBodyImagewithSplines(flat,dark,bb, obmask, values);
            break;
            }
        case (ImagingAlgorithms::ReferenceImageCorrection::manuallyThresholdedMask): {
            bb_ob_param = m_corrector.PrepareBlackBodyImagewithSplines(flat,dark,bb, obmask, values);
            break;
            }
        case (ImagingAlgorithms::ReferenceImageCorrection::userDefinedMask): {
            obmask = LoadUserDefinedMask();
            bb_ob_param = m_corrector.PrepareBlackBodyImagewithSplinesAndMask(dark,bb, obmask, values);
            break;
        }
        case (ImagingAlgorithms::ReferenceImageCorrection::referenceFreeMask): {
            // to be implemented
            throw ImagingException("referenceFreeMask method under development, please choose another method",__FILE__,__LINE__);
            break;
        }

        default: throw ImagingException("trying to switch to unknown m_maskCreationMethod in PrepareBBdata",__FILE__,__LINE__);
        }
//        bb_ob_param = m_corrector.PrepareBlackBodyImagewithSplines(flat,dark,bb, obmask, values);
    }
    catch (ModuleException &e) {
        msg.str(""); msg<<"Failed to compute bb_ob_parameters. Try to change thresholding method or value. " << e.what();
        logger(kipl::logging::Logger::LogDebug,msg.str());
        throw ModuleException("Failed to compute bb_ob_parameters. Try to change thresholding method or value. ", __FILE__,__LINE__);
    }
    catch(ImagingException &e){
        msg.str(""); msg<<"Failed to compute bb_ob_parameters. Try to change thresholding method or value. " << e.what();
        logger(kipl::logging::Logger::LogDebug,msg.str());
        throw ImagingException("Failed to compute bb_ob_parameters. Try to change thresholding method or value. ", __FILE__,__LINE__);
    }
    catch(kipl::base::KiplException &e){
        msg.str(""); msg<<"Failed to compute bb_ob_parameters. Try to change thresholding method or value. " << e.what();
        logger(kipl::logging::Logger::LogDebug,msg.str());
        throw kipl::base::KiplException("Failed to compute bb_ob_parameters. Try to change thresholding method or value. ", __FILE__,__LINE__);
    }

     if (bPBvariante)
     {
      kipl::base::TImage<float,2> mybb = m_corrector.InterpolateBlackBodyImagewithSplines(bb_ob_param,values, doseBBroi);
      float mydose = computedose(mybb);
      fBlackDose = fBlackDose + ((1.0/tau-1.0)*mydose);
     }

     m_corrector.SetSplineObValues(values);

     if(bUseNormROI && bUseNormROIBB)
     {
      fBlackDose = fBlackDose<1 ? 1.0f : fBlackDose;
      for (size_t i=0; i<values.size()+3; i++){
          bb_ob_param[i]= bb_ob_param[i]*(fFlatBBdose-fdarkBBdose);
          bb_ob_param[i]= bb_ob_param[i]/(fBlackDose*tau); // now the dose is already computed for I_OB_BB
      }
     }

    //  ob_bb_param = new float[values.size()+3];

    //  memcpy(ob_bb_param, bb_ob_param, sizeof(float)*(values.size()+3));
     ob_bb_param=bb_ob_param;


     std::vector<float> temp_parameters;
     size_t nProj=(m_Config.mImageInformation.nLastFileIndex-m_Config.mImageInformation.nFirstFileIndex+1);
     size_t step = (nProj)/(nBBSampleCount);

     float angles[4] = {fScanArc[0], fScanArc[1], ffirstAngle, flastAngle};
     m_corrector.SetAngles(angles, nProj, nBBSampleCount);

     std::vector<float> doselist(nProj,0.0f);
     for (size_t i=0; (i<nProj && (updateStatus(float(i)/nProj,"Loading dose for BB images")==false) ); i++)
     {
         doselist[i] = DoseBBLoader(m_Config.mImageInformation.sSourceFileMask, m_Config.mImageInformation.nFirstFileIndex+i,
                                    1.0f, fdarkBBdose); // D(I*n-Idc) in the doseBBroi
     }

     m_corrector.SetDoseList(doselist);

    // here Exceptions need to be added to veirfy if the selected module is compatible with the number of loaded images
    switch (m_BBOptions) {

    case (ImagingAlgorithms::ReferenceImageCorrection::Interpolate): {

        //   bb_sample_parameters = new float[(values.size()+3)*nBBSampleCount];
        //   sample_bb_param = new float[(values.size()+3)*nBBSampleCount];
        //   temp_parameters = new float[(values.size()+3)];

          if (nBBSampleCount!=0)
          {
              logger(kipl::logging::Logger::LogDebug,"Loading sample images with BB");

              for (size_t i=0; (i<nBBSampleCount && (updateStatus(float(i)/nBBSampleCount,"Calculating ThinPlateSplines interpolation")==false)); i++)
              {
                  samplebb = BBLoader(blackbodysamplename,i+nBBSampleFirstIndex,1,1.0f,fdarkBBdose, fBlackDoseSample);

                  float dosesample; // used for the correct dose roi computation (doseBBroi)
                  float current_dose; // current dose for sample with BBs
                  int index;
                  index = GetnProjwithAngle((angles[2]+(angles[3]-angles[2])/(nBBSampleCount-1)*i));

                  // in the first case compute the mask again (should not be necessary in well acquired datasets) however this requires that the first image (or at least one) of sample image with BB has the same angle of the sample image without BB
                  if (i==0)
                  {
                      if (!bSameMask && m_maskCreationMethod!=ImagingAlgorithms::ReferenceImageCorrection::userDefinedMask)
                      {
                          sample = BBLoader(m_Config.mImageInformation.sSourceFileMask, m_Config.mImageInformation.nFirstFileIndex+index, 1, 1.0f,fdarkBBdose, dosesample);
                          kipl::base::TImage<float,2> mask(sample.dims());
                          mask = 0.0f;
                          temp_parameters = m_corrector.PrepareBlackBodyImagewithSplines(sample,dark,samplebb,mask,values_bb);
                          m_corrector.SetSplineSampleValues(values_bb);
                          mMaskBB = mask; // or memcpy

                      }
                      else
                      {
                          mMaskBB = obmask;
                          values_bb = values;
                          temp_parameters = m_corrector.PrepareBlackBodyImagewithSplinesAndMask(dark,samplebb,mMaskBB, values_bb);
                          m_corrector.SetSplineSampleValues(values_bb);
                      }

                      if (bUseNormROIBB && bUseNormROI)
                      {
      //                     prenormalize interpolation parameters with dose
                          current_dose = fBlackDoseSample;

                          if (bPBvariante)
                          {
                              kipl::base::TImage<float,2> mybb = m_corrector.InterpolateBlackBodyImagewithSplines(temp_parameters, values_bb, doseBBroi);
                              float mydose = computedose(mybb);
                              current_dose = current_dose + ((1.0/tau-1.0)*mydose);
                          }
                          current_dose = current_dose<1 ? 1.0f : current_dose;

                          for(size_t j=0; j<values_bb.size()+3; j++)
                          {
                            temp_parameters[j]/=(current_dose);
                          }
                      }

                        bb_sample_parameters=temp_parameters;
                    //   memcpy(bb_sample_parameters, temp_parameters, sizeof(float)*(values_bb.size()+3));
                  }
                  else
                  {
                      temp_parameters = m_corrector.PrepareBlackBodyImagewithSplinesAndMask(dark,samplebb,mMaskBB,values_bb);
                      current_dose = fBlackDoseSample;

                      if (bPBvariante)
                      {
                          kipl::base::TImage<float,2> mybb = m_corrector.InterpolateBlackBodyImagewithSplines(temp_parameters,values_bb,doseBBroi);
                          float mydose = computedose(mybb);
                          current_dose = current_dose + ((1.0/tau-1.0)*mydose);
                      }

                      current_dose = current_dose<1 ? 1.0f : current_dose;

                      for(size_t j=0; j<(values_bb.size()+3); j++)
                      {
                        temp_parameters[j]/=(current_dose);
                      }

                      std::copy_n(temp_parameters.begin(),values_bb.size()+3,bb_sample_parameters.begin()+i*(values_bb.size()+3))  ;    
                    //   memcpy(bb_sample_parameters+i*(values_bb.size()+3), temp_parameters, sizeof(float)*(values_bb.size()+3));
                  }
              }
          }

        sample_bb_param = bb_sample_parameters;
        // memcpy(sample_bb_param, bb_sample_parameters, sizeof(float)*(values.size()+3)*nBBSampleCount);
        break;
    }

    case (ImagingAlgorithms::ReferenceImageCorrection::Average): {

        // bb_sample_parameters = new float[values.size()+3]; // i am assuming there is the same number of BBs
        // sample_bb_param = new float[values.size()+3];
        // temp_parameters = new float[values.size()+3];
        std::vector<float> mask_parameters(values.size()+3);

        kipl::base::TImage<float,2> samplebb_temp;
        float dose_temp;
        samplebb_temp = BBLoader(blackbodysamplename, nBBSampleFirstIndex,1,1.0f,0.0f, dose_temp);
        samplebb = BBLoader(blackbodysamplename, nBBSampleFirstIndex,nBBSampleCount,1.0f,fdarkBBdose, fBlackDoseSample);

        float dosesample; // used for the correct dose roi computation (doseBBroi)
        float current_dose;

        int index;
        index = GetnProjwithAngle(ffirstAngle);
        sample = BBLoader(m_Config.mImageInformation.sSourceFileMask, m_Config.mImageInformation.nFirstFileIndex+index,
                    1, 1.0f,fdarkBBdose, dosesample); // load the projection with angle closest to the first BB sample data

        if (!bSameMask && m_maskCreationMethod!=ImagingAlgorithms::ReferenceImageCorrection::userDefinedMask)
        {
            kipl::base::TImage<float,2> mask(sample.dims());
            mask = 0.0f;
            mask_parameters = m_corrector.PrepareBlackBodyImagewithSplines(sample,dark, samplebb_temp, mask,values_bb);
            mMaskBB = mask; // or memcpy

        }
        else
        {
            mMaskBB = obmask;
            values_bb = values;
            mask_parameters = m_corrector.PrepareBlackBodyImagewithSplinesAndMask(dark,samplebb, mMaskBB, values_bb);
        }

        m_corrector.SetSplineSampleValues(values_bb);
        temp_parameters = m_corrector.PrepareBlackBodyImagewithSplinesAndMask(dark,samplebb,mMaskBB,values_bb);

        if (bUseNormROIBB && bUseNormROI){
        // prenormalize interpolation parameters with dose

         current_dose = fBlackDoseSample;
          if (bPBvariante)
          {
              kipl::base::TImage<float,2> mybb = m_corrector.InterpolateBlackBodyImagewithSplines(temp_parameters,values_bb, doseBBroi);
              float mydose = computedose(mybb);
              current_dose+= ((1.0/tau-1.0)*mydose);
          }

          current_dose = current_dose<1 ? 1.0f : current_dose;

          for(size_t j=0; j<(values_bb.size()+3); j++)
          {
              temp_parameters[j]/=current_dose;
          }

        }

        sample_bb_param = temp_parameters;
        // memcpy(sample_bb_param, temp_parameters, sizeof(float)*(values.size()+3));
        // delete [] mask_parameters;
        break;
    }

    case (ImagingAlgorithms::ReferenceImageCorrection::OneToOne): {

         bb_sample_parameters.resize((values.size()+3)*nBBSampleCount);
         sample_bb_param.resize((values.size()+3)*nBBSampleCount);
        //  temp_parameters = new float[values.size()+3];
        //  bb_sample_parameters = new float[(values.size()+3)*nBBSampleCount];
        //  sample_bb_param = new float[(values.size()+3)*nBBSampleCount];
        //  temp_parameters = new float[values.size()+3];

         float dosesample;
         float current_dose;

         for (size_t i=0; (i<nBBSampleCount && (updateStatus(float(i)/nBBSampleCount,"Calculating ThinPlateSplines interpolation")==false)); i++)
         {
             samplebb = BBLoader(blackbodysamplename,i+nBBSampleFirstIndex,1,1.0f,fdarkBBdose, fBlackDoseSample);
             sample = BBLoader(m_Config.mImageInformation.sSourceFileMask, m_Config.mImageInformation.nFirstFileIndex+i,
                                 1, 1.0f,fdarkBBdose, dosesample);

             // compute the mask again only for the first sample image, than assume BBs do not move during experiment
             if (i==0)
             {
                 if (!bSameMask && m_maskCreationMethod!=ImagingAlgorithms::ReferenceImageCorrection::userDefinedMask)
                 {
                     kipl::base::TImage<float,2> mask(sample.dims());
                     mask = 0.0f;
                     temp_parameters = m_corrector.PrepareBlackBodyImagewithSplines(sample,dark,samplebb,mask,values_bb);
                     m_corrector.SetSplineSampleValues(values_bb);
                     mMaskBB = mask; // or memcpy

                 }
                 else
                 {
                     mMaskBB = obmask;
                     values_bb = values;
                     temp_parameters = m_corrector.PrepareBlackBodyImagewithSplinesAndMask(dark,samplebb,mMaskBB, values_bb);
                     m_corrector.SetSplineSampleValues(values_bb);
                 }


                 if (bUseNormROIBB && bUseNormROI)
                 {
                     // prenormalize interpolation parameters with dose
                     current_dose= fBlackDoseSample;
                     if (bPBvariante)
                     {
                         kipl::base::TImage<float,2> mybb = m_corrector.InterpolateBlackBodyImagewithSplines(temp_parameters,values_bb,doseBBroi);
                         float mydose = computedose(mybb);
                         current_dose += ((1.0/tau-1.0)*mydose);
                     }

                     current_dose = current_dose<1 ? 1.0f : current_dose;

                     for(size_t j=0; j<(values.size()+3); j++)
                     {
                         temp_parameters[j]/=current_dose;
                     }
                 }

                bb_sample_parameters = temp_parameters;
                //  memcpy(bb_sample_parameters, temp_parameters, sizeof(float)*(values.size()+3));
             }
             else
             {
                 temp_parameters = m_corrector.PrepareBlackBodyImagewithSplinesAndMask(dark,samplebb,mMaskBB,values_bb);
                 current_dose= fBlackDoseSample;

                 if (bPBvariante)
                 {
                     kipl::base::TImage<float,2> mybb = m_corrector.InterpolateBlackBodyImagewithSplines(temp_parameters, values_bb, doseBBroi);
                     float mydose = computedose(mybb);
                     current_dose+= ((1.0/tau-1.0)*mydose);
                 }

                 current_dose = current_dose<1 ? 1.0f : current_dose;

                 for(size_t j=0; j<(values.size()+3); j++)
                 {
                      temp_parameters[j]/=current_dose;
                 }
                 std::copy_n(temp_parameters.begin(),values.size()+3,bb_sample_parameters.begin()+i*(values.size()+3));
                //  memcpy(bb_sample_parameters+i*(values.size()+3), temp_parameters, sizeof(float)*(values.size()+3));
             }
             float dose = dosesample/(current_dose*tau);
         }

        sample_bb_param = bb_sample_parameters;
        // memcpy(sample_bb_param, bb_sample_parameters, sizeof(float)*(values.size()+3)*nBBSampleCount);
        break;
      }

    case (ImagingAlgorithms::ReferenceImageCorrection::noBB): {
        throw ImagingException("trying to switch to case ImagingAlgorithms::ReferenceImageCorrection::noBB in PrepareBBdata",__FILE__,__LINE__);
        break;
    }
    case (ImagingAlgorithms::ReferenceImageCorrection::ExternalBB) : {
      throw ImagingException("trying to switch to case ImagingAlgorithms::ReferenceImageCorrection::ExternalBB in PrepareBBdata",__FILE__,__LINE__);
    }

    default: throw ImagingException("trying to switch to unknown BBOption in PrepareBBdata",__FILE__,__LINE__);

    }

    return values.size();

}

int BBLogNorm::GetnProjwithAngle(float angle)
{

    // range of projection angles
    double nProj=(static_cast<double>(m_Config.mImageInformation.nLastFileIndex)-static_cast<double>(m_Config.mImageInformation.nFirstFileIndex)+1.0);

    int index =0;
    float curr_angle;

    for (size_t i=0; i<nProj; i++)
    {
        curr_angle = fScanArc[0]+(fScanArc[1]-fScanArc[0])/(nProj-1)*i;

        if (curr_angle<=angle+0.5f & curr_angle>=angle-0.5f)
        {
            break;
        }
        else index++;
    }

    return index;
}

float BBLogNorm::GetInterpolationError(kipl::base::TImage<float,2> &mask)
{

    if (flatname.empty() && nOBCount!=0)
        throw ImagingException("The flat field image mask is empty",__FILE__,__LINE__);
    if (darkname.empty() && nDCCount!=0)
        throw ImagingException("The dark current image mask is empty",__FILE__,__LINE__);

    if (blackbodyname.empty() && nBBCount!=0)
        throw ImagingException("The open beam image with BBs image mask is empty",__FILE__,__LINE__);

    kipl::base::TImage<float,2> flat, dark, bb;

    std::string flatmask=flatname;
    std::string darkmask=darkname;

    float darkdose = 0.0f;
    float flatdose = 1.0f;
    float blackdose = 1.0f;

    // reload the OB and DC into the BBroi and doseBBroi
    dark = BBLoader(darkmask,nDCFirstIndex,nDCCount,0.0f,0.0f,darkdose);
    flat = BBLoader(flatmask,nOBFirstIndex,nOBCount,1.0f,0.0f,flatdose);

    // load OB image with BBs

    // float *bb_ob_param = new float[6];
    std::vector<float> bb_ob_param;
    bb = BBLoader(blackbodyname,nBBFirstIndex,nBBCount,1.0f,0.0f,blackdose);
    std::vector<int> diffroi(BBroi.begin(),BBroi.end()); // it is now just the BBroi position, makes more sense

    m_corrector.SetRadius(radius);
    m_corrector.SetMinArea(min_area);
    m_corrector.SetInterpolationOrderX(m_xInterpOrder);
    m_corrector.SetInterpolationOrderY(m_yInterpOrder);
    m_corrector.setDiffRoi(diffroi); // left to compute the interpolation parameters in the abssolute image coordinates

    std::stringstream msg;
    msg.str(""); msg<<"Min area set to  "<<min_area;
    logger(kipl::logging::Logger::LogDebug,msg.str());

    float error;
    kipl::base::TImage<float,2> obmask(bb.dims());
    try {
        bb_ob_param = m_corrector.PrepareBlackBodyImage(flat,dark,bb, obmask, error);
    }
    catch (ModuleException &e) {
        msg.str(""); msg<<"Failed to compute bb_ob_parameters. Try to change thresholding method or value. " << e.what();
        logger(kipl::logging::Logger::LogDebug,msg.str());
        throw ModuleException("Failed to compute bb_ob_parameters. Try to change thresholding method or value. ", __FILE__,__LINE__);
    }
    catch(ImagingException &e){
        msg.str(""); msg<<"Failed to compute bb_ob_parameters. Try to change thresholding method or value. " << e.what();
        logger(kipl::logging::Logger::LogDebug,msg.str());
        throw ImagingException("Failed to compute bb_ob_parameters. Try to change thresholding method or value. ", __FILE__,__LINE__);
    }
    catch(kipl::base::KiplException &e){
        msg.str(""); msg<<"Failed to compute bb_ob_parameters. Try to change thresholding method or value. " << e.what();
        logger(kipl::logging::Logger::LogDebug,msg.str());
        throw kipl::base::KiplException("Failed to compute bb_ob_parameters. Try to change thresholding method or value. ", __FILE__,__LINE__);
    }

    mask = obmask;
    // delete [] bb_ob_param;
    return error;
}


float BBLogNorm::GetInterpolationErrorFromMask(kipl::base::TImage<float, 2> &mask)
{

    if (flatname.empty() && nOBCount!=0)
        throw ImagingException("The flat field image mask is empty",__FILE__,__LINE__);
    if (darkname.empty() && nDCCount!=0)
        throw ImagingException("The dark current image mask is empty",__FILE__,__LINE__);

    if (blackbodyname.empty() && nBBCount!=0)
        throw ImagingException("The open beam image with BBs image mask is empty",__FILE__,__LINE__);

    kipl::base::TImage<float,2> flat, dark, bb;

    std::string flatmask=flatname;
    std::string darkmask=darkname;

    float darkdose = 0.0f;
    float blackdose = 1.0f;

    // reload DC into the BBroi and doseBBroi
    dark = BBLoader(darkmask,nDCFirstIndex,nDCCount,0.0f,0.0f,darkdose);

    // load OB image with BBs
    // float *bb_ob_param = new float[6];
    std::vector<float> bb_ob_param;
    bb = BBLoader(blackbodyname,nBBFirstIndex,nBBCount,1.0f,0.0f,blackdose);
    std::vector<int> diffroi(4,0); // diffroi is not relevant for external mask option

    m_corrector.SetRadius(radius);
    m_corrector.SetMinArea(min_area);
    m_corrector.SetInterpolationOrderX(m_xInterpOrder);
    m_corrector.SetInterpolationOrderY(m_yInterpOrder);
    m_corrector.setDiffRoi(diffroi); // left to compute the interpolation parameters in the absolute image coordinates

    std::stringstream msg;
    msg.str(""); msg<<"Min area set to  "<<min_area;
    logger(kipl::logging::Logger::LogDebug,msg.str());

    float error;
    try {
        bb_ob_param = m_corrector.PrepareBlackBodyImagewithMask(dark, bb, mask, error);
    }
    catch (ModuleException &e) {
        msg.str(""); msg<<"Failed to compute bb_ob_parameters. Try to change thresholding method or value. " << e.what();
        logger(kipl::logging::Logger::LogDebug,msg.str());
        throw ModuleException("Failed to compute bb_ob_parameters. Try to change thresholding method or value. ", __FILE__,__LINE__);
    }
    catch(ImagingException &e){
        msg.str(""); msg<<"Failed to compute bb_ob_parameters. Try to change thresholding method or value. " << e.what();
        logger(kipl::logging::Logger::LogDebug,msg.str());
        throw ImagingException("Failed to compute bb_ob_parameters. Try to change thresholding method or value. ", __FILE__,__LINE__);
    }
    catch(kipl::base::KiplException &e){
        msg.str(""); msg<<"Failed to compute bb_ob_parameters. Try to change thresholding method or value. " << e.what();
        logger(kipl::logging::Logger::LogDebug,msg.str());
        throw kipl::base::KiplException("Failed to compute bb_ob_parameters. Try to change thresholding method or value. ", __FILE__,__LINE__);
    }

    // delete [] bb_ob_param;
    return error;

}

kipl::base::TImage<float,2> BBLogNorm::GetMaskImage(){
    return mMaskBB;
}

float BBLogNorm::computedose(kipl::base::TImage<float,2>&img){

    float *pImg=img.GetDataPtr();
    std::vector<float> means(img.Size(1),0.0f);
    // memset(means,0,img.Size(1)*sizeof(float));

    for (size_t y=0; y<img.Size(1); y++) 
    {
        pImg=img.GetLinePtr(y);

        for (size_t x=0; x<img.Size(0); x++) 
        {
            means[y]+=pImg[x];
        }
        means[y]=means[y]/static_cast<float>(img.Size(0));
    }

    float dose=0.0f;
    kipl::math::median(means,&dose);
    // delete [] means;
    return dose;


}

int BBLogNorm::ProcessCore(kipl::base::TImage<float,2> & img, std::map<std::string, std::string> & coeff)
{

    return 0;
}

int BBLogNorm::ProcessCore(kipl::base::TImage<float,3> & img, std::map<std::string, std::string> & coeff) {

    std::stringstream msg;
    msg.str(""); msg<<"ProcessCore";
    logger(kipl::logging::Logger::LogDebug,msg.str());

    if (bUseBB && nBBCount!=0 && nBBSampleCount!=0)
    {
            PrepareBBData();
    }

    SetROI(m_Config.mImageInformation.nROI);

    int nDose=img.Size(2);
    float *doselist=new float[nDose];

    ImageReader reader;

    if (bUseNormROI==true)
    {
//        doselist=new float[nDose];
        std:: string filename,ext;
        std::string fmask = m_Config.mImageInformation.sSourceFileMask;
        int firstIndex = m_Config.mImageInformation.nFirstFileIndex;


        for (int i=0; i<nDose; i++)
        {
            kipl::strings::filenames::MakeFileName(fmask,firstIndex+i,filename,ext,'#','0');
            msg.str(""); msg<<"Reading dose for: " << filename;
            logger(kipl::logging::Logger::LogDebug,msg.str());

            float dose = reader.projectionDose(  filename,dose_roi,
                                                  m_Config.mImageInformation.eFlip,
                                                  m_Config.mImageInformation.eRotate,
                                                  1.0f);
            doselist[i] = dose-fDarkDose;
        }
    }

    m_corrector.Process(img,doselist);

    if (doselist!=nullptr)
        delete [] doselist;

    return 0;
}

void BBLogNorm::SetReferenceImages(kipl::base::TImage<float,2> dark, kipl::base::TImage<float,2> flat)
{
    mdark = dark;
    mdark.Clone();

    mflat = flat;
    mflat.Clone();
}


kipl::base::TImage<float,2> BBLogNorm::ReferenceLoader(std::string fname,
                                                      int firstIndex, int N,
                                                      const std::vector<size_t> &roi,
                                                      float initialDose,
                                                      float doseBias,
                                                      KiplProcessConfig &config,
                                                      float &dose)
{
    std::ostringstream msg;

    kipl::base::TImage<float,2> img, refimg;

    float tmpdose = 0.0f;

    if (fname.empty() && N!=0)
        throw ImagingException("The reference image file name mask is empty",__FILE__,__LINE__);

    std::string fmask=fname;
    std::string filename,ext;
    ImageReader reader;
    size_t found;

    dose = initialDose; // A precaution in case no dose is calculated

    if (N!=0)
    {
        msg.str(""); msg<<"Loading "<<N<<" reference images";
        logger(kipl::logging::Logger::LogDebug,msg.str());

        kipl::strings::filenames::MakeFileName(fmask,firstIndex,filename,ext,'#','0');

        if ( !roi.empty() )
        {
            msg.str(""); msg<<"Reading images with name: " << filename << " and roi: "<< roi[0] <<
                              " " << roi[1] << " " << roi[2] << " " << roi[3];
            logger(kipl::logging::Logger::LogDebug,msg.str());
        }

        img = reader.Read(filename,
                          m_Config.mImageInformation.eFlip,
                          m_Config.mImageInformation.eRotate,
                          1.0f,
                          roi);

        msg.str(""); msg<<"Reading dose";
        logger(kipl::logging::Logger::LogDebug,msg.str());
        tmpdose=bUseNormROI ? reader.projectionDose(filename,
                                                       nOriginalNormRegion,
                                                       m_Config.mImageInformation.eFlip,
                                                       m_Config.mImageInformation.eRotate,
                                                      1.0f) : initialDose;
        tmpdose   = tmpdose - doseBias;
        dose      = tmpdose;


        std::vector<size_t> obdims = { img.Size(0),
                                       img.Size(1),
                                       static_cast<size_t>(N)};

        kipl::base::TImage<float,3> img3D(obdims);
        memcpy(img3D.GetLinePtr(0,0),img.GetDataPtr(),img.Size()*sizeof(float));

        for (int i=1; i<N; ++i)
        {
            kipl::strings::filenames::MakeFileName(fmask,i+firstIndex,filename,ext,'#','0');

            img=reader.Read(filename,
                            m_Config.mImageInformation.eFlip,
                            m_Config.mImageInformation.eRotate,
                           1.0f,
                    roi);
            memcpy(img3D.GetLinePtr(0,i),img.GetDataPtr(),img.Size()*sizeof(float));

            tmpdose = bUseNormROI ? reader.projectionDose(filename,
                                                          nOriginalNormRegion,
                                                             m_Config.mImageInformation.eFlip,
                                                             m_Config.mImageInformation.eRotate,
                                                            1.0f) : initialDose;
            tmpdose   = tmpdose - doseBias;
            dose     += tmpdose;
        }


        dose/=static_cast<float>(N);
        msg.str(""); msg<<"Dose="<<dose;
        logger(logger.LogDebug,msg.str());

        float *tempdata=new float[N];
        refimg.resize(img.dims());

        ImagingAlgorithms::AverageImage avg;

        refimg = avg(img3D,m_ReferenceAverageMethod);

        delete [] tempdata;
    }
    else
        logger(kipl::logging::Logger::LogWarning,"Reference image count is zero");

    msg.str(""); msg<<"Loaded reference image (dose="<<dose<<")";
    logger(kipl::logging::Logger::LogDebug,msg.str());
    return refimg;
}


kipl::base::TImage<float,2> BBLogNorm::BBLoader(std::string fname,
                                                      int firstIndex, int N,
                                                      float initialDose,
                                                      float doseBias,
                                                      float &dose)
{
    std::ostringstream msg;
    kipl::base::TImage<float,2> img, refimg;
    float tmpdose = 0.0f;

    if (fname.empty() && N!=0)
        throw ImagingException("The reference image file name mask is empty",__FILE__,__LINE__);

    std::string fmask=fname;
    std::string filename,ext;
    ImageReader reader;
    size_t found;
    dose = initialDose; // A precaution in case no dose is calculated

    if (N!=0)
    {
        msg.str(""); msg<<"Loading "<<N<<" reference images";
        logger(kipl::logging::Logger::LogDebug,msg.str());
        found = fmask.find("hdf");
        kipl::strings::filenames::MakeFileName(fmask,firstIndex,filename,ext,'#','0');

        if (m_maskCreationMethod!=ImagingAlgorithms::ReferenceImageCorrection::userDefinedMask)
        {
            img = reader.Read(filename,
                              m_Config.mImageInformation.eFlip,
                              m_Config.mImageInformation.eRotate,
                              1.0f,
                              BBroi);
        }
        else
        {
            img = reader.Read(filename,
                              m_Config.mImageInformation.eFlip,
                              m_Config.mImageInformation.eRotate,
                              1.0f,
                              {});

        }


        tmpdose=bUseNormROIBB ? reader.projectionDose(filename,
                                                      doseBBroi,
                                                         m_Config.mImageInformation.eFlip,
                                                         m_Config.mImageInformation.eRotate,
                                                        1.0f) : initialDose;

        tmpdose   = tmpdose - doseBias;
        dose      = tmpdose;
        std::vector<size_t> obdims = { img.Size(0),
                                       img.Size(1),
                                       static_cast<size_t>(N)};

        kipl::base::TImage<float,3> img3D(obdims);
        memcpy(img3D.GetLinePtr(0,0),img.GetDataPtr(),img.Size()*sizeof(float));

        for (int i=1; i<N; ++i)
        {
            kipl::strings::filenames::MakeFileName(fmask,i+firstIndex,filename,ext,'#','0');

            if (m_maskCreationMethod!=ImagingAlgorithms::ReferenceImageCorrection::userDefinedMask)
            {
                img = reader.Read(filename,
                                  m_Config.mImageInformation.eFlip,
                                  m_Config.mImageInformation.eRotate,
                                  1.0f,
                                  BBroi);
            }
            else
            {
                img = reader.Read(filename,
                                  m_Config.mImageInformation.eFlip,
                                  m_Config.mImageInformation.eRotate,
                                  1.0f,
                                  {});

            }
            memcpy(img3D.GetLinePtr(0,i),img.GetDataPtr(),img.Size()*sizeof(float));
            tmpdose = bUseNormROIBB ? reader.projectionDose(filename,
                                                            doseBBroi,
                                                               m_Config.mImageInformation.eFlip,
                                                               m_Config.mImageInformation.eRotate,
                                                              1.0f) : initialDose;
            tmpdose   = tmpdose - doseBias;
            dose     += tmpdose;
        }

        dose/=static_cast<float>(N);
        msg.str(""); msg<<"Dose="<<dose;
        logger(logger.LogDebug,msg.str());
        refimg.resize(img.dims());
        ImagingAlgorithms::AverageImage avg;
        refimg = avg(img3D,m_ReferenceAverageMethod);
    }
    else
        logger(kipl::logging::Logger::LogWarning,"Reference image count is zero");

    msg.str(""); msg<<"Loaded reference image (dose="<<dose<<")";
    logger(kipl::logging::Logger::LogDebug,msg.str());
    return refimg;
}

float BBLogNorm::DoseBBLoader(std::string fname,
                             int firstIndex,
                             float initialDose,
                             float doseBias)
{
    std::string fmask=fname;
    float tmpdose = 0.0f;
    float dose;

    std::string filename,ext;
    ImageReader reader;
    size_t found;

    found=fname.find("hdf");

    kipl::strings::filenames::MakeFileName(fmask,firstIndex,filename,ext,'#','0');
    tmpdose=bUseNormROIBB ? reader.projectionDose(filename,
                                                  doseBBroi,
                                                     m_Config.mImageInformation.eFlip,
                                                     m_Config.mImageInformation.eRotate,
                                                    1.0f) : initialDose;
    tmpdose   = tmpdose - doseBias;
    dose      = tmpdose;
    return dose;
}

kipl::base::TImage <float,2> BBLogNorm::BBExternalLoader(std::string fname,
                                                         const std::vector<size_t> &roi,
                                                         float &dose)
{
    kipl::base::TImage<float,2> img;

    if (fname.empty())
        throw ImagingException("The reference image file name mask is empty",__FILE__,__LINE__);

    ImageReader reader;

    img = reader.Read(fname,
                      m_Config.mImageInformation.eFlip,
                      m_Config.mImageInformation.eRotate,
                     1.0f,
                     roi);

    dose = bUseNormROI ? reader.projectionDose(fname,
                                               nOriginalNormRegion,
                                                  m_Config.mImageInformation.eFlip,
                                                  m_Config.mImageInformation.eRotate,
                                                 1.0f) : 0.0f;
    return img;

}

kipl::base::TImage <float,3> BBLogNorm::BBExternalLoader(std::string fname,
                                                         int N,
                                                         const std::vector<size_t> &roi,
                                                         int firstIndex,
                                                         std::vector<float> &doselist){


    kipl::base::TImage <float, 2> tempimg;
    kipl::base::TImage<float, 3> img;



    if (fname.empty() && N!=0)
        throw ImagingException("The reference image file name mask is empty",__FILE__,__LINE__);

    std::vector<float> mylist(N,0.0f);

    std::string fmask=fname;

    std::string filename,ext;
    ImageReader reader;

    if (N!=0)
    {
        for (int i=0; i<N; ++i)
        {
            kipl::strings::filenames::MakeFileName(fmask,i+firstIndex,filename,ext,'#','0');
            tempimg=reader.Read(filename,
                                m_Config.mImageInformation.eFlip,
                                m_Config.mImageInformation.eRotate,
                               1.0f,
                                roi);
            if (i==0)
            {
                std::vector<size_t> dims = { tempimg.Size(0),
                                             tempimg.Size(1),
                                             static_cast<size_t>(N)};
                img.resize(dims);
            }

            mylist[i] = bUseNormROI ? reader.projectionDose(filename,nOriginalNormRegion,
                                                               m_Config.mImageInformation.eFlip,
                                                               m_Config.mImageInformation.eRotate,
                                                              1.0f) : 0.0f;
            memcpy(img.GetLinePtr(0,i),tempimg.GetDataPtr(),tempimg.Size()*sizeof(float));
        }

        doselist = mylist;
    }

    return img;
}


kipl::base::TImage<float,2> BBLogNorm::LoadUserDefinedMask()
{
    kipl::base::TImage<float,2> mask;
    std::stringstream msg;
    // test on existance of the file name
    if ( !std::filesystem::exists(fs::path(blackbodyexternalmaskname)) )
    // if (!QFile::exists(QString::fromStdString(blackbodyexternalmaskname)) )
    {
        msg.str(""); msg<<"user provided filename for mask does not exist. Please check your inputs. ";
        logger(kipl::logging::Logger::LogDebug,msg.str());
        throw ModuleException("user provided filename for mask does not exist. Please check your inputs.",__FILE__, __LINE__);
    }
    else
    {
        ImageReader reader;
        mask = reader.Read(blackbodyexternalmaskname,
                              m_Config.mImageInformation.eFlip,
                              m_Config.mImageInformation.eRotate,
                              1.0f,
                              {});

        // normalize and check for binary status of the image

        float max = *std::max_element(mask.GetLinePtr(0), mask.GetLinePtr(0)+mask.Size());

        // TODO Revise for more efficiency - only one loop needed

        for (size_t i=0; i<mask.Size(); ++i)
        {
            mask[i] /= max;
        }

        bool non_binary = false;

        for (size_t i=0; i<mask.Size(); ++i)
        {
            if (mask[i]!=0.0 && mask[i]!=1.0)
            {
                non_binary=true;
            }
        }

        if (non_binary)
        {
            msg.str(""); msg<<"User provided mask is non binary. Please check your inputs. ";
            logger(kipl::logging::Logger::LogDebug,msg.str());
            throw ModuleException("User provided mask is non binary. Please check your inputs.",__FILE__, __LINE__);
        }


    }

    return mask;
}



