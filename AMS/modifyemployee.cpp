#include "modifyemployee.h"
#include "ui_modifyemployee.h"

ModifyEmployee::ModifyEmployee(employee *e, QWidget *parent)
    : QDialog(parent), ui(new Ui::ModifyEmployee) {
    ui->setupUi(this);
    this->e = e;
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    e->getAccountTypes(ui->New_Type_2);
    ui->New_CIN_2->setText(e->getCIN());
    QDate date = QDate::fromString(e->getDateNaiss(), "yyyy-MMM-dd");
    ui->New_DOB_2->setDate(date);
    ui->New_Name_2->setText(e->getNom());
    ui->New_Type_2->setCurrentIndex(e->getType() - 1);
    ui->New_LastName_2->setText(e->getPrenom());
    ui->New_Email_3->setText(e->getMail());
    ui->textEdit->setText(e->getRemarks());
    ui->phone->setText(e->getPhone());
    qDebug() << e->getPhone();
}

ModifyEmployee::~ModifyEmployee() { delete ui; }

void ModifyEmployee::on_pushButton_2_released() {
    if (!CINValidator(ui->New_CIN_2->text())) {
        QMessageBox::warning(this, "Error", "CIN Inserted is Invalid!");
        return;
    }
    if(!CINValidator(ui->phone->text())){
        QMessageBox::warning(this, "Error", "Phone Inserted is Invalid!");
        return;
    }
    if (!NameValidator(ui->New_Name_2->text())) {
        QMessageBox::warning(
            this, "Error",
            "Your name must be 3 to 20 letters long and only alphabetic.");
        return;
    }
    if (!NameValidator(ui->New_LastName_2->text())) {
        QMessageBox::warning(
            this, "Error",
            "Your last name must be 3 to 20 letters long and only alphabetic.");
        return;
    }
    employee tst;
    if (ui->textEdit->toPlainText().length() > 255) {
        QMessageBox::warning(
            this, "Error",
            "Remarks is too long: Value cannot exceed 255 characters!");
        return;
    }
    e->setNom(ui->New_Name_2->text());
    e->setPrenom(ui->New_LastName_2->text());
    e->setDateNaiss(ui->New_DOB_2->date().toString("yyyy-MM-dd"));
    e->setType(ui->New_Type_2->currentIndex());
    e->setRemarks(ui->textEdit->toPlainText());
    e->setAdmin(!ui->checkBox_4->isChecked());
    e->setPhone(ui->phone->text());
    e->updateSelf();
    QMessageBox::information(
        this, "Info",
        "Account successfully updated! Please note that in order for the "
        "update to take changes, the user will have to log in again!");
    this->close();
}

void ModifyEmployee::on_pushButton_released() { this->close(); }

void ModifyEmployee::on_Face_ID_Btn_2_released() {
    auto *dialog = new faceid();
    if (dialog->exec() == QDialog::Accepted) {
        if (e->setFaceID(dialog->getResult()))
            QMessageBox::information(
                this, "Info", "Face ID has been successfully added/updated!");
    }
}

void ModifyEmployee::on_Face_ID_Btn_3_released()
{
    arduino A;
    if (A.connect_arduino(FINGERPRINT_PORT) != 0){
        QMessageBox::warning(this, "Error",
                             "Arduino Not Available...");
        return;
    }
    A.close_arduino();
    if (e->getCurrentFingerprint() == 0){
        if (e->GetLastFingerprintID() != 127) {
            auto *dialog = new fingerprint(e->GetLastFingerprintID() + 1);
            if (dialog->exec() == QDialog::Accepted) {
                e->setFingerprint(e->GetLastFingerprintID() + 1);
            } else {
                QMessageBox::warning(this, "Error", "Error");
            }
        } else
            QMessageBox::warning(this, "Error", "Reached Fingerprint Limit");
    }else{
        if (e->GetLastFingerprintID() != 127) {
            auto *dialog = new fingerprint(e->getCurrentFingerprint());
            if (dialog->exec() == QDialog::Accepted) {
                e->setFingerprint(e->getCurrentFingerprint());
            } else {
                QMessageBox::warning(this, "Error", "Error");
            }
        } else
            QMessageBox::warning(this, "Error", "Reached Fingerprint Limit");
    }
}

void ModifyEmployee::on_checkBox_4_toggled(bool checked)
{
    if (checked && ui->New_Type_2->currentIndex() != 1
            && ui->New_Type_2->currentIndex() != 4){
        QMessageBox::warning(this, "Error", "Incorrect Department: No User mode for this type");
        ui->checkBox_4->setChecked(false);
    }
}

void ModifyEmployee::on_New_Type_2_currentIndexChanged(int index)
{
    if (ui->checkBox_4->isChecked() && index != 1 && index != 4){
        QMessageBox::warning(this, "Error", "Incorrect Department: No User mode for this type");
        ui->checkBox_4->setChecked(false);
    }
}
