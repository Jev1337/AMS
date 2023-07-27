#ifndef USERSELECTION_H
#define USERSELECTION_H

#include "employee.h"
#include <QDialog>
#include <QMessageBox>

namespace Ui {
class userSelection;
}

class userSelection : public QDialog {
    Q_OBJECT

  public:
    explicit userSelection(employee &e, QWidget *parent = nullptr);
    ~userSelection();
    int getResult();

  private slots:
    void on_pushButton_2_released();

    void on_pushButton_released();

  private:
    Ui::userSelection *ui;
    employee e;
};

#endif // USERSELECTION_H
