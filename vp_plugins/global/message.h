#ifndef MESSAGE_H
#define MESSAGE_H
/**
  * @class Messeage ������ ����� ������������� � ���� ��������� ������������ ��
  * ������� � ������� � �������
  */
#include <QObject>

class Message : public QObject
{
Q_OBJECT
public:
    explicit Message(QObject *parent = 0);

signals:

public slots:

};

#endif // MESSAGE_H
