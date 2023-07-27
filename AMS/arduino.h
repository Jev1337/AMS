#ifndef ARDUINO_H
#define ARDUINO_H
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

class arduino {
  public: // méthodes de la classe Arduino
    arduino();
    ~arduino();
    int connect_arduino();            // permet de connecter le PC à Arduino
    int close_arduino();              // permet de femer la connexion
    int write_to_arduino(QByteArray); // envoyer des données vers arduino
    QByteArray read_from_arduino(); // recevoir des données de la carte Arduino
    QSerialPort *getserial();       // accesseur
    QString getarduino_port_name();
    bool isOpen();
    int connect_arduino(QString port);

  private:
    QSerialPort *serial; // Cet objet rassemble des informations (vitesse, bits
                         // de données, etc.)
    // et des fonctions (envoi, lecture de réception,…) sur ce qu’est une voie
    // série pour Arduino.
    QString arduino_port_name;
    static const quint16 arduino_uno_vendor_id = 9025;
    static const quint16 arduino_uno_producy_id = 67;
    bool arduino_is_available;
    QByteArray data; // contenant les données lues à partir d'Arduino

};

#endif // ARDUINO_H
