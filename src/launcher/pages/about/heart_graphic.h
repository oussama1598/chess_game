#include <cmath>
#include "QGraphicsScene"
#include "QGraphicsItem"
#include "QPainter"
#include "QWidget"

#include <iostream>

class Heart_Graphic : public QGraphicsItem {
public:
    Heart_Graphic();

    [[nodiscard]] QRectF boundingRect() const override;

    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget) override;

private:
    QPolygonF poly_;
};
