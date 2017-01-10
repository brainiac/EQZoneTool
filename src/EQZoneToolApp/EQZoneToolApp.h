#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_EQZoneToolApp.h"

class EQZoneToolApp : public QMainWindow
{
    Q_OBJECT

public:
    EQZoneToolApp(QWidget *parent = Q_NULLPTR);

private:
    Ui::EQZoneToolAppClass ui;
};
