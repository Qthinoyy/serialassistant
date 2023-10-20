#include "serialmain.h"

#include "ui_serialmain.h"
serialParameter serialparameter;
serialmain::serialmain(QWidget* parent) : QMainWindow(parent), ui(new Ui::serialmain) {
    ui->setupUi(this);
    serialPort = new QSerialPort;
}

serialmain::~serialmain() { delete ui; }
void serialmain::cfg_searchPorts() {
    ui->comboBox_port->clear();
    QList<QString> comList;
    foreach (const QSerialPortInfo& qspinfo, QSerialPortInfo::availablePorts()) { comList.append(qspinfo.portName()); }
    QCollator collator;
    collator.setNumericMode(true);                        // 数字模式
    std::sort(comList.begin(), comList.end(), collator);  // 使用std::sort函数
    ui->comboBox_port->addItems(comList);
}
void serialmain::cfg_setConfig() {
    //判断是否有串口可用
    if (!ui->comboBox_port->currentText().isEmpty()) {
        QString portName = ui->comboBox_port->currentText();
        serialPort->setPortName(portName);
        serialPort->setBaudRate(ui->comboBox_serialBaud->currentText().toUInt());  // QSerialPort::Baud115200
        serialPort->setParity(QSerialPort::NoParity);
        serialPort->setDataBits(QSerialPort::Data8);
        serialPort->setStopBits(QSerialPort::OneStop);
        serialPort->setFlowControl(QSerialPort::NoFlowControl);
    }
}
void serialmain::cfg_openPort(int status) {
    cfg_setConfig();
    if (serialPort->open(QIODevice::ReadWrite) == false) {
        QMessageBox::information(NULL, "失败", "串口打开失败");
        serialparameter.openStatus = false;
        return;
    }
    serialparameter.openStatus = true;
}
void serialmain::fun_send() {}
void serialmain::fun_receive() {}
