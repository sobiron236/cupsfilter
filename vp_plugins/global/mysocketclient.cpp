#include "mysocketclient.h"
#include "message.h"


mySocketClient::mySocketClient(QLocalSocket *parent)
        : packetSize(-1)
        , m_chekpoints(VPrn::glob_Init)
        , e_info(QString())
{
    connect(this,SIGNAL(connected()),
            this,SLOT(do_connected())
            );
    connect(this,SIGNAL(readyRead()),
            this,SLOT(readyRead()));
    connect(this,SIGNAL(error(QLocalSocket::LocalSocketError)),
            this,SLOT(do_error(QLocalSocket::LocalSocketError))
            );
}


void mySocketClient::readyRead()
{
    //QLocalSocket *client = (QLocalSocket*)sender();
    //QString clientName = clients_uuid[client];

    //������ ����� � �����
    QDataStream in ( this );
    in.setVersion(QDataStream::Qt_4_0);

    while (this->bytesAvailable() > 0){
        if (packetSize == -1) {
            //��������� ���������� ���� ��������� ��� ������;
            //�� ���� ���� ���������� �������� ������ 4-� ����
            if( this->bytesAvailable() < sizeof(qint32) ){
                return;
            }
            //������ ������ ������
            in >> packetSize;
        }
        //�������� ��� � ����� ������ ���� ����� � �� ��� �������
        if (this->bytesAvailable() < packetSize){
            return;
        }
        //������� ������ ������, ��� ��������� ����������
        packetSize = -1;

        //������� � �������� ������ ���������
        qint32 m_format;
        in >> m_format;
        if( m_format != format ) {
            setError(tr("������ � ������� ���������, ��� ������ ������� � �������� %1")
                     .arg(this->serverName()));
            return;
        }

        // ������� ��� ���������
        int m_Type;
        in >> m_Type;

        //������� ���� ���������
        QByteArray msg;
        in >> msg;
        Message message( this );
        message.setType((MessageType) m_Type); //��������� ��� ������������
        message.setMessage( msg );        
        // �������� ���������
        messageReady( message );
    }
}

void mySocketClient::do_connected()
{
    setCheckPoint(VPrn::loc_Connected);
    //������ �����������
    Message msg(this);
    msg.setType(VPrn::Que_Register);
    this->sendMessage(msg);

}

void mySocketClient::setCheckPoint(MyCheckPoints m_cpoint)
{
    m_chekpoints = m_cpoint;
    emit checkPointChanged(m_chekpoints);
}

void mySocketClient::setError(const QString &info)
{
    e_info = info;
    setCheckPoint(VPrn::glob_Error);
}

void mySocketClient::sendMessage(const Message &msg)
{
    if (this->state() == QLocalSocket::ConnectedState){
        this->write(msg.createPacket());
        this->flush();
    }else{
        setError(tr("������� ������ ������ � �������� �����!"));
    }
}

void mySocketClient:: do_error(QLocalSocket::LocalSocketError r_error)
{
    switch(r_error){
    case QLocalSocket::ServerNotFoundError:
        setCheckPoint(VPrn::loc_ServerNotFound);
        break;
    default:
        setError(tr("��� ������ � % ��������� ������ %2")
                 .arg(this->serverName(),this->errorString()));
        break;
    }
}
