#ifndef ENGINE_H
#define ENGINE_H
#include "QtCore/QObject"
#include <QtCore/QFileInfo>

QT_FORWARD_DECLARE_CLASS ( QStringList )
QT_FORWARD_DECLARE_CLASS ( QLocalSocket )

class mySocketClient;
class Message;


#include "mytypes.h"
#include "igs_plugin.h"

using namespace VPrn;

class Engine :public QObject
{
    Q_OBJECT
public:

    Engine(QObject*parent = 0,const QString &app_path = QString(),
           const QString &client_name = QString());
    /**
      * @fn ~Engine();
      * @brief В деструкторе проверяем живость соденения и если оно есть прибиваем его
      */
    ~Engine();

    void init();
    /**
      @fn void prepareFileToPrint(const QString & file_name);
      @brief Отпаравляет запрос на GateKeeper с требованием конвертировать ps -> pdf
      */
    void prepareFileToPrint(const QString & file_name);
    /**
      * @fn bool isError();
      * @brief Если ошибка вернет истину
      */
    bool isError(){return e_state;}
    /**
      * @fn QString lastError()
      * @brief Возвращает текстовое описание последней ошибки
      */
    QString lastError() const {return e_info;}

    /**
      * @fn  void setUserMandat(const QString & u_name,const QString & u_mandat);
      * @brief При авторизации с консоли юзер выбрам мандат
      * передадим его демону и запросим список уровней секретности для данного мандата
      */
    void setUserMandat(const QString & u_name,const QString & u_mandat);
    /**
      * @fn QStringList &getSecLevelList() const;
      * @brief Возвращает список уровней секретности
      */
    QStringList getSecLevelList() const {return secLevelList;}
    /**
      * @fn QStringList &getTemplatesList() const;
      * @brief Возвращает список шаблонов
      */
    QStringList getTemplatesList() const {return templatesList; }

    /**
     * @fn void authUserToPrinter(const QString &printer_uri);
     * @brief Авторизация текщего пользователя на доступ к выбранному принтеру
     */
    void authUserToPrinter(const QString &printer_uri);
    /**
      * @fn void checkMB( const QString &sql_query);
      * @brief Отправляет локальному серверу запрос на проверку существования
      * документа с заданными атрибутами (МБ,номер экз. и название документа
      * в БД учета
      */
    void checkMB( const QString &sql_query );
    /**
      * @fn void registerMB ( const QString &sql_query );
      * @brief Запрос на регистрацию документа в БД учета
      */
    void registerMB ( const QString &sql_query );

    void doMergeDocWithTemplates (QByteArray field_data,bool preview_mode);
signals:
    // Вернем путь к шаблону
    void setTemplatesFileName( QString t_path);
    // Результат наложения шаблона на документ
    void MergeDocWithTemplates( bool flag,const QString &info);
    // список путей к сформированным  страницам предпросмотра
    void PreviewPages (QByteArray page_list);
    // Список путей уже сформированным документам готовым для печати
    void ReadyForPrintPages(QByteArray page_list);
    // Получено требование завершить работу
    void reciveGoodBayMsg(const QString &shutdown_info);
    // Требуется передать демону дополнительные параметы документа для его регистрации
    // в бд учета
    void needRegisterDocInBase();
    //Документ зарегистрирован в БД УЧЕТА
    void RegisterDocInBase(bool flag,const QString &info);
    // Результат авторизации доступа к устройству
    void authToDevice(bool flag,const QString &info);

    void infoMessage(const QString &info);
    void gk_notReady(const QString &ext_info); // локальный сервер не готов к работе
    // Требуется авторизовать пользователя есть имя, и связь с демоном
    void needAuthUser(const QString &login_mandat_list);
    void gk_fullReady(const QString &login,const QString &mandat);
    //Локальный сервер вернул список принтеров доступных данному пользователю
    void getPrinterList(QStringList &prn_list);
    // Успешная загрузка плагинов
    void pluginsLoad();
    // Регистрация на локальном сервере
    void LocalSrvRegistr();
    void RemoteDemonRegistr();
    void ReciveUserName();
    void ReciveUserMandat();
    void RecivePrintersList();
    void ReciveSecLevelList();
    // Документ успешно конвертирован в pdf
    void doc_converted();
    // Подсчитали количество страниц
    void getPagesCount (int p_count);
    // Когда завершенно разбиение страниц испускается соответсвующий сигнал
    void firstPageSplitted();
    void otherPageSplitted();
    /**
      * @fn void error(const QString &e_info);
      * @brief При возникновении ошибки, сигнал передает описание ошибки
      * @todo Убрать функции @sa @fn QString lastError() @sa @fn bool isError()
      */
    void error(const QString &e_info);
public slots:
    /**
      * @fn void convertTemplatesNameToFilePath(QString t_name);
      * @brief Преобразование имени шаблона в полный путь к его файлу
      */
    void convertTemplatesNameToFilePath(QString t_name);

private slots:
    void do_checkPointChanged(MyCheckPoints r_cpoint);
    /**
      * @fn void parseMessage(const Message r_msg);
      * @brief Обработка полученных сообщений от локального сервера
      */
    void parseMessage(const Message &r_msg);
    /**
      * @fn void launchAndConnect();
      * @brief проверяет факт запуска GateKeeper (Запускает его если не запущен)
      * и устаналивает с ним долгие и доверительные отношения
      */
    void launchAndConnect();

private:
    /** @brief Список переменных из ini файла
      * @var serverHostName;   Имя удаленного сервера (Демон для mainfarme)
      * @var serverPort;       Порт для связи
      * @var link_name;        Имя локального сервера для общения с Локальным миром
      * @var gsBin;            Путь к исполняемому файлу gs
      * @var pdftkBin;         Путь к исполняемому файлу pdfTk
      * @var spoolDir;         Путь к каталогу временных файлов
      * @var gatekeeper_bin;   Путь к исполняемому файлу GateKeeper
      * @var local_t_path;     Путь к локальным шаблонам
      * @var global_t_path;    Путь к глобальным шаблонам
      *
      * @brief Локальные переменные
      * @var clientName;       Имя клиента приложения для индетификации в GateKeeper
      * @var e_info;           Последняя возникшая ошибка
      * @var e_state;          Статус Локального сокета
      * @var m_LocalClient;    Указатель на локальный сокет
      * @var stopLaunch;       Триггер повторного запуска GateKeeper
      * @var m_appPath;        Путь запуска приложения для поиска ini файла
      * @var gs_plugin;        Указатель на плагин gs @todo Нужен ли ?
      * @var client_uuid;      Уникальный номер полученный от GateKeeper
      * @var secLevelList;     Список уровней секретности для данного мандата
      */
    QString serverHostName;
    int serverPort;
    QString link_name;
    QString gsBin;
    QString pdftkBin;
    QString spoolDir;
    QString gatekeeper_bin;
    QString local_t_path;
    QString global_t_path;

    QString clientName; // Имя клиента
    QString e_info;
    bool e_state;
    mySocketClient *m_LocalClient;
    bool stopLaunch;
    QString m_appPath;
    Igs_plugin *gs_plugin;
    QString client_uuid; // Уникальный номер полученный от GateKeeper
    QStringList secLevelList;
    QStringList templatesList;
    QString currentUserName;
    QString currentUserMandat;

    QList <QFileInfo> templatesFileInfoList;
    QMap <QString,QString> printer_device_list;

    /**
      * @fn void sendMessage2LocalSrv(const Message &s_msg);
      * @brief отправка сообщения локальному серверу
      */
    void sendMessage2LocalSrv(const Message &s_msg);
    /**
      * @fn void readConfig();
      * @brief Читаем установки из ini файла
      */
    void readConfig(const QString &ini_file);

    /**
      * @fn setError(const QString &info);
      * @brief Устанавливает состояние ошибки,  записывает новое описание ошибки
      */
    void setError(const QString &info);
    /**
      * @fn setError();
      * @brief Устанавливает состояние ошибки = НЕТ ошибки
      */
    void setError();

    /**
      * @fn void setFileList(const QString &t_path,const QString &prefix);
      * @brief Основная функция которая записывает список шаблонов в templatesList
      */
    void setFileList(const QString &t_path,const QString &prefix);

    /**
      * @fn QString findPrinterInDeviceURIList(const QString &prn);
      * @brief Поиск в списке device_uri выбранного пользователем принтера
      * и возврат его  device_uri для авторизации
      */
    QString findPrinterInDeviceURIList(const QString &prn);
    /**
      * @fn void convertDeviceURIListToPrinterList(const QString & device_uri_list);
      * @brief Сформирует из списка device_uri список принтеров, согласно
      * канонам дзенбуддизма и отправит сигнал с этим списком
      */
    void convertDeviceURIListToPrinterList(const QString & device_uri_list);
};



#endif // ENGINE_H