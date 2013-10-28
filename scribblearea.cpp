#include <QPainter>
#include "scribblearea.h"

//! [0]
ScribbleArea::ScribbleArea(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    setAcceptedMouseButtons(Qt::AllButtons);
//    setAttribute(Qt::WA_StaticContents);
    modified = false;
    scribbling = false;
    myPenWidth = 5;
    myPenColor = Qt::blue;
}
//! [0]

//! [1]
bool ScribbleArea::openImage(const QString &fileName)
//! [1] //! [2]
{
    QImage loadedImage;
    if (!loadedImage.load(fileName))
        return false;

    QSize newSize = loadedImage.size().expandedTo(size());
    resizeImage(&loadedImage, newSize);
    image = loadedImage;
    modified = false;
    update();
    return true;
}
//! [2]

//! [3]
bool ScribbleArea::saveImage(const QString &fileName, const char *fileFormat)
//! [3] //! [4]
{
    QImage visibleImage = image;
    resizeImage(&visibleImage, size());

    if (visibleImage.save(fileName, fileFormat)) {
        modified = false;
        return true;
    } else {
        return false;
    }
}
//! [4]

//! [5]
void ScribbleArea::setPenColor(const QColor &newColor)
//! [5] //! [6]
{
    myPenColor = newColor;
}
//! [6]

//! [7]
void ScribbleArea::setPenWidth(int newWidth)
//! [7] //! [8]
{
    myPenWidth = newWidth;
}
//! [8]

//! [9]
void ScribbleArea::clearImage()
//! [9] //! [10]
{
    image.fill(qRgb(255, 255, 255));
    modified = true;
    update();
}
//! [10]

//! [11]
void ScribbleArea::mousePressEvent(QMouseEvent *event)
//! [11] //! [12]
{
    if (event->button() == Qt::LeftButton) {
        lastPoint = event->pos();
        scribbling = true;
    }
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
    if ((event->buttons() & Qt::LeftButton) && scribbling)
        drawLineTo(event->pos());
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && scribbling) {
        drawLineTo(event->pos());
        scribbling = false;
    }
}

//! [12] //! [13]
//void ScribbleArea::paintEvent(QPaintEvent *event)
void ScribbleArea::paint(QPainter *painter)
//! [13] //! [14]
{
//    QPainter painter(this);
//    QRect dirtyRect = event->rect();
//    qDebug() << "DirtyRect: " << dirtyRect;
    painter->drawImage(currRect, image, currRect);
//    painter->drawImage(boundingRect(), image, boundingRect());
}
//! [14]

//! [15]
//void ScribbleArea::resizeEvent(QResizeEvent *event)
void ScribbleArea::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry)
//! [15] //! [16]
{
    if (newGeometry.width() > image.width() || newGeometry.height() > image.height()) {
        int newWidth = qMax(int(newGeometry.width()) + 128, image.width());
        int newHeight = qMax(int(newGeometry.height()) + 128, image.height());
        resizeImage(&image, QSize(newWidth, newHeight));
//        resizeImage(&mOverlayImage, QSize(newWidth, newHeight));
    }

    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
}
//! [16]

//! [17]
void ScribbleArea::drawLineTo(const QPoint &endPoint)
//! [17] //! [18]
{
    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    if(boundingRect().toRect().contains(endPoint))
    {
        painter.drawLine(lastPoint, endPoint);
        modified = true;

        int rad = (myPenWidth / 2) + 2;
        currRect = QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad);
        update(currRect);
        qDebug() << "Rect: " << QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad);
        lastPoint = endPoint;
    }
}
//! [18]

//! [19]
void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
//! [19] //! [20]
{
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_ARGB32_Premultiplied);
    newImage.fill(Qt::transparent);
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}
//! [20]

QSize ScribbleArea::size() const {
    return QSize(boundingRect().size().toSize());
}
