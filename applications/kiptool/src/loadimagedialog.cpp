//<LICENSE>

#include "loadimagedialog.h"
#include "ui_loadimagedialog.h"

#include <QMessageBox>
#include <thread>
#include <chrono>

#include <KiplFrameworkException.h>
#include <ModuleException.h>
#include <base/KiplException.h>
#include "ImageIO.h"

LoadImageDialog::LoadImageDialog(kipl::interactors::InteractionBase *interactor, QWidget *parent) :
    QDialog(parent),
    logger("LoadImageDialog"),
    ui(new Ui::LoadImageDialog),
    m_Interactor(interactor)
{
    ui->setupUi(this);
    if (interactor==nullptr)
        throw KiplFrameworkException("Progress dialog can't open without valid interactor");

    connect(this,&LoadImageDialog::updateProgress,this,&LoadImageDialog::changedProgress);
    connect(this,&LoadImageDialog::processFailure,this,&LoadImageDialog::on_processFailure);
    connect(this,&LoadImageDialog::processDone,this,&LoadImageDialog::on_processDone);
}

LoadImageDialog::~LoadImageDialog()
{
    delete ui;
}

int LoadImageDialog::exec(KiplProcessConfig *config, kipl::base::TImage<float,3> *img)
{
    if (img==nullptr)
    {
        logger(logger.LogError,"Called load image dialog with uninitialized image");
        return Rejected;
    }

    m_img=img;

    if (config==nullptr)
    {
        logger(logger.LogError,"Called load image dialog without config");
        return Rejected;
    }

    mConfig=config;

    finish=false;
    logger(kipl::logging::Logger::LogMessage,"Start");

    m_Interactor->Reset();

    logger(logger.LogMessage,"Starting with threads");

    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(100);

    auto process_thread  = std::thread([=]{process();});
    auto progress_thread = std::thread([=]{progress();});

    auto res = QDialog::exec();
    finish=true;
    progress_thread.join();
    process_thread.join();

    if (res==QDialog::Rejected) 
    {
        logger.verbose("Cancel requested by user");
        Abort();
    }

    if (res==QDialog::Accepted) 
    {
        logger.message("Loading finished well");
    }

    logger.message("Threads are joined");
    return res;

}

int LoadImageDialog::progress()
{
    logger(kipl::logging::Logger::LogMessage,"Progress thread is started");
    std::this_thread::sleep_for(std::chrono::milliseconds(250));
    while (!m_Interactor->Finished() && !m_Interactor->Aborted() ){
        emit updateProgress(m_Interactor->CurrentProgress(),
                            m_Interactor->CurrentOverallProgress(),
                            QString::fromStdString(m_Interactor->CurrentMessage()));

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    logger(kipl::logging::Logger::LogMessage,"Progress thread end");

    return 0;
}

void LoadImageDialog::changedProgress(float progress, float overallProgress, QString msg)
{
    ui->progressBar->setValue(static_cast<int>(progress*100));

    ui->label_message->setText(msg);
}

int LoadImageDialog::process()
{
    logger(kipl::logging::Logger::LogMessage,"Process thread is started");
    ostringstream msg;

    bool failed=false;
    try {
        *m_img = LoadVolumeImage(*mConfig,m_Interactor);
    }
    catch (KiplFrameworkException &e)
    {
        msg<<"ReconException with message: "<<e.what();
        failed =true;
    }
    catch (ModuleException &e) {
        msg<<"ModuleException with message: "<<e.what();
        failed = true;
    }
    catch (kipl::base::KiplException &e)
    {
        msg<<"KiplException with message: "<<e.what();
        failed = true ;
    }
    catch (std::exception &e)
    {
        msg<<"STL exception with message: "<<e.what();
        failed = true;
    }
    catch (...)
    {
        msg<<"An unknown exception";
        failed =true;
    }

    if (failed==true)
    {
        logger.message(msg.str());
        finish=false;
        emit processFailure(QString::fromStdString(msg.str()));
        return 0;
    }
    logger.message("Loading done");

    finish=true;

    try 
    {
        Done();
        emit processDone();
    } 
    catch (std::exception &e)
    {
        msg.str("");
        msg<<"Caught an exception the closing the dialog \n"<<e.what();
        logger.error(msg.str());
    }

    logger.message("Loading thread done");
    return 0;
}

void LoadImageDialog::Abort()
{
    if (m_Interactor!=nullptr) 
    {
        m_Interactor->Abort();
    }
    this->reject();
}

bool LoadImageDialog::Finished()
{
    if (m_Interactor!=nullptr) 
    {
        return m_Interactor->Finished();
    }

    return true;
}

void LoadImageDialog::Done()
{
    if (m_Interactor!=nullptr) 
    {
        m_Interactor->Done();
    }
}

void LoadImageDialog::on_processFailure(QString msg)
{
    QMessageBox dlg;
    dlg.setWindowTitle("Failed to load image");

    dlg.setText("Could not load the image.");
    dlg.setDetailedText(msg);
    dlg.exec();
    Abort();
}

void LoadImageDialog::on_processDone()
{
    logger.message("process done");
    accept();
    logger.message("process done, accept");
}

void LoadImageDialog::on_buttonBox_rejected()
{
    Abort();
    this->reject();
}
