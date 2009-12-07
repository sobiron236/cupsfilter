#ifndef TMPL_PLUGIN_H
#define TMPL_PLUGIN_H

#include <QObject>
#include <QDebug>
#include <QPixmap>
#include <QtPlugin>
#include <QPainter>
#include <QFile>
#include <QDir>
#include <QTemporaryFile>
#include <QDataStream>
#include <QPrinter>
#include <QPainter>
#include <QDate>
#include <QGraphicsRectItem>


#include "itmpl_plugin.h"
#include "tech_global.h"

using namespace SafeVirtualPrinter;

class Tmpl_plugin :public QObject, Itmpl_plugin
{
    Q_OBJECT
    Q_INTERFACES(Itmpl_plugin)
public:

    Tmpl_plugin(QObject *parent=0){};
    void init(const QString & spool,const QString & sid);
    void createEmptyTemplate();
    // ���������� ������ ���� � ��������������� �� ��������� ������� � ������ ������ �����
    void setTemplates(const QString & templates_in_file,QStandardItemModel * model);
    void printFormatingPageToFile(int pageNum);
    QGraphicsScene *getFirstPage(){return firstPage_tmpl;};
    QGraphicsScene *getSecondPage(){return secondPage_tmpl;};
    QGraphicsScene *getThirdPage(){return thirdPage_tmpl;};
    QGraphicsScene *getFourthPage(){return fourthPage_tmpl;};

signals:
    void error(QString error_message);
    void allTemplatesPagesParsed();
    void emptyTemplateCreate(const QString & t_name);
public slots:
    void update_scene(int pageNum);
protected:
    bool parse_templates(const QString & in_file);
    // ���� � ������ ������� � ��������� [���] ��� ���==����� �������
    // � ���������� ��� �������� ������� ��� ���������������
    QString findFromModel(const QString &find_line);
    static const int version = 1;

    QStandardItemModel *work_model;

    QString spool_dir;
    QString current_sid;
    QString currentTemplates;
    QString emptyTemplate_file; // ���� ���������� ������ ������

    QString firstPage_tmpl_fn;
    QString secondPage_tmpl_fn;
    QString thirdPage_tmpl_fn;
    QString fourthPage_tmpl_fn;

    QGraphicsScene * firstPage_tmpl;
    QGraphicsScene * secondPage_tmpl;
    QGraphicsScene * thirdPage_tmpl;
    QGraphicsScene * fourthPage_tmpl;


    // ������� ������
    QDate date_time;               // ���� � ����� �������� �������
    QString author;                  // ����� ������� (author [date_time]) ������������ � tooltipe
    QString templates_name;          // �������� �������, �� ��� ������� � ������ ��������
    QString description;             // �������� �������, ����� ���� ������
    QString paper_size;
    QMap<QString, int> page_size;
    qreal page_height;               //
    qreal page_width;                //
    bool  page_orient;               //
    qreal margin_top;                //
    qreal margin_bottom;             //
    qreal margin_left;               //
    qreal margin_right;              //

    int firstPageElemCount;
    int secondPageElemCount;
    int thirdPageElemCount;
    int fourthPageElemCount;
};

#endif
