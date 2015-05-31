#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupPlot();

public slots:
    void openFile();
    void recalc();
    void calcChance();

private:
    Ui::MainWindow *ui;
    QString m_fileName;
};

#endif // MAINWINDOW_H
