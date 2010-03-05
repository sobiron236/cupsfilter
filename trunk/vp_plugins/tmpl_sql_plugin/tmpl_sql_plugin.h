#ifndef TMPL_SQL_PLUGIN_H
#define TMPL_SQL_PLUGIN_H

/**
  * @mainpage Плагин
  * предназначен для работы с шаблоном, который представляет БД SQLITE
  * При первоначальной загрузке плагина происходит инициализация рабочих,
  * наборов данных:
  * @var  QMap <int,myScene *> scenesGrp;
  * Список указателей на сцены которые отрисовывают страницы шаблона
  * @var QUndoGroup * undoGrp;
  * Группа UndoStack  каждый их которых связан со своей сценой
  * В памяти :memory: создается пустая БД заполненая необходимыми
  * таблицами и связями, с данной БД связваются  необходимые модели.
  * Если пользователь выбирает СОЗДАТЬ новый пустой шаблон, то создается
  * новая БД по указанному пользователем пути,которая присоедияется к БД
  * в памяти ATTACH DB и в нее копируются данный из временной БД, после чего
  * модели настраиваютсяя на получение данных из новой БД, если возникает
  * необходимость нового временного шаблона, то данный цикл повторяется
*/
#include <QObject>
#include <QSqlDatabase>
#include <QtCore/QHash>
#include <QUndoStack>
#include <QUndoGroup>
#include <QMap>

class QSqlQuery;
class QSqlQueryModel;
class QSqlRelationalTableModel;
class QSqlError;
class QSqlTableModel;
//class QGraphicsItem;
class myScene;


#include "itmpl_sql_plugin.h"
#include "tinfoeditmodel.h"
#include "myscene.h"
#include "mytypes.h"

using namespace VPrn;


class Tmpl_sql_plugin : public QObject , Itmpl_sql_plugin
{
    Q_OBJECT
    Q_INTERFACES(Itmpl_sql_plugin)
public:

    Tmpl_sql_plugin(QObject *parent = 0);
    ~Tmpl_sql_plugin();
    /**
      * @fn init(const QString & spool,const QString & sid)
      * Первоначальная настройка плагина
      */
    void init(const QString & spool,const QString & sid);

    inline bool isDBOpened(){return m_dbOpened;};
    inline bool isDBConnected(){return m_dbConnect;};

    /**
      * @brief Геттеры возвращают константные указатели на
      * @fn getInfoModel();        модель ИНФО_ШАБЛОНА
      * @fn getPSizeModel();       модель РАЗМЕРЫ_ЛИСТА
      * @fn getPagesModel();       модель СТРАНИЦЫ_ШАБЛОНА
      * @fn getBaseElemNameList(); набор базовых элементов шаблона
      * @fn getUndoGrp();          группу стеков
      * @fn getScenesGroup();      список сцен, со связанными стеками Undo
      */
    TemplateInfoEditModel * getInfoModel() const {return tInfoModel;};
    QSqlQueryModel        * getPSizeModel()const {return pSizeModel;};
    QSqlQueryModel        * getPagesModel()const {return pagesModel;};
    QStringList             getBaseElemNameList() const;
    QUndoGroup            * getUndoGrp() const {return undoGrp;};
    QMap <int,myScene *>  getScenesGroup() {return scenesGrp;}
    //-------------------------------------------------------------------

    /**
      * @brief Устанавливает имя пользователя полученное от плагина Auth
      */
    void setUserName (const QString &user){userName = user;};

signals:
    void error(pluginsError errCode,QString error_message);
    void allTemplatesPagesParsed();
  /*
    void allTemplatesPagesParsed(myScene *scene_11,myScene *scene_12,
                                 myScene *scene_13,myScene *scene_14,
                                 myScene *scene_15,myScene *scene_2,
                                 myScene *scene_3,myScene *scene_4);
*/
public slots:
    /**
      * @fn void setTagValue(const QHash <QString, QString> &tagValue);
      * @brief Предназначенна для записи в шаблон значений полей
      * пробегает по всем элементам хеша и ищет соответсвитвие в списке
      * элементов шаблона после чего делает update таблицы elem и обновление
      * элементов отображения
      */
    void setTagValue(const QHash<QString, QString> &tagValue);
    /**
      * @fn Открытие шаблона
      * @brief работает так если драйвер БД загружен и установленно соединение,
      * то открыть существующий файл как БД
      */
    void openTemplates(const QString & t_fileName);
    /**
      * @fn Создание пустого шаблона (БД) во временном файле
      * @brief если драйвер БД загружен и установленно соединение,
      * то создадим БД в заданном файле и наполним его начальным содержимым
      * Пустой шаблон создается в следующем порядке:
      * 1. проверка что файл заданный как БД шаблона не существует, но мождет быть создан
      *    - имя файла проходит проверку на валидность (имя_файла.tmpl)
      *    - такой файл в заданном месте можно создать
      * 2. Проверка того что драйвер БД загружен, соединение по умолчанию установленно
      * 3. Открытие БД и создание структуры базы
      */
    void createEmptyTemplate();
    /**
      * @fn сохраним шаблон с заданным именем
      */
    void saveTemplatesAs(const QString & fileName);
    void closeTemplates();

    /** @fn doAddBaseElementToPage(int page,QString &tag)
      * добавляет базовый элемент на страницу с номером page
      */
    void doAddBaseElementToPage(int page,const QString &tag);

private:    

    bool m_dbOpened;
    bool m_dbConnect;
    /**
      * @var spool_dir    каталог для временных файлов
      * @var current_sid  уникальный сеансовый номер
      * Пути к генерируемым страницам шаблона 1-4
      * @todo должны формироваться по запросу приложения
      * @var firstPage_tmpl_fn;
      * @var secondPage_tmpl_fn;
      * @var thirdPage_tmpl_fn;
      * @var fourthPage_tmpl_fn;
      * Набор из 8-х сцен
      */
    QString userName;
    QString spool_dir;
    QString current_sid;

    QString firstPage_tmpl_fn;
    QString firstPage_tmpl_fn2;
    QString firstPage_tmpl_fn3;
    QString firstPage_tmpl_fn4;
    QString firstPage_tmpl_fn5;

    QString secondPage_tmpl_fn;
    QString thirdPage_tmpl_fn;
    QString fourthPage_tmpl_fn;

    /**
      * @var QMap <int,myScene *> scenesGrp; Список сцен
      */
    QMap <int,myScene *> scenesGrp;
    /**
      * @var QUndoGroup * undoGrp; Группа стеков Undo
      */
    QUndoGroup * undoGrp;
/*
    myScene * firstPage_scene;
    myScene * firstPage_sceneN2;
    myScene * firstPage_sceneN3;
    myScene * firstPage_sceneN4;
    myScene * firstPage_sceneN5;
    myScene * secondPage_scene;
    myScene * thirdPage_scene;
    myScene * fourthPage_scene;
*/
    bool view_code_state;

    QString m_connectionName;
    /**
      * @var baseElemList;
      * список базовых элементов
      */
    QStringList baseElemList;

    /**
      * @var currentDBFileName
      * Имя файла текущей открытой БД
      */
    QString currentDBFileName;
    /**
      * @fn Модель ИНФО_ШАБЛОНА
      */
    TemplateInfoEditModel  * tInfoModel;

    /**
      * @var Модель РАЗМЕРЫ_ЛИСТА
      */
    QSqlQueryModel * pSizeModel;

    /**
      * @var Модель СТРАНИЦЫ_ШАБЛОНА
      * @brief В данной модели храняться все страницы шаблона
      * №п/п|Тип страницы| Отображаемое имя страницы
      */
    QSqlQueryModel * pagesModel;
    /**
      * @var Модель ЭЛЕМЕНТЫ_СТРАНИЦЫ
      * @brief В данной модели храняться все элементы
      * размещенные на i-странице шаблона
      * text|tag|pos_x|pos_y|color|font|angle|border|img_data|always_view
      */
    QSqlQueryModel * elemInPageModel;

    /**
      * @var Singleton DB connection
      */
    QSqlDatabase DB_;

    /**
      * @fn Открываем существующую БД
      */
    bool openDataBase(const QString & t_fileName);
    /**
      * @fn Создаем пустую БД с параметрами по умолчанию
      */
    bool create_emptyDB(const QString &emptyDB_fileName);
    /**
      * @fn Создает набор необходимых таблиц
      * QSqlQuery &query связан с нужным соединением с БД
      */
    bool create_tables(QSqlQuery &query);
    /**
      @fn Загружает драйвер и устанавливает соединение с БД sqlite
      */
    bool createConnection();

    /**
      * @brief Настройка модели, для установленного соединения
      * и файла открытого как БД
      * прочитать данные и записать их в соответсвующие модели
      */
    bool fillModels();
    void DumpError (const QSqlError & lastError);

    /**
      * @brief Предварительная настройка БД
      */
    bool InitDB ();

    /**
      * @brief Проверяет корректность имени файла [имя файла].tmpl
      */
    bool isValidFileName(const QString & fileName);

    /**
      * @brief Проверяет возможность создания файла по указанному пути
      * 1.Проверка что данный файл не существует.
      * Если файл есть на диске, то удаляем его с диска.
      * (Так как пользователь уже согласился с выбором именно этого файла,
      *  выбрал переписать его)
      * 2.Создаем и удаляем файл по указанному пути
      */
    bool isCreateFile(const QString & fileName);


    /** @fn bool fillScenes4Data()
      * Рабор данных полученных из шаблона и запись их в сцены
      */
    bool fillScenes4Data();

    /**
      * @fn bool fillPagesSizeTable(QSqlQuery &query)
      * @brief Заполняет таблицу размеров страниц
      */
    bool fillPagesSizeTable(QSqlQuery &query);

    /**
      * @fn int getId4pageSizeTable(QSqlQuery &query,const QString & findSize)
      * @retval int
      * @brief Возвращает id Записи в таблице размеров страниц
      */
    int getId4pageSizeTable(QSqlQuery &query,const QString & findSize);
    /**
      * @fn void update_scenes(const QHash<QString, QString> &hash)
      * @brief проходит по всем элементам каждой сцены и проверяет
      * есть ли там элемент с тегом = tag если есть, то записывает ему новое
      * значение, координаты элемента не меняются
      */
    void update_scenes(const QHash<QString, QString> &hash);
    /**
      * @fn myScene * selectScene(int page)const
      * @brief возвращает указатель на требуемую сцену
      */
    myScene * selectScene(int page) const;

};


#endif
