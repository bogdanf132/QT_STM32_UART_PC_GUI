#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <qserialport.h>
#include <QDebug>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    STM32_UART = new QSerialPort;
    STM32_UART_INFO = new QSerialPortInfo;
    serialBuffer = "";
    timebase = ui->dial_2->value();
    dataTimer = new QTimer;
    MainWindow::makePlot();

    enumerateUARTPorts();

    ui->baudRate->addItem("1200");
    ui->baudRate->addItem("2400");
    ui->baudRate->addItem("4800");
    ui->baudRate->addItem("9600");
    ui->baudRate->addItem("14400");
    ui->baudRate->addItem("19200");
    ui->baudRate->addItem("38400");
    ui->baudRate->addItem("57600");
    ui->baudRate->addItem("115200");

    ui->dataBits->addItem("4");
    ui->dataBits->addItem("5");
    ui->dataBits->addItem("6");
    ui->dataBits->addItem("7");
    ui->dataBits->addItem("8");

    ui->parity->addItem("None");
    ui->parity->addItem("Even");
    ui->parity->addItem("Odd");
    ui->parity->addItem("Mark");
    ui->parity->addItem("Space");

    ui->stopBits->addItem("1");
    ui->stopBits->addItem("1.5");
    ui->stopBits->addItem("2");

    ui->flowControl->addItem("None");
    ui->flowControl->addItem("Xon/Xoff");
    ui->flowControl->addItem("Hardware");

    QObject::connect(STM32_UART, SIGNAL(readyRead()), this, SLOT(readSTM32UARTData()));
    //connect(STM32_UART, SIGNAL(readyRead()), this, SLOT(readSTM32UARTData()));
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

void MainWindow::enumerateUARTPorts(void)
{
    ui->COMPort->addItem("COM5");
    ui->COMPort->addItem("COM8");
}

void MainWindow::on_connect_released()
{

    STM32_UART->open(QSerialPort::ReadWrite);
    if (STM32_UART->isOpen())
    {
        ui->COMStateLabel->setText("COM opened");
    }
    else
    {
        ui->COMStateLabel->setText("Error openning COM port");
    }
}

void MainWindow::on_COMPort_currentTextChanged(const QString &arg1)
{
        STM32_UART->setPortName("COM9");
}

void MainWindow::on_baudRate_currentTextChanged(const QString &arg1)
{
    STM32_UART->setBaudRate(arg1.toInt());
    //STM32_UART->setBaudRate(QSerialPort::Baud115200);
}

void MainWindow::on_dataBits_currentTextChanged(const QString &arg1)
{
    STM32_UART->setDataBits(QSerialPort::Data8);
}

void MainWindow::on_parity_currentTextChanged(const QString &arg1)
{
    STM32_UART->setParity(QSerialPort::NoParity);

}

void MainWindow::on_stopBits_currentTextChanged(const QString &arg1)
{
     STM32_UART->setStopBits((QSerialPort::OneStop));
}

void MainWindow::on_checkBox_stateChanged(int arg1)
{

}

void MainWindow::on_flowControl_currentTextChanged(const QString &arg1)
{
        STM32_UART->setFlowControl(QSerialPort::NoFlowControl);
}

void MainWindow::on_disconnect_released()
{
    if (STM32_UART->isOpen())
    {
        STM32_UART->close();
        ui->COMStateLabel->setText("COM closed");
    }
    else
    {
        ui->COMStateLabel->setText("Nothing to close");
    }
}
