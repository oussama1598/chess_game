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

protected:
    void advance(int phase) override;

private:
    QPolygonF poly_;
    QColor heart_color_{221, 63, 63};
    int heart_animation_direction_{1};
    float heart_scale_{3};
};
