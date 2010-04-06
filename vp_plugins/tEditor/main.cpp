#include <QtGui/QApplication>
#include <QDebug>
#include <QTextCodec>
#include <QStringList>

#include "config.h"

#include "mainwindow.h"



int main(int argc, char *argv[])
{
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForTr(codec);
    QTextCodec::setCodecForCStrings(codec);
    QTextCodec::setCodecForLocale(codec);
    qInstallMsgHandler(myMessageOutput);

    QApplication app(argc, argv);
    Q_INIT_RESOURCE(images);

    QStringList aList=app.arguments();
    /*
     * @brief По количеству аргументов определяем режим работы
     * - sizeOf (argv[]) = 1 (без аргументов) режим создания пустого шаблона
     * - sizeOf (argv[]) = 2 (имя_файла_шаблона) редактирование выбранного шаблона
    */
    QString file_name;

    MainWindow window;
    window.show();

    switch (aList.size()){
     case 1:
       window.createNewTemplate();
       break;
     case 2:
       file_name = aList.at(1);
       window.loadFromFile(file_name);
       break;
    }

    return app.exec();
}
