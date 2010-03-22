#ifndef SERVER_H
#define SERVER_H

#include "servergears.h"
#include "auth_plugin.h"
#include "inet_plugin.h"
#include "mytypes.h"

#include <QDialog>
#include <QtGui/QSystemTrayIcon>

QT_BEGIN_NAMESPACE
class QLabel;
class QPushButton;
class QGroupBox;
class QFormLayout;
class QLineEdit;
class QLocalServer;
class QComboBox;
class QMenu;
class QPlainTextEdit;
class QDialogButtonBox;
class QGridLayout;
class QCheckBox;
QT_END_NAMESPACE

using namespace VPrn;

class Server : public QDialog
{
    Q_OBJECT

public:
    Server(QWidget *parent = 0);
    void setVisible(bool visible);
    /**
      * @fn bool isReady();
      * @brief ��������� ������ GateKeeper ����� �� �� � ������ ��� ���
      */
    bool isReady(){return m_GateKeeperReady;};

    /**
      * @fn QString lastError();
      * @brief ���������� ��������� ��������� ������
      */
    QString lastError(){return m_lastError;};
    /**
      * @fn void showCriticalInfo(constQStrin & info)
      * @brief  ���������� ����������� ������ � ��������� ������ ���������
      */
    void showCriticalInfo(const QString & info);

protected:
    void closeEvent(QCloseEvent *event);
public slots:
    /**
       @fn void appendStartMsg(const QString & msg);
       @brief ������ ��� ��������� ��������� ��� ������ ������ ����� ��������� 
     */
    void appendStartMsg(const QString & msg);
    /**
      * @fn void showBaloonMessage(int m_Type,const QString &b_msg)
      * @brief ���������� ��������� � ����
      * @param m_Type ��� ��������� critical,warning,information
      * @param b_msg ���� ���������
      */
    //void showBaloonMessage(int m_Type,const QString &b_msg);
    /**
      * @fn void showTrayMessage(trayIcons msg_type,
      *                          const QString & msg_title,
      *                          const QString & msg_body);
      * @brief ���������� � ���� ��������� ��������� ����
      */
    void showTrayMessage(trayIcons msg_type,
                         const QString & msg_title,
                         const QString & msg_body);
private slots:

    void iconActivated(QSystemTrayIcon::ActivationReason reason);

    void errorInfo(pluginsError eCode,QString e_msg);
    /**
      * @fn void setUserName(const QString & login,const QString &mandat);
      * @brief ���������� ���������� ���������� ������������
      */
    void setUserName(QString & login,QString &mandat);
    /**
      * @fn void runTEditor()
      * @brief ������ ��������� �������� ��� ���������� ����������
      */
    void runTEditor();
    /**
      * @fn void do_ChekPointChanged(MyCheckPoints m_scheckPoint);
      * @brief ��� ������ ��������� ������ ����������, ��� ���� ����������
      */
    void do_ChekPointChanged(MyCheckPoints m_scheckPoint);
    /**
      * @fn net_connected(Message msg);
      * @brief �������� ������ ���� ������������ � ��������
      */
    void recive_message(const Message &msg);
private:

    void createActions();
    void createTrayIcon();

    /**
      * @fn void init();
      * @brief ���������� �������������� �������� ����������,
      */
    void init();

    /**
      * @fn bool loadPlugins()
      * @brief �������� ��������
      */
    bool loadPlugins();

    /**
      * @fn void setTrayStatus(trayStatus t_stat,const QString & t_msg);
      * @brief ������������ ������ � ��������� � ����������� �� �������
      */
    void setTrayStatus (trayStatus t_stat, const QString &t_msg);
    /**
      * @fn bool readConfig();
      * @brief ������ ini ����
      */
    bool readConfig();
    /**
      * @var mainGear; �������� ������ ���������
      * @var myNet_plugin; ��������� �� ������� ������
      * @var myAuth_plugin; ��������� �� ������ ���������� (����� �� ?)
      * @var u_login; ������� ����� ������������
      * @var u_mandat; ������� ������ ������������
      * @var currentStatus ������� ��������� ���������
      * @var m_GateKeeperReady; ���� ����� � ������ ��� ���
      */
    serverGears *myServerGears;
    Inet_plugin *myNet_plugin;
    Auth_plugin *myAuth_plugin;
    QString u_login;
    QString u_mandat;    
    QString currentStatus;
    bool m_GateKeeperReady;

    /**
      * @brief --------- ���� ���������� �� ini ����� ------------------------
      * @var serverHostName; ������� ��� ������ ��� IP �����
      * @var serverPort;     ���� ��� ����� c �������
      * @var localSrvName;   ��� ���������� �������
      * @var spoolDir;       ������� ��� ��������� ������
      * @var ticket_fname;   ��� ����� � ������� ��������� ��������� ����������� UNIX only
      * @var m_lastError; ��������� ��������� �� ������
      * @var tEditor_bin ������ ���� � ����� ��������� ��������
      *------------------------------------------------------------------------
      */
    QString serverHostName;
    int serverPort;
    QString localSrvName;
    QString spoolDir;
    QString ticket_fname;
    QString m_lastError;
    QString tEditor_bin;
    QString sid;

    // ����� ����������� ���������
    QGroupBox   *groupBox;
    QFormLayout *formLayout;
    QLabel      *login_label;
    QLineEdit   *login_LE;
    QLabel      *mandat_label;
    QLineEdit   *mandat_LE;
    QGroupBox   *groupBox_2;
    QLineEdit   *demonState_LE;
    QPushButton *quitButton;
    QGridLayout *gridLayout;
    QCheckBox   *authCheckBox;
    QCheckBox   *netCheckBox;


    // ��������
    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;
    QAction *runEditorAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
};

#endif
