#ifndef MESSAGE_H
#define MESSAGE_H
/**
  * @class Messeage ������ ����� ������������� � ���� ��������� ������������ ��
  * ������� � ������� � �������
  */
#include "mytypes.h"

#include <QObject>
#include <QtCore/QByteArray>

using namespace VPrn;



class Message : public QObject
{
    Q_OBJECT
    Q_ENUMS   ( MessageType )
    Q_PROPERTY( MessageType type READ type WRITE setType SCRIPTABLE true USER true )
    //Q_PROPERTY( int parts READ parts WRITE setNumberOfParts SCRIPTABLE false USER true )
    Q_PROPERTY( QByteArray messageData READ messageData SCRIPTABLE true USER true )

public:
    explicit Message(QObject *parent = 0);
    /**
      * @fn void clear();
      * @brief ������� ���������
      */
    void clear();
    /**
      * @brief ������� � ���������� ����� ������ ������� ��� ��������
      * @param msg ��������� ���������� � �����
      * @param type ��� ��������� � ������
      * @return QByteArray
      */
     QByteArray createPacket() const;

    /**
      * @fn bool isMessageReady();
      * @brief ���������� TRUE ���� ��������� ����� ��� �������� ����� FALSE
      * @return bool
      */
    bool isMessageReady();

    /**
      * @fn void prepareMessage();
      * @brief ���������� ��������� �����  ���������
      * @internal
     */
    void prepareMessage();
    /**
      * @fn void setMessage( const QByteArray &msg );
      * @param msg const QByteArray &
      * @brief ������ msg �� ���������� ��������� � ������ ��� ��� ���������
      */       
    void setMessage( const QByteArray &msg );

    /**
     * @brief messageData ��������: ���������� ������ �������������� ��� ���������
     * @return QByteArray
     */
    QByteArray messageData() const;

    /**
     * @brief type property: Getter
     * @return MessageType
     */
    MessageType type() const;

    /**
     * @brief type property: Setter
     * @param tp MessageType
     */
    void setType( MessageType tp );

signals:

public slots:

private:
    MessageType messageType;
    QByteArray msgData;
    bool ready;

};

#endif // MESSAGE_H
