#include "onebox.h"
#include <QPainter>

OneBox::OneBox(const QColor &color):brushColor(color)
{

}

QRectF OneBox::boundingRect() const
{
    qreal penwidth = 1;
    return  QRectF(-10 - penwidth/2, -10 - penwidth/2,
                   20 + penwidth, 20 + penwidth);
}

void OneBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
       painter->drawPixmap(-10, -10, 20, 20, QPixmap(":/box.gif"));
       painter->setBrush(brushColor);
       QColor penColor = brushColor ;
       penColor.setAlpha(220);
       painter->setPen(penColor);
       painter->drawRect(-10, -10, 20, 20);
}

QPainterPath OneBox::shape() const
{
    QPainterPath path;
    path.addRect(-9.5 , -9.5, 19, 19);
    return path;
}
