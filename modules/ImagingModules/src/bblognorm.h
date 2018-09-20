#ifndef BBLOGNORM_H
#define BBLOGNORM_H

//#include "StdPreprocModules_global.h"

#include "ImagingModules_global.h"
#include "imagingmodules.h"
#include <logging/logger.h>
#include <base/timage.h>
#include <math/LUTCollection.h>

#include <ReferenceImageCorrection.h>
#include <KiplProcessModuleBase.h>

#include <KiplProcessConfig.h>
#include <interactors/interactionbase.h>

//#include <PreprocModuleBase.h>
//#include <ReconConfig.h>

//#include "PreprocEnums.h"
#include <averageimage.h>

//#include "../include/NormPlugins.h"

class IMAGINGMODULESSHARED_EXPORT BBLogNorm: public KiplProcessModuleBase {
public:
    BBLogNorm(kipl::interactors::InteractionBase *interactor=nullptr);
    virtual ~BBLogNorm();

//    virtual int Configure(std::map<std::string, std::string> parameters); /// Configure all parameters and calls PrepareBBData
    virtual int Configure(KiplProcessConfig config, std::map<std::string, std::string> parameters); /// Configure all parameters and calls SetRoi and PrepareBBData
    virtual int ConfigureDLG(KiplProcessConfig config, std::map<std::string, std::string> parameters); /// Configure all parameters and does not call PrepareBBData
    virtual std::map<std::string, std::string> GetParameters();
    virtual void LoadReferenceImages(size_t *roi); /// load all images that are needed for referencing in the current roi
    virtual bool SetROI(size_t *roi); /// set the current roi to be processed and calls LoadReferenceImages

    virtual int ProcessCore(kipl::base::TImage<float,2> & img, std::map<std::string, std::string> & coeff);
    virtual int ProcessCore(kipl::base::TImage<float,3> & img, std::map<std::string, std::string> & coeff);
    virtual void SetReferenceImages(kipl::base::TImage<float,2> dark, kipl::base::TImage<float,2> flat); /// set references images
    virtual float GetInterpolationError(kipl::base::TImage<float,2> &mask); /// computes and returns interpolation error and mask on OB image with BBs
    virtual kipl::base::TImage<float, 2> GetMaskImage();
    virtual void PrepareBBData(); /// read all data (entire projection) that I need and prepare them for the BB correction, it is now called in LoadReferenceImages
    virtual void LoadExternalBBData(size_t *roi); /// load BB images pre-processed elsewhere

protected:

    KiplProcessConfig m_Config;
    std::string path; /// path, maybe not used
    std::string flatname; /// name mask for OB image
    std::string darkname; /// name mask for DC image
    std::string blackbodyname; /// name mask for OB image with BBs
    std::string blackbodysamplename; /// name mask for sample image with BBs

    std::string blackbodyexternalname; /// name of the externally computed background for the OB
    std::string blackbodysampleexternalname; /// names of the externally computed backgrounds for the sample

    std::string pathBG; /// path for saving BGs
    std::string flatname_BG; /// filename for saving the open beam BG
    std::string filemask_BG; /// filemask for saving the computed sample BGs

    size_t nBBextCount; /// number of preprocessed BB images;
    size_t nBBextFirstIndex; /// first index in filneame for preprocessed BB images

    size_t nOBCount; /// number of OB images
    size_t nOBFirstIndex; /// first index in filename for OB images
    size_t nDCCount; /// number of DC images
    size_t nDCFirstIndex; /// first index in filename for DC images
    size_t nBBCount; /// number of open beam images with BB
    size_t nBBFirstIndex; /// first index in filename for OB images with BB
    size_t nBBSampleCount; /// number of sample images with BB
    size_t nBBSampleFirstIndex; /// first index in filename for sample images with BB

    float fFlatDose; /// dose value in open beam images in dose roi
    float fDarkDose; /// dose value in dark current images in dose roi
    float fBlackDose; /// dose value in black body images in BB dose roi
    float fBlackDoseSample; /// dose values in black body images with sample in BB dose roi
    float fdarkBBdose; /// dose value in dark current images within BB dose roi
    float fFlatBBdose; /// dose value in open beam image within BB dose roi

    float tau; /// mean pattern transmission, default 0.97
    float thresh; /// manual threshold

    bool bUseNormROI; /// boolean value on the use of the norm roi
    bool bUseLUT; /// boolean value on the use of LUT (not used)
    bool bUseWeightedMean;
    bool bUseBB; /// boolean value on the use of BBs, to be set when calling PrepareBBData
    bool bUseExternalBB; /// boolean value on the use of externally produced BBs
    bool bUseNormROIBB; /// boolean value on the use of the norm roi on BBs
    bool bSameMask; /// boolean value on the use of the same mask computed for OB images with BB also for sample image with BB, in case of false the mask is recomputed on the first available image with BB. This implies that exists an image with the sample and without BB and the sample is in the exact same position
    bool bUseManualThresh; /// boolean value on the use of a manual threshold instead of Otsu
    bool bPBvariante; /// boolean value to enable the full formulation of the correction by PB. It is actually the only one used
    bool bSaveBG; /// boolean value to enable the option of saving the computed BGs

    size_t nNormRegion[4];
    size_t nOriginalNormRegion[4];
    size_t BBroi[4]; /// region of interest to be set for BB segmentation
    size_t doseBBroi[4]; /// region of interest for dose computation in BB images
    size_t dose_roi[4]; /// region of interest for dose coputation in projection images


    size_t radius; /// radius used to select circular region within the BBs to be used for interpolation
    size_t min_area; /// minimal area to be used for BB segmentation: if smaller, segmented BB is considered as noise and not considered for background computation
    float ferror; /// interpolation error, computed on the open beam with BBs image
    float ffirstAngle; /// first angle for BB sample image, used for BB interpolation option
    float flastAngle; /// last angle for BB sample image, used for BB interpolation option
    float fScanArc[2]; /// first and last angle of projections (to be used for tomo)

    float *ob_bb_param;
    float *sample_bb_param;

    int GetnProjwithAngle(float angle); /// compute the index of projection data at a given angle


    kipl::base::TImage<float,2> mMaskBB;
    kipl::base::TImage<float,2> mdark;
    kipl::base::TImage<float,2 > mflat;

    virtual kipl::base::TImage<float,2> ReferenceLoader(std::string fname,
                                                        int firstIndex,
                                                        int N,
                                                        size_t *roi,
                                                        float initialDose,
                                                        float doseBias,
                                                        KiplProcessConfig &config,
                                                        float &dose); /// Loader function and dose computation for standard reference images (OB and DC)


    virtual kipl::base::TImage<float,2> BBLoader(std::string fname,
                                                        int firstIndex,
                                                        int N,
                                                        float initialDose,
                                                        float doseBias,
                                                        float &dose); /// Loader function and dose computation for BB reference images (OB with BB and sample with BB)

    virtual float DoseBBLoader(std::string fname,
                                 int firstIndex,
                                 float initialDose,
                                 float doseBias); /// Loader function that only compute Dose in the BB dose roi , whitout loading the entire image

    virtual kipl::base::TImage<float,2> BBExternalLoader(std::string fname,
                                                         size_t *roi,
                                                         float &dose); /// Loader function for externally created BB, open beam case (only 1 image)
    virtual kipl::base::TImage<float,3> BBExternalLoader(std::string fname,
                                                         int N,
                                                         size_t *roi,
                                                       int firstIndex,
                                                       float *doselist); /// Loader function for externally created BB, sample image case (nProj images with filemask)


    float computedose(kipl::base::TImage<float,2>&img);

    void PreparePolynomialInterpolationParameters(); /// get the interpolation parameters for the polynomial case
    int PrepareSplinesInterpolationParameters(); /// get the interpolation parameters for the thin plates spline case

private:
    int m_nWindow; /// apparentely not used
//    kipl::interactors::InteractionBase *m_Interactor;
    ImagingAlgorithms::AverageImage::eAverageMethod m_ReferenceAverageMethod; /// method chosen for averaging Referencing images
    ImagingAlgorithms::ReferenceImageCorrection::eReferenceMethod m_ReferenceMethod;/// method chosen for Referencing (BBLogNorm or LogNorm, only BBLogNorm is implemented at the moment)
    ImagingAlgorithms::ReferenceImageCorrection::eBBOptions m_BBOptions; /// options for BB image reference correction (Interpolate, Average, OneToOne)
    ImagingAlgorithms::ReferenceImageCorrection m_corrector; /// instance of ReferenceImageCorrection
    ImagingAlgorithms::ReferenceImageCorrection::eInterpOrderX m_xInterpOrder; /// order chosen for interpolation along the X direction
    ImagingAlgorithms::ReferenceImageCorrection::eInterpOrderY m_yInterpOrder; /// order chosen for interpolation along the Y direction
    ImagingAlgorithms::ReferenceImageCorrection::eInterpMethod m_InterpMethod; /// interpolation method

};



#endif // BBLOGNORM_H
