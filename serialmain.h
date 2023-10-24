#ifndef SERIALMAIN_H
#define SERIALMAIN_H

#include <QCollator>
#include <QMainWindow>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

typedef struct {
    bool openStatus;
} serialParameter;

QT_BEGIN_NAMESPACE
namespace Ui {
    class serialmain;
}
QT_END_NAMESPACE

class serialmain : public QMainWindow {
    Q_OBJECT

  public:
    serialmain(QWidget* parent = nullptr);
    ~serialmain();
  private slots:
    void cfg_searchPorts();  //查找串口
    void cfg_setConfig();    //配置参数
    void cfg_openPort();     //打开串口/关闭串口
    void fun_send();         //发送
    void fun_receive();      //接收
  private:
    Ui::serialmain* ui;
    QSerialPort* serialPort;
};
#endif  // SERIALMAIN_H
