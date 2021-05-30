//<LICENSE>

#ifndef BBLOGNORMDLG_H
#define BBLOGNORMDLG_H
#include "imagingmodulesgui_global.h"

#include <string>
#include <vector>
#include <QDialog>

#include <ConfiguratorDialogBase.h>
#include <ConfigBase.h>
#include <KiplProcessConfig.h>
#include <imageviewerwidget.h>
#include <plotter.h>
#include <base/timage.h>

#include <bblognorm.h>

//#include <windowsx.h>
//#include <Windows.h>
//#include <winnt.h

namespace Ui {
class BBLogNormDlg;
}

class BBLogNormDlg : public ConfiguratorDialogBase
{
    Q_OBJECT

public:
    explicit BBLogNormDlg(QWidget *parent = nullptr);
     ~BBLogNormDlg();
    virtual int exec(ConfigBase * config, std::map<std::string, std::string> &parameters, kipl::base::TImage<float,3> & img);

private slots:
    void on_button_OBBBpath_clicked();
    void on_buttonPreviewOBBB_clicked();
    void on_button_sampleBBpath_clicked();
    void on_buttonPreviewsampleBB_clicked();
    void on_errorButton_clicked();
    void on_combo_BBoptions_activated(const QString &arg1);
    void on_button_OB_BB_ext_clicked();
    void on_button_BBexternal_path_clicked();
    void on_combo_InterpolationMethod_activated(const QString &arg1);
    void on_checkBox_thresh_clicked(bool checked);
    void on_spinThresh_valueChanged(double arg1);
    void on_checkBox_thresh_stateChanged(int arg1);
    void on_pushButton_filenameOBBB_clicked();
    void on_pushButton_filenameBB_clicked();
    void on_pushButton_OB_clicked();
    void on_pushButton_DC_clicked();
    void on_pushButton_browseOB_clicked();
    void on_pushButton_browseDC_clicked();
    void on_checkBox_saveBG_clicked(bool checked);
    void on_pushButton_browseDest_clicked();
    void on_check_singleext_clicked(bool checked);
    void on_check_singleext_stateChanged(int arg1);
    void on_pushButton_ext_back_clicked();
    void on_pushButton_ext_sample_back_clicked();
    void on_comboBox_maskmethod_currentIndexChanged(int index);
    void on_pushButton_browsemask_clicked();

private:
    virtual void ApplyParameters();
    virtual void UpdateDialog();
    virtual void UpdateParameters();
    virtual void UpdateParameterList(std::map<std::string, std::string> &parameters);

    void BrowseOBBBPath();
    void UpdateBBROI();
    void BrowseSampleBBPath();
    void UpdateDoseROI();
    void BrowseOBPath();
    void BrowseDCPath();

    Ui::BBLogNormDlg *ui;
    KiplProcessConfig *m_Config;

    std::string path; /// path, maybe not used
    std::string flatname; /// name mask for OB image
    std::string darkname; /// name mask for DC imagef

    size_t nOBCount; /// number of OB images
    size_t nOBFirstIndex; /// first index in filename for OB images
    size_t nDCCount; /// number of DC images
    size_t nDCFirstIndex; /// first index in filename for DC images

    size_t nBBCount; /// number of open beam images with BB
    size_t nBBFirstIndex; /// first index in filename for OB images with BB
    std::string blackbodyname;

    size_t nBBSampleCount; /// number of sample images with BB
    size_t nBBSampleFirstIndex; /// first index in filename for sample images with BB
    std::string blackbodysamplename;
    std::string blackbodyexternalmaskname; /// name of user provided mask

    std::vector<size_t> BBroi;
    std::vector<size_t> doseBBroi;
    std::vector<size_t> dose_roi;
    size_t radius;
    size_t min_area;

    float ffirstAngle;
    float flastAngle;
    std::vector<float> fScanArc; /// first and last angle of projections (to be used for tomo)

    std::string blackbodyexternalname;
    std::string blackbodysampleexternalname;

    std::string pathBG; /// path for saving BGs
    std::string flatname_BG; /// filename for saving the open beam BG
    std::string filemask_BG; /// filemask for saving the computed sample BGs

    size_t nBBextCount; /// number of preprocessed BB images;
    size_t nBBextFirstIndex; /// first index in filneame for preprocessed BB images

    float tau; /// mean pattern transmission, default 0.97
    float thresh; /// manual threshold
    bool bPBvariante; /// boolean value on the use of PB formula (default now)
    bool bUseNormROI; /// boolean value on the use of the norm roi
    bool bUseNormROIBB; /// boolean value on the use of the norm roi on BBs
    bool bSameMask; /// boolean value on the use of the same mask for all images with BBs
//    bool bUseManualThresh; /// boolean value on the use of a manual threshold instead of Otsu
    bool bSaveBG; /// triggers the saving of computed BGs
    bool bExtSingleFile; /// boolean value on the use of single external file for sample background
//    bool bUseBB; /// boolean value on the use of BBs, to be set when calling PrepareBBData
//    bool bUseExternalBB; /// boolean value on the use of externally produced BBs
    ImagingAlgorithms::AverageImage::eAverageMethod m_ReferenceAverageMethod;
    ImagingAlgorithms::ReferenceImageCorrection::eReferenceMethod m_ReferenceMethod;
    ImagingAlgorithms::ReferenceImageCorrection::eBBOptions m_BBOptions;
    ImagingAlgorithms::ReferenceImageCorrection::eInterpOrderX m_xInterpOrder;
    ImagingAlgorithms::ReferenceImageCorrection::eInterpOrderY m_yInterpOrder;
    ImagingAlgorithms::ReferenceImageCorrection::eInterpMethod m_InterpMethod;
    ImagingAlgorithms::ReferenceImageCorrection::eMaskCreationMethod m_maskCreationMethod;
    int m_nWindow;

    kipl::base::TImage <float,2> m_Preview_OBBB;
    kipl::base::TImage <float,2> m_Preview_sampleBB;
    kipl::base::TImage <float,2> m_User_Mask;

    BBLogNorm module;
};

#endif // BBLOGNORMDLG_H
