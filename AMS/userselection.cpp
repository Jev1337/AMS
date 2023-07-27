#include "userselection.h"
#include "ui_userselection.h"

userSelection::userSelection(employee &e, QWidget *parent)
    : QDialog(parent), ui(new Ui::userSelection) {
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    this->e = e;
    this->e.getDeps(this->e.getType(), ui->New_Type_2);
}

userSelection::~userSelection() { delete ui; }

void userSelection::on_pushButton_2_released() {
    if (ui->New_Type_2->currentIndex() == -1) {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please select a correct ID");
        return;
    }
    this->accept();
}

void userSelection::on_pushButton_released() { this->reject(); }

int userSelection::getResult() { return ui->New_Type_2->currentText().toInt(); }
