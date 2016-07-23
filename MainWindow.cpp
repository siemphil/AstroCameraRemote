#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "SonyAlphaRemote_Helper.h"
#include "SonyAlphaRemote_Sender.h"

#include <QMessageBox>

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(ui->toggleRecordModeBtn->isChecked())
    {
        sender->send(stopRecMode);
        aboutToClose = true;
        event->ignore();
    }
    else
    {
        statusPoller->stop();
        event->accept();
    }
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , startRecMode(new SonyAlphaRemote::Json::StartRecMode(this))
    , stopRecMode(new SonyAlphaRemote::Json::StopRecMode(this))
    , setShutterSpeed(new SonyAlphaRemote::Json::SetShutterSpeed(this))
    , setIsoSpeedRate(new SonyAlphaRemote::Json::SetIsoSpeedRate(this))
    , actTakePicture(new SonyAlphaRemote::Json::ActTakePicture(this))
    , startBulbShooting(new SonyAlphaRemote::Json::StartBulbShooting(this))
    , stopBulbShooting(new SonyAlphaRemote::Json::StopBulbShooting(this))
    , sender(new SonyAlphaRemote::Sender(this))
    , statusPoller(new SonyAlphaRemote::StatusPoller(sender, this))
    , bulbShootSequencer(new SonyAlphaRemote::Sequencer::BulbShootSequencer(statusPoller, sender, this))
    , aboutToClose(false)
    , currentTimeDisplayTimer(new QTimer(this))

    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(currentTimeDisplayTimer, SIGNAL(timeout()), this, SLOT(updateCurrentTimeDisplay()));
    currentTimeDisplayTimer->start(10);

    connect(sender, SIGNAL(loadedPostViewImage(QByteArray)), this, SLOT(updatePostViewImage(QByteArray)));

    connect(statusPoller, SIGNAL(statusChanged(QString)), this, SLOT(handleCameraStatus(QString)));
    connect(statusPoller, SIGNAL(isoSpeedRatesChanged(QStringList,QString))
            , this, SLOT(isoSpeedRatesChanged(QStringList,QString)));
    connect(statusPoller, SIGNAL(shutterSpeedsChanged(QStringList,QString))
            , this, SLOT(shutterSpeedsChanged(QStringList,QString)));

    connect(statusPoller, SIGNAL(batteryStatusChanged()), this, SLOT(updateBatteryStatus()));


    connect(startRecMode, SIGNAL(confirmed()), this, SLOT(toggleRecordModeBtnStarted()));
    connect(startRecMode, SIGNAL(confirmed()), this, SLOT(hello()));
    connect(startRecMode, SIGNAL(error(QString)), this, SLOT(helloError(QString)));

    connect(stopRecMode, SIGNAL(confirmed()), this, SLOT(toggleRecordModeBtnStopped()));
    connect(stopRecMode, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(setShutterSpeed, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(setIsoSpeedRate, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(actTakePicture, SIGNAL(havePostViewUrl(QString)), this, SLOT(onPostView(QString)));
    connect(actTakePicture, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(startBulbShooting, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(stopBulbShooting, SIGNAL(error(QString)), this, SLOT(error(QString)));

    connect(bulbShootSequencer, SIGNAL(statusMessage(QString)), this, SLOT(appendOutputMessage(QString)));
    connect(bulbShootSequencer, SIGNAL(havePostViewUrl(QString, int, int)), this, SLOT(onPostView(QString, int, int)));


//    ui->centralWidget->setEnabled(false);
    sender->send(startRecMode);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_toggleRecordModeBtn_clicked()
{
    if(!ui->toggleRecordModeBtn->isChecked())
    {
        sender->send(stopRecMode);
    }
    else
    {
        sender->send(startRecMode);
    }
}

void MainWindow::toggleRecordModeBtnStopped()
{
    ui->toggleRecordModeBtn->setText(tr("Start record mode"));
    ui->toggleRecordModeBtn->setChecked(false);
    if(aboutToClose)
        close();
}

void MainWindow::toggleRecordModeBtnStarted()
{
    ui->toggleRecordModeBtn->setText(tr("Stop record mode"));
    ui->toggleRecordModeBtn->setChecked(true);
}

void MainWindow::hello()
{
    ui->centralWidget->setEnabled(true);

    if(!statusPoller->isActive())
    {
        statusPoller->start(1);
    }
}

void MainWindow::helloError(QString msg)
{
    error(msg);

//    ui->centralWidget->setEnabled(false);
    QMessageBox::StandardButtons btn;
    do
    {
        btn = QMessageBox::question(
                    this, tr("No connection")
                    , tr("no connection to camera!\rRetry to connect?"));

        if(QMessageBox::Yes == btn)
        {
            sender->send(startRecMode);
        }
    }
    while(QMessageBox::Yes == btn);
}

void MainWindow::error(QString msg)
{
    ui->output->append(msg);
}

void MainWindow::isoSpeedRatesChanged(const QStringList &candidates, const QString &current)
{
    ui->isoSpeedRate->clear();
    foreach(QString speed, candidates)
    {
        ui->isoSpeedRate->insertItem(0, speed);
    }
    ui->isoSpeedRate->setCurrentText(current);

}

void MainWindow::shutterSpeedsChanged(const QStringList &candidates, const QString &current)
{
    ui->shutterSpeed->clear();
    foreach(QString speed, candidates)
    {
        ui->shutterSpeed->insertItem(0, speed);
    }
    ui->shutterSpeed->setCurrentText(current);
}

void MainWindow::stillQualityChanged(const QStringList &candidates, const QString &current)
{
    ui->format->clear();
    foreach(QString format, candidates)
    {
        ui->format->insertItem(0, format);
    }
    ui->format->setCurrentText(current);
}


void MainWindow::on_shutterSpeed_activated(const QString &speed)
{
    setShutterSpeed->setShutterSpeed(speed);
    sender->send(setShutterSpeed);
}

void MainWindow::on_takeShotBtn_clicked()
{
    sender->send(actTakePicture);
}

void MainWindow::onPostView(const QString &url)
{
    ui->output->append(tr("have new image: %0").arg(url));
    ui->imageSubTitle->setText(QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss:zzz"));
    sender->loadPostViewImage(url);
}

void MainWindow::onPostView(const QString& url, int i, int numShots)
{
    ui->output->append(tr("have new image: %0 (%1/%2)").arg(url).arg(i).arg(numShots));
    ui->imageSubTitle->setText(tr("image %0/%1").arg(i).arg(numShots));
    sender->loadPostViewImage(url);
}

void MainWindow::updatePostViewImage(QByteArray data)
{
    QPixmap pixmap = QPixmap::fromImage(QImage::fromData(data, "JPG"));
    ui->postViewImage->setPixmap(pixmap);
}

void MainWindow::on_startBulbSequence_clicked()
{
    if(bulbShootSequencer->isRunning())
    {
        ui->output->append(tr("cannot start BULB sequence until running sequence has been finished!"));
        return;
    }

    ui->output->append("-------------------------------------------------------------");
    ui->output->append(tr("start BULB sequence at %0").arg(QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss:zzz")));

    applyBulbSettings();
    int duration = bulbShootSequencer->calculateSequenceDuration();

    QTime dt = QTime(0,0,0,0).addMSecs(duration);
    ui->output->append(
                tr("calculated duration: %0h %1min %2sec %3msec")
                .arg(dt.hour()).arg(dt.minute()).arg(dt.second()).arg(dt.msec()));

    ui->output->append(
                tr("calculated finish time: %0")
                .arg(QDateTime::currentDateTime()
                     .addMSecs(duration).toString("yyyy-MM-ddTHH:mm:ss:zzz")));
    ui->output->append("-------------------------------------------------------------");

    bulbShootSequencer->start();
}

void MainWindow::applyBulbSettings()
{
    bulbShootSequencer->setNumShots(ui->numShots->value());
    bulbShootSequencer->setShutterSpeed(ui->shutterSpeedBulb->value());
    bulbShootSequencer->setPauseDelay(ui->pause->value());
    bulbShootSequencer->setStartDelay(ui->startDelay->value());
}

void MainWindow::appendOutputMessage(QString msg)
{
    ui->output->append(tr("%0: %1").arg(QDateTime::currentDateTime().toString("yyyy-MM-ddTHH:mm:ss:zzz")).arg(msg));
}

void MainWindow::on_simCamReadyBtn_clicked()
{
    statusPoller->simCamReady();
}

void MainWindow::handleCameraStatus(QString status)
{
    ui->cameraStatus->setText(status);
}

void MainWindow::on_isoSpeedRate_activated(const QString &isoSpeedRate)
{
    setIsoSpeedRate->setIsoSpeedRate(isoSpeedRate);
    sender->send(setIsoSpeedRate);
}

void MainWindow::updateCurrentTimeDisplay()
{
    QDateTime now = QDateTime::currentDateTime();

    ui->currentDate->setText(now.toString("dd. MM. yyyy"));
    ui->currentTime->setText(now.toString("HH:MM:ss:zzz"));
}

void MainWindow::updateBatteryStatus()
{
    SonyAlphaRemote::BatteryInfo info = statusPoller->getBatteryInfo();
    ui->batteryStatus->setMinimum(0);
    ui->batteryStatus->setMaximum(info.getLevelDenom());
    ui->batteryStatus->setValue(info.getLevelNumber());

    if(info.getAdditionalStatus() == SonyAlphaRemote::BatteryInfo::AdditionalStatus_batteryNearEnd)
        ui->batteryStatus->setStyleSheet(SonyAlphaRemote::BatteryInfo::getStyleDanger());
    else
        ui->batteryStatus->setStyleSheet(SonyAlphaRemote::BatteryInfo::getStyleNormal());

}
