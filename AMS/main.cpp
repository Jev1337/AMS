#include "fingerprint.h"
#include "mainwindow.h"
#include <QApplication>
#include <QFileInfo>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QTextToSpeech>

int main(int argc, char *argv[]) {

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    int currentExitCode = 0;
    do {
    QApplication a(argc, argv);
    QNetworkAccessManager manager;
    manager.get(QNetworkRequest(QUrl("1.1.1.1")));


    ConnSQL conn("malek", "1337");

    MainWindow w;
    w.show();

    currentExitCode = a.exec();

    } while( currentExitCode == EXIT_CODE_REBOOT );

    return currentExitCode;
}

/*
// Check if libraries exist in current directory
    QFile ssleay32(QCoreApplication::applicationDirPath() + "/" +
"ssleay32.dll"); QFile libeay32(QCoreApplication::applicationDirPath() + "/" +
"libeay32.dll");

    if (!ssleay32.exists() || !libeay32.exists()) {
        QMessageBox::StandardButton reply = QMessageBox::question(nullptr,
"Missing Libraries", "One or more required libraries are missing. Would you like
to install them automatically?", QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            // Copy libraries to current directory
            QString currentPath = QCoreApplication::applicationDirPath();
            QString srcPath = currentPath + "../../../AMS/libraries/";
            QString destPath = QCoreApplication::applicationDirPath() + "/";

            if (QFile::copy(srcPath + "ssleay32.dll", destPath +
"ssleay32.dll")) { qDebug() << "ssleay32.dll copied successfully"; } else {
                QMessageBox::warning(nullptr, "Error", "Error copying libraries
to application directory."); return 0;
            }

            if (QFile::copy(srcPath + "libeay32.dll", destPath +
"libeay32.dll")) { qDebug() << "libeay32.dll copied successfully"; } else {
                QMessageBox::warning(nullptr, "Error", "Error copying libraries
to application directory."); return 0;
            }
            QMessageBox::information(nullptr, "Success", "Required DLLs has been
successfully installed. Please restart the applciation!"); return 0;
        }
    }
*/
