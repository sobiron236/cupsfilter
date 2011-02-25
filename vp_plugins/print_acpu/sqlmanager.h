#ifndef SQLMANAGER_H
#define SQLMANAGER_H

#include <QObject>
//#include <QtSql/QSqlDatabase>
#include <QStandardItemModel>

#include "message.h"
#include "mytypes.h"
using namespace VPrn;

class sqlManager : public QObject
{
    Q_OBJECT
public:
    explicit sqlManager(QObject *parent = 0);
    ~sqlManager();
    void setModel ( QStandardItemModel* model = 0 );

public slots:
    /**
      * @fn void convertModelToMsg (const QString &cuuid );
      * @brief ����������� �������� ���������, ����������� � ������ @sa sqlManager::m_model
      * � ��������� ���� @sa Message
      */
    void convertModelToMsg();
    /**
      * @fn void findDocCardInBase(const QString &mb, const QString &copy_number);
      * @brief ����� � �� ����� �������� � ����������
      * @param QString mb          ����� ��
      * @param QString copy_number ����� ����������
      * @param int     find_type   ����� ������ (������� �����)
      * 0 - ��������� � ��������
      * 1 - ��������� � ����
      * 2 - �����
      */
    void findDocCardInBase(const QString &mb, const QString &copy_number,int find_type);

signals:
    void error(VPrn::AppErrorType errCode,const QString &error_message);
    /**
      * @fn void sendMessageToDemon( const Message &msg );
      * @brief ���������������� ��������� ��� �������� ���������� ������
      */
    void sendMessageToDemon( const Message &msg );

private:
    QStandardItemModel* m_model;
    //  void DumpError (const QSqlError & lastError);
};

#endif // SQLMANAGER_H
