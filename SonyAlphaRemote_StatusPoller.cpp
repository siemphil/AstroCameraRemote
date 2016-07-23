#include "SonyAlphaRemote_StatusPoller.h"

#include <math.h>
#include "SonyAlphaRemote_Json_Command.h"
#include "SonyAlphaRemote_Helper.h"
#include <QUrl>

namespace SonyAlphaRemote {

QString StatusPoller::getCameraStatus() const
{
    return cameraStatus;
}

BatteryInfo StatusPoller::getBatteryInfo() const
{
    return batteryInfo;
}


void StatusPoller::simCamReady()
{
    Q_EMIT statusChanged("IDLE");
}

StatusPoller::StatusPoller(Sender *sender, QObject *parent)
    : QObject(parent)
    , sender(sender)
    , trigger(NULL)
    , cameraStatus(tr("not connected"))
    , getEvent(new Json::GetEvent(this))
    , waitingForEventReply(false)
{
    connect(getEvent, SIGNAL(confirmed()), this, SLOT(handleEventReply()));
    connect(getEvent, SIGNAL(declined()), this, SLOT(handleEventError()));
}

void StatusPoller::start(double interval)
{
    if(trigger)
        delete trigger;
    trigger = new QTimer(this);
    connect(trigger, SIGNAL(timeout()), this, SLOT(poll()));

    int msec = (int)trunc(interval * 1000.);
    getEvent->setCallbackImmedialetyEnabled(true);
    trigger->start(msec);

    SAR_INF("status poller started");
}

void StatusPoller::stop()
{
    if(trigger)
        delete trigger;
    trigger = NULL;

    SAR_INF("status poller stopped");
}

bool StatusPoller::isActive() const
{
    return trigger && trigger->isActive();
}

void StatusPoller::poll()
{
    if(!waitingForEventReply)
    {
//        SAR_INF("SEND GETEVENT NOW (" << getEvent->isCallbackImmedialetyEnabled() << ")");
        sender->send(getEvent);
        waitingForEventReply = true;
    }
}


bool StatusPoller::getEnumeratedOption(
        int index, const QString& type, QJsonArray status
        , const QString& currentName, const QString& candidatesName
        , QString& current, QStringList& candidates) const
{
    int statusCount = status.count();
    if(statusCount <= index)
        return false;

    QJsonValueRef value = status[index];
    if(value.isNull() || !value.isObject())
        return false;

    QJsonObject obj = value.toObject();
    if(type != obj["type"].toString())
        return false;

    if(!obj.contains(candidatesName))
        return false;

    QJsonArray array = obj[candidatesName].toArray();
    for(int i=0; i<array.count(); i++)
    {
        candidates << array[i].toString();
    }

    if(!obj.contains(currentName))
        return false;

    current = obj[currentName].toString();

    return true;

}

void StatusPoller::getBatteryInfo(QJsonArray status)
{
    int statusCount = status.count();
    if(statusCount <= 56)
        return;

    QJsonValueRef value = status[56];
    if(value.isNull() || !value.isObject())
        return;

    QJsonObject obj = value.toObject();
    if(obj["type"].toString() != "batteryInfo")
        return;

    QJsonArray batteryInfos = obj["batteryInfo"].toArray();
    if(0 >= batteryInfos.count())
        return;

    QJsonObject batteryInfo = batteryInfos[0].toObject();

    this->batteryInfo = BatteryInfo(batteryInfo);
    Q_EMIT batteryStatusChanged();
}



void StatusPoller::handleEventReply()
{
//    SAR_INF("RECV GETEVENT NOW (" << getEvent->isCallbackImmedialetyEnabled() << ")");
    getEvent->setCallbackImmedialetyEnabled(false);

    QJsonValueRef camStatVal = getEvent->getStatus()[1];
    if(!camStatVal.isNull() && camStatVal.isObject())
    {
        cameraStatus = camStatVal.toObject()["cameraStatus"].toString();
        SAR_INF(cameraStatus);
        Q_EMIT statusChanged(cameraStatus);
    }

    QJsonArray eventStatusArray = getEvent->getStatus();

    QString isoSpeed; QStringList isoSpeedCandidates;
    if(getEnumeratedOption(29, "isoSpeedRate", eventStatusArray, "currentIsoSpeedRate", "isoSpeedRateCandidates", isoSpeed, isoSpeedCandidates))
        Q_EMIT isoSpeedRatesChanged(isoSpeedCandidates, isoSpeed);

    QString shutterSpeed; QStringList shutterSpeedCandidates;
    if(getEnumeratedOption(32, "shutterSpeed", eventStatusArray, "currentShutterSpeed", "shutterSpeedCandidates", shutterSpeed, shutterSpeedCandidates))
        Q_EMIT shutterSpeedsChanged(shutterSpeedCandidates, shutterSpeed);

    QString stillQuality; QStringList stillQualityCandidates;
    if(getEnumeratedOption(37, "stillQuality", eventStatusArray, "stillQuality", "candidate", stillQuality, stillQualityCandidates))
        Q_EMIT shutterSpeedsChanged(shutterSpeedCandidates, shutterSpeed);

    getBatteryInfo(eventStatusArray);

    waitingForEventReply = false;
}

void StatusPoller::handleEventError()
{
    getEvent->setCallbackImmedialetyEnabled(false);
    waitingForEventReply = false;
}



} // namespace SonyAlphaRemote
