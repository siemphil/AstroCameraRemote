#include "Json_Command.h"

#include <QJsonObject>
#include <QJsonArray>
#include "Helper.h"
#include "AstroBase.h"

#include <share.h>

namespace Json {


void Command::setReply(QNetworkReply *value)
{
    if(!value)
        AB_WRN("no reply object");
    reply = value;
    connect(reply, SIGNAL(finished()), this, SLOT(handleReply()));
}

void Command::handleReply(const QJsonDocument &replyJson)
{
    if(!replyJson.isObject() || !replyJson.object().contains("result") || !replyJson.object()["result"].isArray())
    {
        handleError(replyJson, tr("invalid reply"));
        return;
    }

    Q_EMIT confirmed();

}

void Command::handleError(const QJsonDocument &replyJson, const QString &msg)
{
    Q_EMIT error(tr("%0 (command: %1, json: %2)").arg(msg).arg(objectName()).arg(QString(replyJson.toJson())));
}

void Command::handleReply()
{
    if(!reply)
    {
        AB_ERR("(" << objectName() << ") reply is NULL");
        return;
    }
    if(reply->error() != QNetworkReply::NoError)
    {
        AB_ERR("(" << objectName() << ") reply error: " << reply->error());
//        return;
    }
    QByteArray replyData { reply->readAll() };

    reply->deleteLater();
    reply = nullptr;

    AB_INF("(" << objectName() << ") reply: " << QString(replyData));
    handleReply(QJsonDocument::fromJson(replyData));
}

Command::Command(QObject *parent)
    : QObject { parent }
    , json {{ "id", 1 }, { "version", "1.0" }}
{
}

QByteArray Command::getJson() const
{
    return QJsonDocument { json }.toJson();
}



void SetShutterSpeed::setShutterSpeed(const QString &value)
{
    json["params"] = QJsonArray { value };
}

QString SetShutterSpeed::getShutterSpeed() const
{
    return json["params"].toString();
}

SetShutterSpeed::SetShutterSpeed(QObject* parent)
    : Command{parent}
{
    setObjectName("SetShutterSpeed");
    json["method"] ="setShutterSpeed";
}

void SetIsoSpeedRate::setIsoSpeedRate(const QString &value)
{
    json["params"] = QJsonArray { value };
}

QString SetIsoSpeedRate::getIsoSpeedRate() const
{
    return json["params"].toString();
}

SetIsoSpeedRate::SetIsoSpeedRate(QObject* parent)
    : Command { parent }
{
    setObjectName("SetIsoSpeedRate");

    json["method"] = "setIsoSpeedRate";
}

GetShutterSpeed::GetShutterSpeed(QObject* parent)
    : Command { parent }
{
    setObjectName("GetShutterSpeed");

    json["method"] = "getShutterSpeed";
    json["params"] = QJsonArray {};
}

StartRecMode::StartRecMode(QObject* parent)
    : Command { parent }
{
    setObjectName("StartRecMode");
    json["method"] = "startRecMode";
    json["params"] = QJsonArray {};

}


StopRecMode::StopRecMode(QObject* parent)
    : Command { parent }
{
    setObjectName("StopRecMode");
    json["method"] = "stopRecMode";
    json["params"] = QJsonArray {};
}


ActTakePicture::ActTakePicture(QObject *parent)
    : PostViewProviderCommand { parent }
{
    setObjectName("ActTakePicture");
    json["method"] = "actTakePicture";
    json["params"] = QJsonArray {};
}

SetContShootingMode::SetContShootingMode(QObject *parent)
    : Command { parent }
{
    setObjectName("SetContShootingMode");
    json["method"] = "setContShootingMode";
}

SetContShootingMode::Mode SetContShootingMode::getMode() const
{
    return string2Mode(json["params"].toObject()["contShootingMode"].toString());
}

void SetContShootingMode::setMode(const Mode &value)
{
    json["params"] = QJsonObject { { "contShootingMode", mode2String(value) } };
}

QString SetContShootingMode::mode2String(Mode mode)
{
    switch(mode)
    {
    case Mode::Single          : return "Single";
    case Mode::Continuous      : return "Continuous";
    case Mode::SpdPriorityCont : return "Spd Priority Cont.";
    case Mode::Burst           : return "Burst";
    case Mode::MotionShot      : return "MotionShot";
    case Mode::InvalidMode     : break;
    }
    return "Invalid mode";
}

SetContShootingMode::Mode SetContShootingMode::string2Mode(const QString &mode)
{
    if(mode == "Single")
        return Mode::Single;
    else if(mode == "Continuous")
        return Mode::Continuous;
    else if(mode == "Spd Priority Cont.")
        return Mode::SpdPriorityCont;
    else if(mode == "Burst")
        return Mode::Burst;
    else if(mode == "MotionShot")
        return Mode::MotionShot;

    return Mode::InvalidMode;
}

const QString &GetAvailableContShootingModes::getCurrentContShootingMode() const
{
    return currentContShootingMode;
}

const QStringList& GetAvailableContShootingModes::getContShootingModes() const
{
    return contShootingModes;
}

GetAvailableContShootingModes::GetAvailableContShootingModes(QObject *parent)
    : Command(parent)
{
    setObjectName("GetAvailableContShootingModes");
    json["method"] = "getAvailableContShootingMode";
    json["params"] = QJsonArray {};
}

void GetAvailableContShootingModes::handleReply(const QJsonDocument &replyJson)
{
    if(!replyJson.isObject() || !replyJson.object().contains("result") || !replyJson.object()["result"].isArray())
    {
        handleError(replyJson, tr("invalid reply"));
        return;
    }

    QJsonArray result = replyJson.object()["result"].toArray();
    if(result.count() != 2)
    {
        handleError(replyJson, tr("\"result\" is not an array"));
        return;
    }

    currentContShootingMode = result[0].toString();
    if(!result[1].isArray())
    {
        handleError(replyJson, tr("second entry of \"result\" (cont. shooting modes) is not an array"));
        return;
    }

    contShootingModes.clear();
    QJsonArray available = result[1].toArray();
    for(int i=0; i<available.count(); i++)
    {
        contShootingModes << available[i].toString();
    }

    if(!contShootingModes.contains(currentContShootingMode))
    {
        handleError(replyJson, tr("cont. shooting modes don't contain curent cont. shooting mode"));
        return;
    }

    Q_EMIT confirmed();

}

StartBulbShooting::StartBulbShooting(QObject* parent)
    : Command(parent)
{
    setObjectName("StartBulbShooting");
    json["method"] = "startBulbShooting";
    json["params"] = QJsonArray {};
}


StopBulbShooting::StopBulbShooting(QObject* parent)
    : Command(parent)
{
    setObjectName("StopBulbShooting");
    json["method"] = "stopBulbShooting";
    json["params"] = QJsonArray {};
}


AwaitTakePicture::AwaitTakePicture(QObject* parent)
    : PostViewProviderCommand(parent)
{
    setObjectName("AwaitTakePicture");
    json["method"] = "awaitTakePicture";
    json["params"] = QJsonArray {};
}

void PostViewProviderCommand::handleReply(const QJsonDocument &replyJson)
{
    if(!replyJson.isObject() || !replyJson.object().contains("result") || !replyJson.object()["result"].isArray())
    {
        handleError(replyJson, tr("invalid reply"));
        return;
    }

    QJsonObject obj = replyJson.object();
    QJsonArray result = obj["result"].toArray();
    if(result.count() < 1 || !result[0].isArray())
    {
        handleError(replyJson, tr("invalid result"));
        return;
    }

    QJsonArray postView = result[0].toArray();
    if(postView.count() < 1)
    {
        handleError(replyJson, tr("no post view url"));
        return;
    }

    postViewUrl = postView[0].toString();

    Q_EMIT havePostViewUrl(postViewUrl);

    Q_EMIT confirmed();

}

PostViewProviderCommand::PostViewProviderCommand(QObject *parent)
    : Command(parent)
{

}

} // namespace Json

