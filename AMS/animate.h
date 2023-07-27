#ifndef ANIMATE_H
#define ANIMATE_H
#include <QDebug>
#include <QGraphicsOpacityEffect>
#include <QLabel>
#include <QLineEdit>
#include <QPropertyAnimation>
#include <QPushButton>

class animate {
  public:
    animate();
};

void updateText(QGraphicsOpacityEffect *, QLabel *, QString, int, int, int);

void updateText(QGraphicsOpacityEffect *, QLineEdit *, QString, int, int, int);

void updateText(QGraphicsOpacityEffect *, QPushButton *, QString, int, int,
                int);

void updatePos(QWidget *, QPoint, QPoint, int);

#endif // ANIMATE_H
