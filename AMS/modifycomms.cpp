#include "modifycomms.h"
#include "ui_modifycomms.h"

modifycomms::modifycomms(communication *c, QWidget *parent)
    : QDialog(parent), ui(new Ui::modifycomms) {
    ui->setupUi(this);
    this->c = c;
    ui->New_ID->setText(QString::number(c->getId()));
    ui->New_Name->setText(c->getNom());
    ui->New_Address->setText(c->getAdresse());
    ui->New_Phone->setText(c->getNum_tel());
    ui->textEdit->setText(c->getRemarques());
    ui->New_Email->setText(c->getEmail());
}

modifycomms::~modifycomms() { delete ui; }

void modifycomms::on_pushButton_2_released() {

    QString idString = ui->New_ID->text();
    bool ok;
    int id = idString.toInt(&ok);
    QString email = ui->New_Email->text();
    QString phone = ui->New_Phone->text();

    if (!ok) {
        QMessageBox::warning(nullptr, "Error",
                             "Error: ID must contain only numbers!");
        return;
    }
    if (idString.isEmpty()) {
        QMessageBox::warning(nullptr, "Error", "Error: Please input your id!");
        return;
    }
    // ID is valid, continue with your code
    if (ui->New_Name->text() == "") {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input your name!");
        return;
    }
    if (ui->New_Address->text() == "") {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input your adresse!");
        return;
    }
    if (email.isEmpty()) {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input your email!");
        return;
    }
    if (!email.contains('@') || !email.contains('.')) {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input a valid email address!");
        return;
    }
    if (phone.isEmpty()) {
        QMessageBox::warning(nullptr, "Error",
                             "Error: Please input your phone!");
        return;
    }
    for (const QChar &c : phone) {
        if (!c.isDigit()) {
            QMessageBox::warning(
                nullptr, "Error",
                "Error: Phone number must contain only numbers!");
            return;
        }
    }
    c->setID(id);
    c->setNom(ui->New_Name->text());
    c->setAdresse(ui->New_Address->text());
    c->setNum_tel(ui->New_Phone->text());
    c->setEmail(ui->New_Email->text());
    c->setRemarques(ui->textEdit->toPlainText());
    c->modifier();
    this->close();
}

void modifycomms::on_pushButton_released() { this->close(); }
