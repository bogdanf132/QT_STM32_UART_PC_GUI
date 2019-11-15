#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QByteArray serialData;
    QString serialBuffer;
    int timebase;
    ~MainWindow();

private slots:
    void on_label_linkActivated(const QString &link);

    void on_label_2_windowIconTextChanged(const QString &iconText);

    void readSTM32UARTData(void);

    void makePlot(void);

    void realtimeDataSlot(double y);

    void on_dial_valueChanged(int value);

    void on_dial_2_valueChanged(int value);

    void on_connect_released();

    void on_COMPort_currentTextChanged(const QString &arg1);

    void on_baudRate_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    QSerialPort *STM32_UART = nullptr;
    QSerialPortInfo* STM32_UART_INFO = nullptr;
    QTimer* dataTimer = nullptr;
    QTextStream m_standardOutput;
    void enumerateUARTPorts(void);


};
#endif // MAINWINDOW_H
