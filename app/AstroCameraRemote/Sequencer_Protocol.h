#ifndef SEQUENCER_PROTOCOL_H
#define SEQUENCER_PROTOCOL_H

#include <QObject>
#include <QDateTime>
#include <QList>
#include <QDomDocument>
#include <QXmlStreamWriter>

#include "Sequencer_Properties.h"
#include "Sequencer_PhotoShot.h"

namespace Sequencer {

class Protocol : public QObject
{
    Q_OBJECT

public :


    typedef QList<PhotoShot> PhotoShotList;
    typedef QMap<PhotoShot::Type, PhotoShotList> PhotoShotMap;

private :

    enum Status
    {
          Status_Stopped
        , Status_Recording
        , Status_Stopping
    };

    Status status;


    Properties properties;
    PhotoShotMap photoShots;
    QList<QRectF> markers;
    QDateTime startTime;
    QString objectName;

    PhotoShot::Type currentPhotoShotType;

public:
    explicit Protocol(const QString& name, QObject *parent = nullptr);
    ~Protocol();

    bool isRecording() const;

    void serializeXml(QXmlStreamWriter& writer) const;
    void deSerializeXml(const QByteArray& data);

    const Properties &getProperties() const;
    void setProperties(const Properties &value);

    QDateTime getStartTime() const;
    int getNumShotsFinished() const;

    static QString getProtocolPath(bool createIfNotExists = false);

    QString getObjectName() const;
    void setObjectName(const QString &value);

    QString getFilePath() const;

    QList<QRectF> getReferenceMarkers() const;

    const PhotoShotMap& getPhotoShots() const;

    int compareTo(const Protocol* rhs) const;

    PhotoShot::Type getCurrentPhotoShotType() const;
    void setCurrentPhotoShotType(const PhotoShot::Type &value);

public Q_SLOTS :
    void start();
    void havePostViewUrl(QString url, int index, int numShots);
    void havePostViewImage(const QByteArray& data);
    void setReferenceMarkers(const QList<QRectF>& markers);
    void cleanUpMarkers();
    void stop();
    void save();

    bool deleteFile();

};

} // namespace Sequencer

#endif // SEQUENCER_PROTOCOL_H
