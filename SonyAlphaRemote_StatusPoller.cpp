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
    waitingForEventReply = false;
}

void StatusPoller::handleEventError()
{
    getEvent->setCallbackImmedialetyEnabled(false);
    waitingForEventReply = false;
}

void StatusPoller::handleAwaitPicReply(const QString &potUrl)
{
    QUrl url = potUrl;
    if(url.isValid())
    {
        Q_EMIT haveNewPostViewPictureUrl(potUrl);
    }
}

void StatusPoller::handleAwaitPicError()
{

}

} // namespace SonyAlphaRemote
