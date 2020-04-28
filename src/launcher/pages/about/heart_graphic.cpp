#include "heart_graphic.h"

Heart_Graphic::Heart_Graphic() {
    setPos(mapToParent(350 / 2, 192 / 2));

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
    setScale(heart_scale_);

    QPen pen(heart_color_);
    painter->setPen(pen);

    QBrush brush;
    brush.setColor(heart_color_);
    brush.setStyle(Qt::SolidPattern);

    QPainterPath path;
    path.addPolygon(poly_);

    painter->rotate(180);

    painter->drawPolygon(poly_);
    painter->fillPath(path, brush);
}

void Heart_Graphic::advance(int phase) {
    if (!phase) return;

    if (heart_animation_direction_ == 1)
        heart_scale_ += .1 * phase;
    else
        heart_scale_ -= .05 * phase;

    if (heart_scale_ > 5)
        heart_animation_direction_ = 0;
    else if (heart_scale_ <= 3)
        heart_animation_direction_ = 1;
}
