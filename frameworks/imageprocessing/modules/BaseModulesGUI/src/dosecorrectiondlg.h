//<LICENSE>
#ifndef DOSECORRECTIONDLG_H
#define DOSECORRECTIONDLG_H

#include <ConfiguratorDialogBase.h>
#include <vector>

namespace Ui {
class DoseCorrectionDlg;
}

class DoseCorrectionDlg : public ConfiguratorDialogBase
{
    Q_OBJECT

public:
    explicit DoseCorrectionDlg(QWidget *parent = nullptr);
    virtual ~DoseCorrectionDlg();

    virtual int exec(ConfigBase * config, std::map<std::string, std::string> &parameters, kipl::base::TImage<float,3> & img);
private:
    virtual int exec() { return QDialog::exec(); }
    virtual void UpdateDialog();
    virtual void UpdateParameters();
    virtual void ApplyParameters() ;
    void UpdateParameterList(std::map<std::string, std::string> &parameters);
    void updateImage(kipl::base::TImage<float,3> & img);

    Ui::DoseCorrectionDlg *ui;

    double m_fSlope;
    double m_fIntercept;

    std::vector<size_t> m_nROI;
};

#endif // DOSECORRECTIONDLG_H
