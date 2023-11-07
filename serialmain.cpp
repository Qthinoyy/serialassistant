#include "serialmain.h"
#include "ui_serialmain.h"
#include <QDebug>
serialParameter serialparameter;
#define TEXTOUT_BLACK(QString) ("<font color=\"#000000\">" + QString + "</font>")

serialmain::serialmain(QWidget *parent) : QMainWindow(parent), ui(new Ui::serialmain) {
    ui->setupUi(this);

    subthread = new QThread;
    serialPort = new QSerialPort;
    s_datathread = new datathread(serialPort);
    serialPort->moveToThread(subthread);
    s_datathread->moveToThread(subthread);
    subthread->start();
    connect(ui->button_search, SIGNAL(clicked()), this, SLOT(cfg_searchPorts()), Qt::UniqueConnection);
    connect(ui->button_open, SIGNAL(clicked()), this, SLOT(cfg_openPort()), Qt::UniqueConnection);
    //    connect(ui->button_send, SIGNAL(clicked()), this, SLOT(fun_send()), Qt::UniqueConnection);
    connect(ui->button_receive, SIGNAL(clicked()), this, SLOT(fun_receive()), Qt::UniqueConnection);

    connect(this, SIGNAL(sig_receiveStart()), s_datathread, SLOT(publicProcess_serial_receiveStart()), Qt::UniqueConnection);
    connect(this, SIGNAL(sig_receiveStop()), s_datathread, SLOT(publicProcess_serial_receiveStop()), Qt::UniqueConnection);
    connect(this, SIGNAL(sig_sendSingle(QString)), s_datathread, SLOT(publicProcess_serial_sendSingle(QString)), Qt::UniqueConnection);
}

serialmain::~serialmain() {
    delete ui;
}
void serialmain::cfg_searchPorts() {
    ui->comboBox_port->clear();
    QList<QString> comList;
    foreach (const QSerialPortInfo &qspinfo, QSerialPortInfo::availablePorts()) { comList.append(qspinfo.portName()); }
    QCollator collator;
    collator.setNumericMode(true);                       // 数字模式
    std::sort(comList.begin(), comList.end(), collator); // 使用std::sort函数
    ui->comboBox_port->addItems(comList);
}
void serialmain::cfg_setConfig() {
    //判断是否有串口可用
    if (!ui->comboBox_port->currentText().isEmpty()) {
        QString portName = ui->comboBox_port->currentText();
        serialPort->setPortName(portName);
        // QSerialPort::Baud115200  和 115200 作用一致，可看QSerialPort::BaudRate定义
        serialPort->setBaudRate(ui->comboBox_serialBaud->currentText().toUInt());
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
    }
}
void serialmain::cfg_openPort() {
    switch ((int)serialparameter.openStatus) {
        case 0:
            //配置串口
            cfg_setConfig();
            //打开串口
            if (serialPort->open(QIODevice::ReadWrite) == false) {
                QMessageBox::information(NULL, "失败", "串口打开失败");
                serialparameter.openStatus = false;
                return;
            }
            //判断状态
            if (!serialPort->isOpen())
                return;
            ui->button_open->setText("关闭");
            //串口状态标志位
            serialparameter.openStatus = true;
            emit sig_receiveStart();
            break;
        case 1:
            //关闭串口
            serialPort->close();
            //判断状态
            if (serialPort->isOpen())
                return;
            ui->button_open->setText("打开");
            serialparameter.openStatus = false;
            //        serialPort->isOpen();
            emit sig_receiveStop();
            break;
        default:;
    }
#if LOGSWITCH
    qDebug() << "serialparameter.openStatus" << serialparameter.openStatus << serialPort->isOpen() << serialPort->baudRate()
             << serialPort->dataBits() << serialPort->stopBits() << serialPort->flowControl();
#endif
}

void serialmain::fun_send() {
    if (!serialparameter.openStatus)
        return;
    QString text = ui->lineEdit_sendSingle->text();
    qDebug() << "send";
    serialPort->write(text.toLatin1());
}
void serialmain::fun_receive() {
    ui->textBrowser->clear();
    if (!serialparameter.openStatus)
        return;
    qDebug() << "receive";
    QByteArray buffer = serialPort->readAll();
    ui->textBrowser->append(((buffer)));
}

void serialmain::on_button_send_clicked() {
    QString str = ui->lineEdit_sendSingle->text();
    emit sig_sendSingle(str);
}
