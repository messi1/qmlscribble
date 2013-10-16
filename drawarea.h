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

public:
    explicit DrawArea(QQuickItem *parent = 0);

    void paint(QPainter *painter);
    Q_INVOKABLE bool openImage(const QString &fileName);
    Q_INVOKABLE bool saveImage(const QString fileName, QString fileFormat);
    Q_INVOKABLE void clearImage();

public slots:
    int penWidth();
    QColor penColor();
    void setPenWidth(int newWidth);
    void setPenColor(const QColor &newColor);
    void mousePressEvent(QPoint pos);
    void mouseMoveEvent(QPoint pos);
    void mouseReleaseEvent(QPoint pos);

protected:
    virtual void geometryChanged(const QRectF &newGeometry,
                                 const QRectF &oldGeometry);

private:
     void drawLineTo(const QPoint &endPoint);
     void resizeImage(QImage *image, const QSize &newSize);
     QSize size() const;

private:
    bool   mModified;
    bool   mScribbling;
    int    mPenWidth;
    QColor mPenColor;
    QImage mImage;
    QPoint mLastPoint;
};

#endif // DRAWAREA_H
