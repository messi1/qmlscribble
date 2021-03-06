#include <QBrush>
#include <QDebug>
#include <QPainter>
#include <QPointF>

#include "drawarea.h"

DrawArea::DrawArea(QQuickItem *parent) :
    QQuickPaintedItem(parent), mModified(false), mScribbling(false), mDrawMode(DA_FREEHAND),mPenWidth(1), mPenColor(Qt::blue)
{
    setAcceptedMouseButtons(Qt::AllButtons);
//    setRenderTarget(QQuickPaintedItem::FramebufferObject);
}

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

bool DrawArea::saveImage(const QString& fileName, const QString& fileFormat) {
    QImage visibleImage = mImage;
    resizeImage(&visibleImage, this->size());

    if (visibleImage.save(fileName, fileFormat.toLatin1())) {
        mModified = false;
        return true;
    } else
        return false;
}

void DrawArea::setPenColor(const QColor &newColor)
{
    mPenColor = newColor;
}

void DrawArea::setPenWidth(int newWidth)
{
    mPenWidth = newWidth;
}

void DrawArea::clearImage() {
    mImage.fill(Qt::transparent);
    mModified = true;
    update();
    clearOverlayImage();
}

void DrawArea::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
    	mStartPoint = event->pos();
   		mScribbling = true;
	}
}

void DrawArea::mouseMoveEvent(QMouseEvent *event)
{
    if((event->buttons() & Qt::LeftButton) && mScribbling )
    {
        //clearOverlayImage();
        drawTo(event->pos(), false);
    }
}

void DrawArea::mouseReleaseEvent(QMouseEvent *event)
{
     if (event->button() == Qt::LeftButton && mScribbling ) {
        drawTo(event->pos(), true);
        mScribbling = false;
    }
}

void DrawArea::paint(QPainter *painter) {

//    painter->setRenderHints(QPainter::HighQualityAntialiasing);
    painter->drawImage(mCurrRect, mImage, mCurrRect);
//    painter->drawImage(boundingRect(), mOverlayImage, boundingRect());
//    painter->drawImage(boundingRect(), mImage, boundingRect());
//    painter->drawImage(boundingRect(), mOverlayImage, boundingRect());
}

void DrawArea::geometryChanged(const QRectF &newGeometry, const QRectF &oldGeometry) {
    if (newGeometry.width() > mImage.width() || newGeometry.height() > mImage.height()) {
        int newWidth = qMax(int(newGeometry.width()) + 128, mImage.width());
        int newHeight = qMax(int(newGeometry.height()) + 128, mImage.height());
        resizeImage(&mImage, QSize(newWidth, newHeight));
        resizeImage(&mOverlayImage, QSize(newWidth, newHeight));
    }

    QQuickPaintedItem::geometryChanged(newGeometry, oldGeometry);
}

void DrawArea::drawLineTo(const QPoint &endPoint, bool final) {
    QPainter painter;
    int rad = (mPenWidth / 2) + 2;

    if(final)
    {
        painter.begin(&mImage);
        mModified = true;
    }
    else
        painter.begin(&mOverlayImage);

    painter.setPen(QPen(mPenColor, mPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    if(boundingRect().toRect().contains(endPoint))
    {
        painter.drawLine(mStartPoint, endPoint);

        mCurrRect = QRect(mStartPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad);
        update(mCurrRect);
        mPrevRect = mCurrRect;

        if(mDrawMode==DA_FREEHAND)
            mStartPoint = endPoint;
    }
}

void DrawArea::drawRectangle(const QPoint &endPoint, bool final)
{
    QPainter painter;
    int rad = (mPenWidth / 2) + 2;
    QRect rectangle(qMin(endPoint.x(),mStartPoint.x()), qMin(endPoint.y(),mStartPoint.y()),
                     qAbs(endPoint.x()-mStartPoint.x()), qAbs(endPoint.y()-mStartPoint.y()));

    if(final)
    {
        painter.begin(&mImage);
        mModified = true;
    }
    else
        painter.begin(&mOverlayImage);

    painter.setBrush(QBrush(mPenColor, Qt::SolidPattern));
    painter.setPen(QPen(mPenColor, mPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawRect(rectangle);

    update(rectangle.united(mPrevRect).normalized().adjusted(-rad, -rad, +rad, +rad));

    mPrevRect = rectangle;
}

void DrawArea::drawCircle(const QPoint &endPoint, bool final)
{
    QPainter painter;
    int radius  = qAbs((mStartPoint-endPoint).manhattanLength());
    int rad = (mPenWidth / 2) + 2;
    QRect rectangle(mStartPoint.x()-(radius*1.414), mStartPoint.y()-(radius*1.414), 2*radius, 2*radius);

    if(final)
    {
        painter.begin(&mImage);
        mModified = true;
    }
    else
        painter.begin(&mOverlayImage);

    painter.setBrush(QBrush(mPenColor, Qt::SolidPattern));
    painter.setPen(QPen(mPenColor, mPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    painter.drawEllipse(rectangle);

    update(rectangle.united(mPrevRect).normalized().adjusted(-rad, -rad, +rad, +rad));

    mPrevRect = rectangle;
}

void DrawArea::drawTo(const QPoint &point, bool final)
{
    switch(mDrawMode)
    {
        default:
        case DA_FREEHAND:
            drawLineTo(point, true);
        break;

        case DA_LINE:
            drawLineTo(point, final);
        break;

        case DA_RECTANGLE:
            drawRectangle(point, final);
        break;

        case DA_CIRCLE:
            drawCircle(point, final);
        break;

        case DA_POLYGON:
        break;
    }
}

QSize DrawArea::size() const {
    return QSize(boundingRect().size().toSize());
}

void DrawArea::resizeImage(QImage *image, const QSize &newSize) {
    if (image->size() == newSize)
        return;

    QImage newImage(newSize, QImage::Format_ARGB32_Premultiplied);
    newImage.fill(Qt::transparent);
    QPainter painter(&newImage);
    painter.drawImage(QPoint(0, 0), *image);
    *image = newImage;
}

void DrawArea::setDrawMode(DrawArea::DrawMode mode)
{
    if(mode < _DA_COUNT)
        mDrawMode = mode;
}

void DrawArea::clearOverlayImage() {
    mOverlayImage.fill(Qt::transparent);
    update();
}

void DrawArea::mousePressEvent( QPoint pos ) {
        mStartPoint = pos;
        mScribbling = true;
}

void DrawArea::mouseMoveEvent( QPoint pos ) {
    if( mScribbling )
    {
        clearOverlayImage();
        drawTo(pos, false);
    }
}

void DrawArea::mouseReleaseEvent( QPoint pos ) {
    if( mScribbling ) {
        drawTo(pos, true);
        mScribbling = false;
        clearOverlayImage();
    }
}
