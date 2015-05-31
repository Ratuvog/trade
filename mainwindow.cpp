#include "mainwindow.h"
#include "statisticcalculator.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->openFile, SIGNAL(clicked()), SLOT(openFile()));
    connect(ui->run, SIGNAL(clicked()), SLOT(recalc()));
    connect(ui->chance, SIGNAL(clicked()), SLOT(calcChance()));

    setupPlot();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupPlot()
{
    ui->plot->xAxis->setBasePen(QPen(Qt::white, 1));
    ui->plot->yAxis->setBasePen(QPen(Qt::white, 1));
    ui->plot->xAxis->setTickPen(QPen(Qt::white, 1));
    ui->plot->yAxis->setTickPen(QPen(Qt::white, 1));
    ui->plot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->plot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    ui->plot->xAxis->setTickLabelColor(Qt::white);
    ui->plot->yAxis->setTickLabelColor(Qt::white);
    ui->plot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->plot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    ui->plot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->plot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    ui->plot->xAxis->grid()->setSubGridVisible(true);
    ui->plot->yAxis->grid()->setSubGridVisible(true);
    ui->plot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->plot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    ui->plot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    ui->plot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    ui->plot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    ui->plot->axisRect()->setBackground(axisRectGradient);
}

void MainWindow::openFile()
{
    m_fileName = QFileDialog::getOpenFileName(this);
}

void MainWindow::recalc()
{
    ui->plot->clearGraphs();
    ui->plot->clearPlottables();

    QFile file(m_fileName);
    if (!file.open(QIODevice::ReadOnly))
        QMessageBox::critical(this, "Error", "File is not opened");

    QVector<double> values;
    QTextStream stream(&file);
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        QStringList fields = line.split(",");
        values.append(fields.at(5).toDouble());
    }

    file.close();

    StatisticCalculator calculator;
    calculator.setValues(values);
    calculator.setBlockSize(ui->blockSize->value());
    calculator.calc();
    calculator.setGroupSeparator(ui->separator->value());

    QMap<double, double> data = calculator.getGrouppedData();
    QVector<double> x = data.keys().toVector();
    QVector<double> y = data.values().toVector();

    QCPBars *bars = new QCPBars(ui->plot->xAxis, ui->plot->yAxis);
    ui->plot->addPlottable(bars);
    bars->setWidth((double)x.first()/(double)x.size());
    bars->setData(x, y);
    bars->setPen(Qt::NoPen);
    bars->setBrush(QColor(10, 100, 50));
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->plot->rescaleAxes();
    ui->plot->replot();
}

void MainWindow::calcChance()
{
    ui->plot->clearGraphs();
    ui->plot->clearPlottables();

    QFile file(m_fileName);
    if (!file.open(QIODevice::ReadOnly))
        QMessageBox::critical(this, "Error", "File is not opened");

    QVector<double> values;
    QTextStream stream(&file);
    while(!stream.atEnd()) {
        QString line = stream.readLine();
        QStringList fields = line.split(",");
        values.append(fields.at(5).toDouble());
    }

    file.close();

    StatisticCalculator calculator;
    calculator.setValues(values);
    calculator.setBlockSize(ui->blockSize->value());
    calculator.calc();
    calculator.setGroupSeparator(ui->separator->value());

    QMap<double, double> data = calculator.getGrouppedData();
    QVector<double> x = data.keys().toVector();
    QVector<double> y = data.values().toVector();

    double count = 0;
    for(int i = 0; i < y.size(); ++i) {
        count += y[i];
    }
    for(int i = 0; i < y.size(); ++i) {
        y[i] /= count;
        y[i] *= 100;
    }

    QCPBars *bars = new QCPBars(ui->plot->xAxis, ui->plot->yAxis);
    ui->plot->addPlottable(bars);
    bars->setWidth((double)x.first()/(double)x.size());
    bars->setData(x, y);
    bars->setPen(Qt::NoPen);
    bars->setBrush(QColor(10, 100, 50));
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->plot->rescaleAxes();
    ui->plot->replot();
}
