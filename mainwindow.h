//Written by Adham Nour

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCharts>
#include "process.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_AddProcessBtn_clicked();

    void on_RemoveProcessBtn_clicked();

    void on_SchedualSelector_currentIndexChanged(const QString &arg1);

    void on_DisplayBtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
