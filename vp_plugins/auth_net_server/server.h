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
QT_END_NAMESPACE

using namespace VPrn;

class Server : public QDialog
{
    Q_OBJECT

public:
    Server(QWidget *parent = 0);
    void setVisible(bool visible);

protected:
    void closeEvent(QCloseEvent *event);
public slots:
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

    void messageClicked();
    void errorInfo(pluginsError eCode,QString e_msg);
    /**
      * @fn void setUserName(const QString & login,const QString &mandat);
      * @brief ���������� ���������� ���������� ������������
      */
    void setUserName(QString & login,QString &mandat);
private:
    void createIconGroupBox();
    void createMessageGroupBox();
    void createActions();
    void createTrayIcon();

    /**
      * @fn void loadPlugins()
      * @brief �������� ��������
      */
    void loadPlugins();

    /**
      * @fn void setTrayStatus(trayStatus t_stat,const QString & t_msg);
      * @brief ������������ ������ � ��������� � ����������� �� �������
      */

    void setTrayStatus (trayStatus t_stat, const QString &t_msg);
    /**
      * @var mainGear; �������� ������ ���������
      * @var myNet_plugin; ��������� �� ������� ������
      * @var myAuth_plugin; ��������� �� ������ ���������� (����� �� ?)
      * @var u_login; ������� ����� ������������
      * @var u_mandat; ������� ������ ������������
      * @var currentStatus ������� ��������� ���������
      */
    serverGears *myServerGears;
    Inet_plugin *myNet_plugin;
    Auth_plugin *myAuth_plugin;
    QString u_login;
    QString u_mandat;    
    QString currentStatus;

    QGroupBox *groupBox;
    QFormLayout *formLayout;
    QLabel *login_label;
    QLineEdit *lineEdit;
    QLabel *mandat_label;
    QLineEdit *lineEdit_2;
    QGroupBox *groupBox_2;
    QPlainTextEdit *demonStatePlainTextEdit;
    QPushButton *quitButton;
    QGridLayout *gridLayout;

    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

};

#endif
