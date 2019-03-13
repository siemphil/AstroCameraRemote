#include "ImageView.h"

#include <QResizeEvent>
#include <QtMath>

#include "AstroBase.h"

ImageView::ImageView(QWidget* parent)
    : QGraphicsView(parent)
{
    viewport()->installEventFilter(this);
    setMouseTracking(true);
    zoomFactorBbase = 1.0015;
}

void ImageView::gentleZoom(double factor)
{
    scale(factor, factor);
    centerOn(targetScenePos);
    QPointF delta_viewport_pos =
            targetViewportPos - QPointF(viewport()->width() / 2.0,
                                        viewport()->height() / 2.0);
    QPointF viewport_center = mapFromScene(targetScenePos) - delta_viewport_pos;
    centerOn(mapToScene(viewport_center.toPoint()));
    Q_EMIT zoomed();
}


void ImageView::setZoomFactorBase(double value)
{
    zoomFactorBbase = value;
}

bool ImageView::eventFilter(QObject *object, QEvent *event)
{
    if (event->type() == QEvent::MouseMove)
    {
        QMouseEvent* mouse_event = static_cast<QMouseEvent*>(event);
        QPointF delta = targetViewportPos - mouse_event->pos();
        if (qAbs(delta.x()) > 5 || qAbs(delta.y()) > 5)
        {
            targetViewportPos = mouse_event->pos();
            targetScenePos = mapToScene(mouse_event->pos());
        }
    }
    else if (event->type() == QEvent::Wheel)
    {
        QWheelEvent* wheel_event = static_cast<QWheelEvent*>(event);
        if (wheel_event->modifiers().testFlag(Qt::ControlModifier))
        {
            if (wheel_event->orientation() == Qt::Vertical)
            {
                double angle = wheel_event->angleDelta().y();
                double factor = qPow(zoomFactorBbase, angle);
                gentleZoom(factor);
                return true;
            }
        }
    }
    Q_UNUSED(object)
    return false;
}
