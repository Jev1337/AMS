#include "modifyambulance.h"
#include "ui_modifyambulance.h"

Modifyambulance::Modifyambulance(ambulance *e, QWidget *parent)
    : QDialog(parent), ui(new Ui::Modifyambulance) {
    ui->setupUi(this);
    this->a = e;
    ui->matricule->setText(a->get_matricule());
    ui->state->setText(a->get_etat());
    ui->nb_emp->setText(QString::number(a->get_capacite()));
    ui->location->setText(a->get_emplacement());
    ui->equipment->setText(a->get_equipements());
    ui->Remarks->setText(a->get_remarques());
}

Modifyambulance::~Modifyambulance() { delete ui; }

void Modifyambulance::on_pushButton_2_released() {
    QString Emp_nb = ui->nb_emp->text();
    QString Loc = ui->location->text();
    QString state = ui->state->text();

    if (ui->matricule->text() == "") {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input the registration number!");
        return;
    }
    if (state.isEmpty()) {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input the state of the ambulance!");
        return;
    }
    for (const QChar &c : state) {
        if (!c.isDigit()) {
            QMessageBox::warning(nullptr, "Error",
                                 "Error: State must be a number!");
            return;
        }
    }
    if (state != "1" && state != "2" && state != "0") {
        QMessageBox::warning(nullptr, "Error",
                             "Error: State must be 0 or 1 or 2 !");
        return;
    }
    if (Emp_nb.isEmpty()) {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input the number of employees!");
        return;
    }
    for (const QChar &c : Emp_nb) {
        if (!c.isDigit()) {
            QMessageBox::warning(
                nullptr, "Error",
                "Error: The number of Employees must be a number!");
            return;
        }
    }
    if (Loc.isEmpty()) {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input the location !");
        return;
    }
    a->set_matricule(ui->matricule->text());
    a->set_etat(ui->state->text());
    a->set_capacite(ui->nb_emp->text().toInt());
    a->set_emplacement(ui->location->text());
    a->set_equipements(ui->equipment->text());
    a->set_remarques(ui->Remarks->toPlainText());
    a->modifier();
    this->close();
}

void Modifyambulance::on_pushButton_released() { this->close(); }
