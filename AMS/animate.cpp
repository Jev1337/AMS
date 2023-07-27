#include "animate.h"

animate::animate() {}

void updateText(QGraphicsOpacityEffect *effect, QLabel *label, QString text,
                int startValue, int endValue, int duration) {
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    label->setGraphicsEffect(effect);
    animation->setDuration(duration);
    animation->setStartValue(startValue);
    animation->setEndValue(endValue);
    animation->setEasingCurve(QEasingCurve::OutExpo);
    animation->start();
    QObject::connect(animation, &QPropertyAnimation::finished, [=]() {
        label->setText(text);
        QPropertyAnimation *animation =
            new QPropertyAnimation(effect, "opacity");
        animation->setDuration(duration);
        animation->setStartValue(endValue);
        animation->setEndValue(startValue);
        animation->setEasingCurve(QEasingCurve::OutExpo);
        animation->start();
    });
}

void updateText(QGraphicsOpacityEffect *effect, QLineEdit *label, QString text,
                int startValue, int endValue, int duration) {
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    label->setGraphicsEffect(effect);
    animation->setDuration(duration);
    animation->setStartValue(startValue);
    animation->setEndValue(endValue);
    animation->setEasingCurve(QEasingCurve::OutExpo);
    animation->start();
    QObject::connect(animation, &QPropertyAnimation::finished, [=]() {
        label->setText(text);
        QPropertyAnimation *animation =
            new QPropertyAnimation(effect, "opacity");
        animation->setDuration(duration);
        animation->setStartValue(endValue);
        animation->setEndValue(startValue);
        animation->setEasingCurve(QEasingCurve::OutExpo);
        animation->start();
    });
}

void updateText(QGraphicsOpacityEffect *effect, QPushButton *label,
                QString text, int startValue, int endValue, int duration) {
    QPropertyAnimation *animation = new QPropertyAnimation(effect, "opacity");
    label->setGraphicsEffect(effect);
    animation->setDuration(duration);
    animation->setStartValue(startValue);
    animation->setEndValue(endValue);
    animation->setEasingCurve(QEasingCurve::OutExpo);
    animation->start();
    QObject::connect(animation, &QPropertyAnimation::finished, [=]() {
        label->setText(text);
        QPropertyAnimation *animation =
            new QPropertyAnimation(effect, "opacity");
        animation->setDuration(duration);
        animation->setStartValue(endValue);
        animation->setEndValue(startValue);
        animation->setEasingCurve(QEasingCurve::OutExpo);
        animation->start();
    });
}

void updatePos(QWidget *widget, QPoint startValue, QPoint endValue,
               int duration) {
    QPropertyAnimation *animation = new QPropertyAnimation(widget, "pos");
    animation->setDuration(duration);
    animation->setStartValue(startValue);
    animation->setEndValue(endValue);
    animation->setEasingCurve(QEasingCurve::OutExpo);
    animation->start();
}
