#ifndef MODIFYAMBULANCE_H
#define MODIFYAMBULANCE_H

#include "ambulance.h"
#include <QDialog>

namespace Ui {
class Modifyambulance;
}

class Modifyambulance : public QDialog {
    Q_OBJECT

  public:
    explicit Modifyambulance(ambulance *e, QWidget *parent = nullptr);
    ~Modifyambulance();

  private slots:
    void on_pushButton_2_released();

    void on_pushButton_released();

  private:
    Ui::Modifyambulance *ui;
    ambulance *a;
};

#endif // MODIFYAMBULANCE_H
