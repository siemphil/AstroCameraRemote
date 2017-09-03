#ifndef LIVEVIEW_THREADINFO_H
#define LIVEVIEW_THREADINFO_H

#include <QQueue>
#include <QByteArray>
#include <QMutex>


namespace LiveView {

struct ThreadInfo
{
    class JpegQueue
    {
        QQueue<QByteArray> q;
        mutable QMutex m;

    public :
        QByteArray pop();
        void push(QByteArray data);
        int size() const;
        void clear();

    };

    JpegQueue q;
    bool running;
    float targetFps;
    ThreadInfo() : running(false), targetFps(0.5) {}
};

} // namespace LiveView

#endif // LIVEVIEW_THREADINFO_H
