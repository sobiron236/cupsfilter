#ifndef SERVERCORE_H
#define SERVERCORE_H

#include <QObject>
#include "datamodule.h"
class ServerCore : public QObject
{
        Q_OBJECT
public:    
    explicit ServerCore(QObject *parent = 0);
    /**
      * @fn bool init();
      * @brief ���������� ��������� ������� ����� �� �� � ������ ��� ���
      */
    bool isReady(){ return state;}

private:
    bool state;
    DataModule *dm;

    QStringList t_FileList;
    /**
      * @fn bool loadPlugins();
      * @brief ��������� ��������� ��� ������ ������ �������
      */
    bool loadPlugins();

    /**
      * @fn bool loadSettings();
      * @brief ������ ini ���� � ����������� ����������
      */
    bool loadSettings();

};

#endif // SERVERCORE_H
