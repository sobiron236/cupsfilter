#include "sqlmanager.h"
#include <QModelIndex>
#include <QStandardItem>
#include <QDataStream>
#include <QByteArray>
#include <QVariant>

sqlManager::sqlManager(QObject *parent)
        : QObject (parent)
        , m_model(0)
{

}

sqlManager::~sqlManager()
{

}

//--------------------------------- Public slots ---------------------------------------------
void sqlManager::convertModelToMsg ()
{
    if (m_model == 0){
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("������ �������� ��������� �� ����������!\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
        return;
    }

    if ( m_model->rowCount() == 0  || m_model->columnCount() == 0 ){
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("������ �������� ��������� �� ���������!\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
        return;
    }

    QByteArray m_data;
    QDataStream out(&m_data, QIODevice::WriteOnly );
    out.setVersion(QDataStream::Qt_3_0);

    for (int i=1; i < m_model->columnCount();i++){
        QModelIndex cellIndex = m_model->index(m_model->rowCount()-1,i);

        if (cellIndex.isValid()){
            QVariant cellData = m_model->data(cellIndex,Qt::EditRole);
            out << qint8 (i);            
            switch (i){
            case 1/*��� ���������*/:
                out << cellData.toString();
                break;
            case 2 /*����*/:
                out << cellData.toString();
                break;
            case 3 /*����� ��*/:
                out << cellData.toString();
                break;
            case 4 /*����� �������*/:
                out << cellData.toString();
                break;
            case 5 /*����� �������*/:
                out << cellData.toInt();
                break;
            case 6 /*����� ���*/:
                out << cellData.toInt();
                break;
            case 7 /*������� ���*/:
                out << cellData.toInt();
                break;
            case 8 /*��� �������*/: /// @todo ����������� �������� ������������� �������
                out << cellData.toString();
                break;
            case 9 /*�����������*/:
                out << cellData.toString();
                break;
            case 10 /*���������*/:
                out << cellData.toString();
                break;
            case 11 /*�������*/:
                out << cellData.toString();
                break;
            case 12 /*���.�����*/:
                out << cellData.toString();
                break;
            case 13 /*���� ������*/:
                out << cellData.toUInt(); //QDateTime::fromTime_t();
                break;
            case 14 /*����������_1*/:
                out << cellData.toString();
                break;
            case 15 /*����������_2*/:
                out << cellData.toString();
                break;
            case 16 /*����������_3*/:
                out << cellData.toString();
                break;
            case 17 /*����������_4*/:
                out << cellData.toString();
                break;
            case 18 /*����������_5*/:
                out << cellData.toString();
                break;
            }
        }
    }

    // ��������� ��������� Msg
    Message message;
    message.setType(VPrn::Que_SaveDocCardToBase);
    message.setMessageData (m_data);
    emit sendMessageToDemon( message );
}

void sqlManager::findDocCardInBase(const QString &mb, const QString &copy_number,int find_type)
{
    if ( mb.isEmpty()  || copy_number.isEmpty() ){
        emit error(VPrn::InternalAppError,
                   QObject::trUtf8("�� ������ ��������� ������ �������� � �� �����!\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
        return;
    }
    // ��������� ��������� ���������
    QByteArray m_data;
    QDataStream out(&m_data, QIODevice::WriteOnly );
    out.setVersion(QDataStream::Qt_3_0);
    out << mb;
    out << copy_number;
    out << find_type;
    // ��������� ��������� Msg
    Message message;
    message.setType(VPrn::Que_IS_MB_EXIST);
    message.setMessageData (m_data);
    emit sendMessageToDemon( message );
}
// ------------------------------ Private ------------------------------------------------------------

void sqlManager::setModel(QStandardItemModel* model)
{
    m_model = model;
}



/*
#include <QSqlError>
#include <QSqlDriver>

sqlManager::sqlManager(QObject *parent)
        :QObject (parent)
{
    QSqlDatabase db  = QSqlDatabase::addDatabase(QLatin1String("QSQLITE"),
                                                 QLatin1String("TEST_CONN"));
    if (db.driver()->lastError().type() != QSqlError::ConnectionError) {

        qDebug() << "SQLITE driver exist and loaded!" << Q_FUNC_INFO;
        emit error(VPrn::SQLQueryError,
                   QObject::trUtf8("SQLITE driver exist and loaded!\n%1")
                   .arg(QString(Q_FUNC_INFO))
                   );
    }

}

sqlManager::~sqlManager()
{
    QSqlDatabase::removeDatabase(QLatin1String("TEST_CONN"));
}

// ------------------------------ Private ------------------------------------------------------------

void sqlManager::DumpError (const QSqlError & lastError)
{
    QString e_str("Text %1\n dataBaseText %2\n driverText %3\n type %4\n number %5\n");
    e_str.arg( lastError.text(), lastError.databaseText(), lastError.driverText() )
            .arg( lastError.type(), lastError.number() );

    qDebug() << e_str << Q_FUNC_INFO;

    emit error(VPrn::SQLQueryError,
               QObject::trUtf8("������ ���������� sql ������� %1!\n%2")
               .arg(e_str)
               .arg(QString(Q_FUNC_INFO))
               );
}
*/
