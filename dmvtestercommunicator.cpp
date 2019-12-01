#include "dmvtestercommunicator.h"

DMVTesterCommunicator::DMVTesterCommunicator()
{
    m_socket.bind();
    m_socket.connectToHost(DEV_ADDR, DEV_PORT);
    QObject::connect(&m_socket, &QIODevice::readyRead    ,
                     this     , &DMVTesterCommunicator::onReplyReady);
    QObject::connect(&m_socket, &QAbstractSocket::disconnected,
                     this     , &DMVTesterCommunicator::onError          );
    QObject::connect(&m_socket, QOverload<QAbstractSocket::SocketError>::of
                                (&QAbstractSocket::error),
                     this     , &DMVTesterCommunicator::onError   );

    m_replyTimer.setSingleShot(true);
    m_replyTimer.setInterval(DEV_TOUT);
    QObject::connect(&m_replyTimer, &QTimer::timeout    ,
                     this         , &DMVTesterCommunicator::onError);
}

DMVTesterCommunicator::~DMVTesterCommunicator()
{
    m_socket.disconnect();
}

void DMVTesterCommunicator::enableVK()
{
    const bool exec = m_queue.isEmpty();
    m_queue.append(QByteArray::fromHex("19C505000000000000070004"
                                       INDEX_VK_SET "0100FF0269"));
    if (exec) sendCommand();
}

void DMVTesterCommunicator::disableVK()
{
    const bool exec = m_queue.isEmpty();
    m_queue.append(QByteArray::fromHex("19C505000000000000070004"
                                       INDEX_VK_SET "0000FF0268"));
    if (exec) sendCommand();
}

void DMVTesterCommunicator::enableTest()
{
    const bool exec = m_queue.isEmpty();
    m_queue.append(QByteArray::fromHex("19C505000000000000070003"
                                       INDEX_V_SWCH "01FF026A"));
    if (exec) sendCommand();

}

void DMVTesterCommunicator::disableTest()
{
    const bool exec = m_queue.isEmpty();
    m_queue.append(QByteArray::fromHex("19C505000000000000070003"
                                       INDEX_V_SWCH "00FF0269"));
    if (exec) sendCommand();
}

void DMVTesterCommunicator::speed0()
{
    const bool exec = m_queue.isEmpty();
    m_queue.append(QByteArray::fromHex("19C505000000000000070003"
                                       INDEX_PV_BTN "04FF026C"));
    if (exec) sendCommand();
}

void DMVTesterCommunicator::speed1()
{
    const bool exec = m_queue.isEmpty();
    m_queue.append(QByteArray::fromHex("19C505000000000000070003"
                                       INDEX_PV_BTN "08FF0270"));
    if (exec) sendCommand();
}

void DMVTesterCommunicator::speed2()
{
    const bool exec = m_queue.isEmpty();
    m_queue.append(QByteArray::fromHex("19C505000000000000070003"
                                       INDEX_PV_BTN "10FF0278"));
    if (exec) sendCommand();
}

void DMVTesterCommunicator::prevStep()
{
    const bool exec = m_queue.isEmpty();
    m_queue.append(QByteArray::fromHex("19C505000000000000070003"
                                       INDEX_PV_BTN "40FF02A8"));
    if (exec) sendCommand();
}

void DMVTesterCommunicator::nextStep()
{
    const bool exec = m_queue.isEmpty();
    m_queue.append(QByteArray::fromHex("19C505000000000000070003"
                                       INDEX_PV_BTN "20FF0288"));
    if (exec) sendCommand();
}

void DMVTesterCommunicator::prevPattern()
{
    const bool exec = m_queue.isEmpty();
    m_queue.append(QByteArray::fromHex("19C505000000000000070003"
                                       INDEX_PV_BTN "02FF026A"));
    if (exec) sendCommand();
}

void DMVTesterCommunicator::nextPattern()
{
    const bool exec = m_queue.isEmpty();
    m_queue.append(QByteArray::fromHex("19C505000000000000070003"
                                       INDEX_PV_BTN "01FF0269"));
    if (exec) sendCommand();
}

void DMVTesterCommunicator::setBrightness(quint8 value)
{
    const bool exec = m_queue.isEmpty();
    const quint16 check = 0x0232 + value;
    const QString hexs( "19C505000000000000070003" INDEX_BR_VAL
                      + QString::number(value, 16).rightJustified(2, '0') + "ff"
                      + QString::number(check, 16).rightJustified(4, '0'));
    m_queue.append(QByteArray::fromHex(hexs.toLatin1()));
    if (exec) sendCommand();
}

void DMVTesterCommunicator::sendCommand()
{
    if (m_queue.isEmpty()) return;
    m_socket.write(m_queue.dequeue());
    m_replyTimer.start();
}

void DMVTesterCommunicator::onReplyReady()
{
    m_replyTimer.stop();
    const QByteArray reply(m_socket.readAll());
    if (    reply.length() != 19
        ||  (quint8)reply.at(9) != 134
        ||  reply.at(12))
        { onError(); return; }

    if (!m_queue.isEmpty()) sendCommand();
}

void DMVTesterCommunicator::onError()
{
    qDebug() << "COMM ERROR";
    m_replyTimer.stop();
    m_queue.clear();
}
