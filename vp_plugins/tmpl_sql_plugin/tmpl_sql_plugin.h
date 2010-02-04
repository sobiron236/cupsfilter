#ifndef TMPL_SQL_PLUGIN_H
#define TMPL_SQL_PLUGIN_H

#include <QObject>
#include <QSqlDatabase>


class QSqlQuery;
class QSqlQueryModel;
class QSqlRelationalTableModel;
#include "itmpl_sql_plugin.h"

//#include "tech_global.h"

using namespace VirtualPrninterPlugins;

class Tmpl_sql_plugin : public QObject , Itmpl_sql_plugin
{
    Q_OBJECT
    Q_INTERFACES(Itmpl_sql_plugin)


public:

    Tmpl_sql_plugin(QObject *parent = 0);
    ~Tmpl_sql_plugin();

    /**
      *@brief �������� ������� �������� ���:
      * ���� ������� �� �������� � ������������ ����������,
      * �� ������� �������� ���� ��� ��
      */

    bool openTemplates(const QString & t_fileName);
    //QStringList customFilters() const;

    void closeTemplates();	
    bool isLoad();
    bool hasError();


signals:
    void error(pluginsError errCode,QString error_message);
public slots:
    bool createEmptyTemplate(const QString & t_fileName);
private:
    inline bool isDBOpened(){return m_dbOpened;};
    inline bool isDBConnected(){return m_dbConnect;};

    bool createTables(QSqlQuery *query);

    bool m_dbOpened;
    bool m_dbConnect;

    QString m_connectionName;
    //QSqlQuery m_query;

    /// ������ ����� �������� � �������
    //QSqlRelationalTableModel * tInfoModel;
    QSqlQueryModel  * tInfoModel;

    /**
      * @brief Singleton DB connection
      */
    QSqlDatabase DB_;
protected:
    /**
      @brief ������� ������ �� � ����������� �� ���������
      */
    bool create_emptyDB(const QString &emptyDB_fileName);
    /**
      @brief ��������� ������� � ������������� ���������� � �� sqlite
      */
    bool createConnection();
    /**
      * @brief ��������� ������, ��� �������������� ����������
      * � ����� ��������� ��� ��
      * ��������� ������ � �������� �� � �������������� ������
      */
    void setupModel();

    void DumpError (const QSqlError& lastError);

    /**
      * @brief ��������������� ��������� ��
      */
    bool InitDB ();
    /**
      * @brief ��������� ����������� �������� ����� �� ���������� ����
      * 1.�������� ��� ������ ���� �� ����������.
      * ���� ���� ���� �� �����, �� ������� ��� � �����.
      * (��� ��� ������������ ��� ���������� � ������� ������ ����� �����,
      *  ������ ���������� ���)
      * 2.������� � ������� ���� �� ���������� ����
      */
    bool IsValidFileName(const QString & fileName);
};


#endif

