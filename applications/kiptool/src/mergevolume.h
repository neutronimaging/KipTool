//<LICENSE>

#ifndef MERGEVOLUME_H
#define MERGEVOLUME_H

#include <string>
#include <vector>

#include <logging/logger.h>
#include <base/timage.h>
#include <tiffio.h>


class MergeVolume
{
protected:
    kipl::logging::Logger logger;
public:
    MergeVolume();
    ~MergeVolume();

    void Process();
    void LoadVerticalSlice(std::string filemask,
                                        int first,
                                        int last,
                                        kipl::base::TImage<float,2> *img);

    std::string WriteXML(size_t indent=4);
    void ParseXML(std::string fname);

    std::string m_sPathA;
    std::string m_sPathB;
    std::string m_sPathOut;
    std::string m_sMaskOut;

    int m_nFirstA;
    int m_nLastA;
    int m_nStartOverlapA;
    int m_nOverlapLength;
    int m_nFirstB;
    int m_nLastB;
    int m_nFirstDest;

    int m_nMergeOrder;

    bool m_bCropSlices;
    std::vector<int> m_nCropOffset;
    std::vector<int> m_nCrop;
protected:
    void CopyMerge();
    void CropMerge();
};

#endif // MERGEVOLUME_H
