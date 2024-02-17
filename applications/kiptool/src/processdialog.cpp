//<LICENSE>

#include <thread>
#include <chrono>

#include <QMessageBox>


#include "processdialog.h"
#include "ui_processdialog.h"
#include <KiplFrameworkException.h>
#include <ModuleException.h>
#include <base/KiplException.h>

ProcessDialog::ProcessDialog(kipl::interactors::InteractionBase *interactor, QWidget *parent) :
    QDialog(parent),
    logger("ProcessDialog"),
    ui(new Ui::ProcessDialog),
    fraction(0.0f),
    finish(false),
    m_Interactor(interactor)
{
    ui->setupUi(this);
    if (interactor==nullptr)
        throw KiplFrameworkException("Progress dialog can't open without valid interactor");

    connect(this,&ProcessDialog::updateProgress,this,&ProcessDialog::changedProgress);
    connect(this,&ProcessDialog::processFailure,this,&ProcessDialog::on_processFailure);
    connect(this,&ProcessDialog::processDone,this,&ProcessDialog::on_processDone);
}

ProcessDialog::~ProcessDialog()
{
    delete ui;
}


int ProcessDialog::exec(KiplEngine * engine, kipl::base::TImage<float,3> *img)
{
    if (img==nullptr)
    {
        logger.error("Called recon dialog with uninitialized image");
        return Rejected;
    }

    m_img=img;

    if (engine==nullptr)
    {
        logger.error("Called recon dialog with unallocated engine");
        return Rejected;
    }

    m_Engine=engine;

    finish=false;
    logger.message("Start");

    m_Interactor->Reset();

    logger.message("Starting with threads");
    ui->progressBar->setValue(0);
    ui->progressBar->setMaximum(100);

    auto process_thread  = std::thread([=]{ process(); } );
    auto progress_thread = std::thread([=]{ progress();} );
    
    auto res = QDialog::exec();
    
    finish=true;
    
    process_thread.join();
    progress_thread.join();
    
    if (res==QDialog::Rejected) 
    {
        logger.verbose("Cancel requested by user");
        Abort();
    }

    if (res==QDialog::Accepted) 
    {
        logger.message("Loading finished well");
    }
    
    logger.verbose("Threads are joined");
    return res;

}

int ProcessDialog::progress()
{
    logger.message("Progress thread is started");
    std::this_thread::sleep_for(std::chrono::milliseconds(250));

    while (!m_Interactor->Finished() && !m_Interactor->Aborted() ){
        emit updateProgress(m_Interactor->CurrentProgress(),
                            m_Interactor->CurrentOverallProgress(),
                            QString::fromStdString(m_Interactor->CurrentMessage()));

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    logger.message("Progress thread end");

    return 0;
}

void ProcessDialog::changedProgress(float progress, float overallProgress, QString msg)
{
    ui->progressBar->setValue(static_cast<int>(progress*100));
    ui->progressBar_overall->setValue(static_cast<int>(overallProgress*100));

    ui->label_message->setText(msg);
}

int ProcessDialog::process()
{
    logger(kipl::logging::Logger::LogMessage,"Process thread is started");
    ostringstream msg;

    bool failed=false;
    try {
        if (m_Engine!=nullptr)
            m_Engine->Run(m_img);
        else {
            logger(logger.LogError,"Trying to start an unallocated engine.");
            failed=true;
        }
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
        logger(kipl::logging::Logger::LogMessage,msg.str());
        finish=false;
        emit processFailure(QString::fromStdString(msg.str()));
        return 0;
    }
    logger(kipl::logging::Logger::LogMessage,"Processing done");

    finish=true;
    try {
        Done();
        emit processDone();
    }
    catch (std::exception &e) 
    {
        msg.str("");
        msg<<"Caught an exception the closing the dialog \n"<<e.what();
        logger.error(msg.str());
    }

    logger.message("Process thread done");
    return 0;
}

void ProcessDialog::Abort()
{
    if (m_Interactor!=nullptr) 
    {
        m_Interactor->Abort();
    }
    this->reject();
}

bool ProcessDialog::Finished()
{
    if (m_Interactor!=nullptr) 
    {
        return m_Interactor->Finished();
    }

    return true;
}

void ProcessDialog::Done()
{
    if (m_Interactor!=nullptr) 
    {
        m_Interactor->Done();
    }
}

void ProcessDialog::on_processFailure(QString msg)
{
    QMessageBox dlg;
    dlg.setWindowTitle("Processing error");

    dlg.setText("Processing failed");
    dlg.setDetailedText(msg);
    dlg.exec();
    Abort();
}

void ProcessDialog::on_processDone()
{
    accept();
}
void ProcessDialog::on_buttonBox_rejected()
{
    Abort();
    this->reject();
}
