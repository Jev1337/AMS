#ifndef MODIFYCOMMS_H
#define MODIFYCOMMS_H

#include "communication.h"
#include <QDialog>
namespace Ui {
class modifycomms;
}

class modifycomms : public QDialog {
    Q_OBJECT

  public:
    explicit modifycomms(communication *c, QWidget *parent = nullptr);
    ~modifycomms();

  private slots:
    void on_pushButton_2_released();

    void on_pushButton_released();

  private:
    Ui::modifycomms *ui;
    communication *c;
};

#endif // MODIFYCOMMS_H
