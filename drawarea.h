#ifndef DRAWAREA_H
#define DRAWAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QQuickPaintedItem>

class DrawArea : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(int PenWidth READ penWidth WRITE setPenWidth)
    Q_PROPERTY(QColor PenColor READ penColor WRITE setPenColor)
    Q_ENUMS(DrawMode)

public:
    explicit DrawArea(QQuickItem *parent = 0);

    enum DrawMode {
        DA_FREEHAND,
        DA_LINE,
        DA_RECTANGLE,
        DA_CIRCLE,
        DA_POLYGON,
        _DA_COUNT
    };

    void paint(QPainter *painter);
    Q_INVOKABLE bool openImage(const QString &fileName);
    Q_INVOKABLE bool saveImage(const QString &fileName, const QString& fileFormat);
    Q_INVOKABLE int penWidth() { return mPenWidth; }
    Q_INVOKABLE QColor penColor() { return mPenColor; }

public slots:
    void clearImage();
    void setPenWidth(int newWidth);
    void setPenColor(const QColor &newColor);
    void setDrawMode(DrawMode mode);
    void mousePressEvent(QPoint pos);
    void mouseMoveEvent(QPoint pos);
    void mouseReleaseEvent(QPoint pos);

protected:
    virtual void geometryChanged(const QRectF &newGeometry,
                                 const QRectF &oldGeometry);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

private:
     void clearOverlayImage();
     void drawLineTo(const QPoint &endPoint, bool final);
     void drawRectangle(const QPoint &endPoint, bool final);
     void drawCircle(const QPoint &endPoint, bool final);
     void drawTo(const QPoint &point, bool final);
     void resizeImage(QImage *image, const QSize &newSize);
     QSize size() const;

private:
    bool     mModified;
    bool     mScribbling;
    DrawMode mDrawMode;
    int      mPenWidth;
    QColor   mPenColor;
    QImage   mImage;
    QImage   mOverlayImage;
    QPoint   mStartPoint;
    QRect    mCurrRect;
    QRect    mPrevRect;
};

#endif // DRAWAREA_H
