#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QDebug>
#include <QMainWindow>
#include <QUndoStack>
#include <QUndoGroup>
#include <QUndoView>


QT_BEGIN_NAMESPACE
class QAction;
class QListWidget;
class QMenu;
class QTextEdit;
class QStringListModel;
class QStandardItemModel;
class QSqlQueryModel;
QT_END_NAMESPACE

class View;
class cmdFrame;
class UndoFrame;
class EditPagesModel;

//#include "itmpl_plugin.h"
//#include "templ_info.h"
#include "itmpl_sql_plugin.h"
#include "auth_plugin.h"
#include "addtemplate.h"
#include "mytypes.h"

using namespace VPrn;

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow();
    //Директор по такелажу :) - т.е грузчик !(Грузит шаблон из файла)
    void loadFromFile(const QString &file_name);

public slots:
    void createNewTemplate();

private slots:
    void updateUndoLimit(int limit);
    void showTemplatesInfo();
    void pageSelect(int page);
    void toggleAntialiasing();
    void loadTemplates();
    void saveTemplatesAs();

    void do_needCreateEmptyTemplates(QString &file_name);
    void saveUserName(QString & u_name);


    void errorA(QString e_msg); // Ошибка А типа - завершать рабту
    void errorB(QString e_msg); // Ошибка Б типа -  не  завершить работу
    void errorInfo(pluginsError eCode,QString e_msg); // Ошибка А типа -  завершить работу
    void do_CmdButtonClick(const QString &line);// Нажали командную кнопку
    //void do_angle_direct();
    void do_viewCode();
signals:
    void addBaseElementToPage(int, const QString &line);
    void addImgElementToPage(int , const QString &templ_fn);
private:
    void createActions();
    void createMenus();
    void createToolBars();
    void createStatusBar();
    void createDockWindows();
    void loadPlugins();

    void error(QString e_msg,bool admin);
    void printTempl();
    void readGlobal(const QString &app_dir);
    /**
     * @fn Поворачивает страницу на 90 градусов по часовой или против
     */
    //void flipPage(bool angle_direct);

    Auth_plugin *auth_plugin;
    Itmpl_sql_plugin *tmpl_plugin;

    QTabWidget * tabWidget;
    QMenu *templatesMenu;
    QMenu *editMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QMenu *toolsMenu;


    QToolBar *editToolBar;
    QToolBar *toolsToolBar;

    QAction *antialiasAct;
    QAction *newAct;
    QAction *loadAct;
    QAction *saveAct;
    QAction *saveAsAct;
    QAction *showInfoAct;

    QAction *printAct;
    QAction *undoAct;
    QAction* redoAct;

    QAction *aboutAct;
    QAction *aboutQtAct;
    QAction *quitAct;
    //QAction *flipAction;
    QAction *addBaseElem;
    //QAction *portretAct;
    //QAction *landscapeAct;
    QAction *viewCodeAct;
    QAction *showUndoStackAct;



    // Приватные данные
    QString userName;

    QString currentTemplates;
    QStringListModel *page_size_model;
    QStringList elemList;
    bool templ_load; // Признак что шаблон загружен

    AddTemplate *TProperDlg;
    cmdFrame * CmdButtonBox;
    UndoFrame * myUndoFrame;

    // Переменные из установок
    QString serverHostName;
    int serverPort;
    QString spoolDir;
    QString ticket_fname;
    QString local_t_path; // Путь к локальным шаблонам пользователя
    QString global_t_path;
    QString ftpTemplatesDir;
    /**
      * @var QSqlQueryModel * pagesModel; Указатель на модель СТРАНИЦЫ_ШАБЛОНА
      */
    EditPagesModel * pagesModel;

    QUndoStack * m_ActiveStack;   // Текущий активный стек Undo
    QUndoGroup * m_undoGroup;     // Группа стеков undo
    /**
      * @var QMap <int,myScene *>  m_Scenes;
      * @brief Группа сцен
      */
    QMap <int,myScene *> m_Scenes;

    /**
      * @var QMap <int,View *> m_View;
      * @brief Группа окон отображения
      */
    QMap <int,View *> m_View;

};

#endif
