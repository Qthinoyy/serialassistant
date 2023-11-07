#ifndef DATATHREAD_H
#define DATATHREAD_H

#include <QDebug>
#include <QObject>
#include <QQueue>
#include <QSerialPort>
#include <QTimer>
#define LOGSWITCH 1
class datathread : public QObject {
    Q_OBJECT
    public:
    explicit datathread(QSerialPort *ser_data, QObject *parent = nullptr);

    public slots:
    void publicProcess_serial_receiveStart();
    void publicProcess_serial_receiveStop();
    void publicProcess_serial_sendSingle(QString);

    private slots:
    void privateProcess_serial_receiveStart();
    void privateProcess_serial_receiveStop();
    void privateProcess_serial_sendSingle(QString);
    void privateProcess_serial_getdata();

    private:
    QTimer *serialReceiveTimer;
    QSerialPort *serialPort;
    QByteArray byteWaitAddToReadBuffer;              //存储临时buffer
    QQueue<QList<QByteArray>> serialByteArray_queue; // 串口数据队列
    QList<QByteArray> serialByteArrayWatiEnQueue;    // 进队的串口数据
};

#endif // DATATHREAD_H
