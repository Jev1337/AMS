#ifndef MODIFYEMPLOYEE_H
#define MODIFYEMPLOYEE_H

#include "employee.h"
#include "faceid.h"
#include <QDialog>
#include "fingerprint.h"
#include "mainwindow.h"

namespace Ui {
class ModifyEmployee;
}

class ModifyEmployee : public QDialog {
    Q_OBJECT

  public:
    explicit ModifyEmployee(employee *e, QWidget *parent = nullptr);
    ~ModifyEmployee();

  private slots:
    void on_pushButton_2_released();

    void on_pushButton_released();

    void on_Face_ID_Btn_2_released();

    void on_Face_ID_Btn_3_released();

    void on_checkBox_4_toggled(bool checked);

    void on_New_Type_2_currentIndexChanged(int index);

private:
    Ui::ModifyEmployee *ui;
    employee *e;
};

#endif // MODIFYEMPLOYEE_H
