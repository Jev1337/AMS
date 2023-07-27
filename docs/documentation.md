# Documentation

## Making a Window Frameless Using CFramelessWindow Library:

1. In the mainwindow.h file, add the following code at the top of the file:
    ```
    #include "libraries/framelesswindow.h"
    ```
2. Change the class declaration to inherit from CFramelessWindow:
    ```
    class MainWindow : public CFramelessWindow
    //                        ^^^^^^^^^^^^^^^^
    {
        Q_OBJECT
    ```
3. In the mainwindow.cpp file, change the class constructor to inherit from CFramelessWindow:
    ```
    MainWindow::MainWindow(QSqlDatabase *db, QWidget *parent)
        : CFramelessWindow(parent)
    //    ^^^^^^^^^^^^^^^^^^^^^^^^
        , ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        this->db = db;
    }
    ```
4. Build and run the project to test the frameless window.
