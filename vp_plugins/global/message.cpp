#include "message.h"
#include <QtCore/QDebug>
#include <QtCore/QMetaType>
#include <QtCore/QDataStream>


Message::Message(QObject *parent)
    : QObject(parent)
    , messageType( VPrn::NoMsgType )
    , msgData()
    , ready( false )

{

}

bool Message::isMessageReady()
{
    return ready;
}

void Message::setType( MessageType tp )
{
    messageType =  tp ;
}

MessageType Message::type() const
{
    return messageType;
}

void Message::setMessage( const QByteArray &msg )
{
    msgData = msg;
}

QByteArray Message::messageData() const
{
    return msgData;
}

void Message::clear()
{
    setType(VPrn::NoMsgType);
    msgData.clear();
}

QByteArray Message::createPacket() const
{
//    qDebug() << "createPacket(): before creating: \n"
//             << "messageData() "  << this->msgData
//             << "type() "         << this->type();

    QByteArray packet;
    QDataStream out(&packet, QIODevice::WriteOnly );

    out.setVersion(QDataStream::Qt_4_0);
    // ������� ������ ������ ������ ����, �� ������� ��� ���� 4 �����
    out << (qint32)0;

    //������� ��� ��������� � ���� ��������� � �����
    out << ( int ) this->type();
    out << this->msgData;
    // ������� �� ������ �����
    out.device()->seek(0);
    // �������� ������ ����� ������ � ������� �� �� ���� �����
    out << (qint32)(packet.size() - sizeof(qint32));
    int t_size = packet.size();
    //int t_type = ( int ) this->type();
    QByteArray t_arr = packet;

    qDebug() << "\ncreatePacket(): after creating "
             << " packet " << t_arr
             << " full packet size: " << t_size;


    return packet;
}

