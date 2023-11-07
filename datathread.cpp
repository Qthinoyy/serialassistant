#include "datathread.h"

datathread::datathread(QSerialPort *ser_data, QObject *parent) : QObject(parent), serialPort(ser_data) {
}
void datathread::publicProcess_serial_receiveStart() {
    privateProcess_serial_receiveStart();
}
void datathread::publicProcess_serial_receiveStop() {
    privateProcess_serial_receiveStop();
}
void datathread::publicProcess_serial_sendSingle(QString str) {
    privateProcess_serial_sendSingle(str);
}
void datathread::privateProcess_serial_sendSingle(QString str) {
    serialPort->write(str.toLatin1());
    serialPort->write("\r\n");
    serialPort->write(str.toLatin1());
}
void datathread::privateProcess_serial_receiveStop() {
    serialReceiveTimer->stop();
    serialReceiveTimer->deleteLater();
    serialReceiveTimer = NULL;
}
void datathread::privateProcess_serial_receiveStart() {
    serialReceiveTimer = new QTimer;
    connect(serialReceiveTimer, &QTimer::timeout, this, &datathread::privateProcess_serial_getdata, Qt::UniqueConnection);
    serialReceiveTimer->setTimerType(Qt::PreciseTimer); // 设置精度保持
    serialReceiveTimer->setInterval(100);               // 取非10整数倍 减少资源读写冲突耗时
    serialReceiveTimer->start();
#if LOGSWITCH
    qDebug() << "serialReceiveTimer start" << serialReceiveTimer->isActive();
#endif
}
void datathread::privateProcess_serial_getdata() {
    QByteArray buffer = serialPort->readAll();
#if LOGSWITCH
    if (buffer.size())
        qDebug() << __FUNCTION__ << "buffer" << buffer;
#endif
    buffer = byteWaitAddToReadBuffer.append(buffer);
    byteWaitAddToReadBuffer.clear();
#if LOGSWITCH //以换行符区分 如为其他协议的自行修改
    if (buffer.contains("\r\n")) {
        if (!buffer.endsWith("\r\n")) {
            int index = buffer.lastIndexOf("\r\n");
            int length = buffer.size();
            qDebug() << length;
            byteWaitAddToReadBuffer = buffer.right(length - index - 2);
            buffer = buffer.left(index + 2);
        }
    } else {
        byteWaitAddToReadBuffer = buffer;
        buffer.clear();
    }
    if (buffer.size()) {
        //处理数据
        QList<QByteArray> list, list2;
        while (buffer.size()) {
            int index = buffer.indexOf("\r\n");
            int arraysize = buffer.size();
            list.append(buffer.left(index).toHex());
            list2.append(buffer.left(index));
            buffer = buffer.right(arraysize - index - 2);
        }
#if LOGSWITCH
        qDebug() << "receive data:" << list;
        qDebug() << "receive data2:" << list2;
#endif
    }
#endif
}
