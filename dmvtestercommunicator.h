#ifndef DMVTESTERCOMMUNICATOR_H
#define DMVTESTERCOMMUNICATOR_H

#include <QObject>
#include <QTimer>
#include <QUdpSocket>
#include <QQueue>
#include <QDebug>

#define DEV_ADDR QHostAddress("192.168.0.234")
#define DEV_PORT 51812
#define DEV_TOUT 800

#define INDEX_VK_SET "7B"
#define INDEX_V_SWCH "7D"
#define INDEX_PV_BTN "7C"
#define INDEX_BR_VAL "46"


class DMVTesterCommunicator : public QObject
{
        Q_OBJECT
public:
    DMVTesterCommunicator();
    ~DMVTesterCommunicator();

    void enableVK();
    void disableVK();

    void enableTest();
    void disableTest();

    void speed0();
    void speed1();
    void speed2();

    void prevStep();
    void nextStep();

    void prevPattern();
    void nextPattern();

    void setBrightness(quint8 value);

private slots:
    void onReplyReady();
    void onError();

private:
    void sendCommand();


    QUdpSocket         m_socket;
    QQueue<QByteArray> m_queue;
    QTimer             m_replyTimer;
};

#endif // DMVTESTERCOMMUNICATOR_H
