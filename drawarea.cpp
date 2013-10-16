#include <QBrush>
#include <QPainter>
#include <QPointF>

#include "drawarea.h"

DrawArea::DrawArea(QQuickItem *parent) :
    QQuickPaintedItem(parent), mModified(false), mScribbling(false), mPenWidth(1), mPenColor(Qt::blue)
{}

bool DrawArea::openImage(const QString &fileName) {
    QImage loadedImage;
    if (!loadedImage.load(QUrl(fileName).toLocalFile()))
        return false;

    QSize newSize = loadedImage.size().expandedTo(this->size());
    resizeImage(&loadedImage, newSize);
    mImage = loadedImage;
    mModified = false;
    update();
    return true;
}

bool DrawArea::saveImage(const QString fileName, QString fileFormat) {
    QImage visibleImage = mImage;
    resizeImage(&visibleImage, this->size());

    if (visibleImage.save(fileName, fileFormat.toLatin1())) {
        mModified = false;
        return true;
    } else
        return false;
}

void DrawArea::setPenColor(const QColor &newColor) {
    mPenColor = newColor;
}

void DrawArea::clearImage() {
    mImage.fill(qRgb(255, 255, 255));
    mModified = true;
    update();
}

int DrawArea::penWidth() {
    return mPenWidth;
}

QColor DrawArea::penColor() {
    return mPenColor;
}

void DrawArea::setPenWidth(int newWidth) {
    mPenWidth = newWidth;
}

void DrawArea::mousePressEvent( QPoint pos ) {
        mLastPoint = pos;
        mScribbling = true;
}

void DrawArea::mouseMoveEvent( QPoint pos ) {
    if( mScribbling )
        drawLineTo(pos);
}

void DrawArea::mouseReleaseEvent( QPoint pos ) {
    if( mScribbling ) {
        drawLineTo(pos);
        mScribbling = false;
    }
}

void DrawArea::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) {
    if (newGeometry.width() > mImage.width() || newGeometry.height() > mImage.height()) {
        int newWidth = qMax(int(newGeometry.width()) + 128, mImage.width());
        int newHeight = qMax(int(newGeometry.height()) + 128, mImage.height());
        resizeImage(&mImage, QSize(newWidth, newHeight));
        update();
    }
    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
}

void DrawArea::drawLineTo(const QPoint &endPoint) {
    QPainter painter(&mImage);
    int rad = (mPenWidth / 2) + 2;
    painter.setPen(QPen(mPenColor, mPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawLine(mLastPoint, endPoint);
    mModified = true;
    update(QRect(mLastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad));
    mLastPoint = endPoint;
}

QSize DrawArea::size() const {
    return QSize(boundingRect().size().toSize());
}

void DrawArea::resizeImage(QImage *image, const QSize &newSize) {
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_RGB32);
    newImage.fill(qRgb(255, 255, 255));
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void DrawArea::paint(QPainter *painter) {
    painter->drawImage(boundingRect(), mImage, boundingRect());
}
