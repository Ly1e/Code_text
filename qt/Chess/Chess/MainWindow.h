#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include "Board.h"
#include "CtrlPanel.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(int gameType,QWidget *parent = 0);
    ~MainWindow();

    int _gameType;

signals:

public slots:
};

#endif // MAINWINDOW_H
