#ifndef MYSOCKETCLIENT_H
#define MYSOCKETCLIENT_H
/**
  * @class ������� ������ �������� ������, �������� ������� ������� ��� ����������
  * � ����������� ������������������ �������� ����������� �� \sa QLocalSocket
  * ��� �� \sa QTcpSocket. ������� ��������� ���������� �������:
  * \fn onConnect()    ����������� ��������� ������� connected
  * \fn parseMessage() ����������� ��������� ��������� ����������� ���������
  * ���� \sa \class Message.
  */
#include <QObject>

#include <QtNetwork/QLocalSocket>
#include "message.h"

#include "mytypes.h"
using namespace VPrn;


class mySocketClient : public QLocalSocket
{
    Q_OBJECT
public:


    explicit mySocketClient(QLocalSocket *parent = 0);
    QString lastError() const {return e_info;};    
    void sendMessage(const Message &msg);

signals:
    void messageReady( const Message &msg );
    void checkPointChanged(MyCheckPoints state);

private slots:
    /**
      * @fn void readyRead()
      * @brief ������ �������� ������, �������, ����������� � ��������� Message
      */
    void readyRead();
    void do_connected();
    void do_error(QLocalSocket::LocalSocketError r_error);
private:
    /**
      * @var packetSize; ������ ������������� ����� ������
      * @var m_state;    ������ �������
      * @var e_info;     ��������� ��������� ������
      * @var client;     ����������� �������
      */

    qint32 packetSize;
    MyCheckPoints m_chekpoints;
    QString e_info;
    /**
      * @fn setError(const QString &info);
      * @brief ������������� ��������� ������,  ���������� ����� �������� ������
      */
    void setError(const QString &info);
    /**
      * @fn void setMyState(socketState state);
      * @brief ������ ��������� � ��������� �������������� ������
      */
    void setCheckPoint(MyCheckPoints m_cpoint);
};


#endif // MYSOCKETCLIENT_H
