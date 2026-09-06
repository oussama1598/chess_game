#include "heart_graphic.h"

Heart_Graphic::Heart_Graphic() {
    setPos(mapToParent(350 / 2, 192 / 2));
    setScale(2.f);

    int num_points = 100;
    double dt = (2 * M_PI / num_points);

    for (double t = dt; t <= 2 * M_PI; t += dt) {
        double x = 16.0 * pow(sin(t), 3);
        double y = 13.0 * cos(t) - 5 * cos(2 * t) - 2 * cos(3 * t) - cos(4 * t);

        poly_ << QPointF(x, y);
    }
}

QRectF Heart_Graphic::boundingRect() const {
    return QRect(-50, -50, 200, 200);
}

void
Heart_Graphic::paint(QPainter *painter, [[maybe_unused]] const QStyleOptionGraphicsItem *option,
                     [[maybe_unused]] QWidget *widget) {
    painter->setRenderHint(QPainter::Antialiasing);
    painter->setPen(QPen(QColor(203, 155, 86, 224), 1.35));
    painter->setBrush(QColor(61, 178, 151, 28));

    QPainterPath path;
    path.addPolygon(poly_);
    path.closeSubpath();

    painter->rotate(180);
    painter->drawPath(path);
}
