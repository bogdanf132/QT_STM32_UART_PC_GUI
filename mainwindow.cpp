#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serialBuffer = "";
    timebase = ui->dial_2->value();
    dataTimer = new QTimer;
    MainWindow::makePlot();

    STM32_UART = new QSerialPort;
    STM32_UART_INFO = new QSerialPortInfo;
    STM32_UART->setPortName("COM5");
    STM32_UART->setBaudRate(QSerialPort::Baud115200);
    STM32_UART->setDataBits(QSerialPort::Data8);
    STM32_UART->setParity(QSerialPort::NoParity);
    STM32_UART->setStopBits((QSerialPort::OneStop));
    STM32_UART->setFlowControl(QSerialPort::NoFlowControl);
    STM32_UART->open(QSerialPort::ReadWrite);
    QObject::connect(STM32_UART, SIGNAL(readyRead()), this, SLOT(readSTM32UARTData()));

}

MainWindow::~MainWindow()
{
    delete ui;
    if (STM32_UART->isOpen())
    {
        STM32_UART->close();
    }

}

void MainWindow::makePlot(void)
{
    ui->customPlot->addGraph(); // red line
    ui->customPlot->graph(0)->setPen(QPen(QColor(255, 110, 40)));
    ui->customPlot->axisRect()->setupFullAxesBox();
    ui->customPlot->yAxis->setRange(0, 512);
}


void MainWindow::readSTM32UARTData(void)
{
    static double y;

    serialData = STM32_UART->readAll();
    int i;
    if (!(serialData.size() % 5))
    {
        serialBuffer = QString::fromStdString(serialData.toStdString());
        QStringList bufferSplit = serialBuffer.split("*");

        for (i = 0; i <= bufferSplit.size() - 2; i++)
        {
            y = bufferSplit[i].toDouble();
            ui->label_2->setText(bufferSplit[i]);
            ui->lcdNumber->display(y);
            realtimeDataSlot(y);
        }
        ui->customPlot->replot();
    }
}

void MainWindow::realtimeDataSlot(double y)
{
    static double x = 0u;
    ui->customPlot->graph(0)->addData(x, y);
    x++;
    ui->customPlot->xAxis->setRange(x, timebase, Qt::AlignRight);
}


void MainWindow::on_label_linkActivated(const QString &link)
{

}

void MainWindow::on_label_2_windowIconTextChanged(const QString &iconText)
{

}

void MainWindow::on_dial_valueChanged(int value)
{
        ui->customPlot->yAxis->setRange(0, value);
}

void MainWindow::on_dial_2_valueChanged(int value)
{
    timebase = value;
}
