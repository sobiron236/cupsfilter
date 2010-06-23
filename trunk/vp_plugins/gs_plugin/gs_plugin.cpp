#include "gs_plugin.h"
#include "proc_thread.h"
#include "clientdata.h"

#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QDir>
#include <QtCore/QFileInfoList>
#include <QtCore/QRegExp>
#include <QtCore/QTemporaryFile>
#include <QtCore/QTextStream>
#include <QtCore/QDateTime>
#include <QtCore/QUuid>

#include <QtGui/QPixmap>

GS_plugin::GS_plugin(QObject *parent)
    :QObject (parent)
{
    //    if (!QMetaType::isRegistered(QMetaType::type("JobsType"))){
    //        qRegisterMetaType<VPrn::Jobs>("JobsType");
    //    }
}


void GS_plugin::final_clear()
{
    //Самый важнецкий деструктор, удаляет временный файл  gs_rcp;
    // В дальнейшем туточки будет выгружаться библиотека gs_lib
    QFile::remove(gs_rcp);
}

void GS_plugin::init(const QString &gs_bin, const QString &pdftk_bin,
                     const QString &temp_folder)
{
    QDir dir;
    QFile new_file;
    const QString startnow = QDir::currentPath();

    bool Ok = true;
    {
        Ok &= QFile::exists(gs_bin);
        if (Ok){// Проверим факт существования по указанному пути бинарника ghostscript
            gsBin = gs_bin;

            Ok &= dir.cd(temp_folder) && !temp_folder.isEmpty();
            if (Ok){// Проверим факт существования временного каталога
                spoolDir = temp_folder;
                myEnv << QObject::trUtf8("TMPDIR=%1").arg(temp_folder);

                Ok &=QFile::exists(pdftk_bin);
                if (Ok){// Проверим факт существования по указанному пути бинарника
                    pdftkBin = pdftk_bin;
                    // Файл не существует но он мне нужен значит создаем его
                    gs_rcp=QString("%1/%2.rcp").arg(temp_folder,getUuid());
                    // Надо больше спать из за отсутсвия этой строчки 20 минут искал ошибку
                    new_file.setFileName(gs_rcp);
                    if (new_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                        QTextStream out(&new_file);
                        out     << QObject::trUtf8("-dQUIET\n")
                                << QObject::trUtf8("-dNOPAUSE\n")
                                << QObject::trUtf8("-dPARANOIDSAFER\n")
                                << QObject::trUtf8("-dBATCH\n")
                                << QObject::trUtf8("-r600\n")
                                << QObject::trUtf8("-dPDFSETTINGS=/prepress\n")
                                << QObject::trUtf8("-dEmbedAllFonts=true\n")
                                << QObject::trUtf8("-sDEVICE=pdfwrite\n");
                        new_file.close();
                    }else{
                        emit error(VPrn::FileIOError,
                                   QObject::trUtf8("Ошибка создания файла %1\nОшибка %2")
                                   .arg(gs_rcp).arg(new_file.errorString()));
                    }
                }else{
                    emit error(VPrn::FileIOError,
                               QObject::trUtf8("Файл %1 не найден\n")
                               .arg(pdftk_bin));
                }
            }else{
                emit error(VPrn::FileIOError,
                           QObject::trUtf8("Каталог %1 не найден\n")
                           .arg(temp_folder));
            }
        }else {
            emit error(VPrn::FileIOError, QObject::trUtf8("Файл %1 не найден\n").arg(gs_bin));
        }
    }
    QDir::setCurrent(startnow);
}

void GS_plugin::convertPs2Pdf(const QString &client_uuid,const QString &input_fn)
{
    // Проверим фактик наличия входного файла
    qDebug() << Q_FUNC_INFO << input_fn;
    if (QFile::exists(input_fn)) {
        // Поиск данных для клиента
        ClientData *c_data = findClientData(client_uuid);

        // Формируем основной выходной файл для клиента
        c_data->setMainFile(QString("%1/%2/main.pdf")
                            .arg( spoolDir, client_uuid ) );

        // Формируем строку команды для первого этапа
        QStringList args;

        args.append("-q" );
        args.append(QString("@%1").arg(gs_rcp));
        args.append(QString("-sOutputFile=%1").arg(c_data->getMainFile()));
        args.append("-c");
        args.append(".setpdfwrite");
        args.append("-f");
        args.append(input_fn);
        start_proc(client_uuid,gsBin,args,VPrn::job_ConvertPs2Pdf);

    }else {
        emit error(VPrn::FileIOError,
                   QObject::trUtf8("ERROR : Файл %1 не найден\n").arg(input_fn));
    }
}

void GS_plugin::print2devide (const QString &client_uuid,  const QString &print_file,
                              const QString &prn_device,bool usePageCount)
{
    //-q -dQUIET -dNOPAUSE -dPARANOIDSAFER -dBATCH -r300 -sDEVICE=pdfwrite -sOutputFile="%printer%pdfcreator" -c .setpdfwrite -f %1
    // Поиск данных для клиента
    QStringList args;

    ClientData *c_data = findClientData(client_uuid);
    int print_copy;
    if (usePageCount){
        print_copy = c_data->getPageCount();
        print_copy -=1;
    }else{
        print_copy = 1;
    }
#if defined(Q_OS_UNIX)
    args.clear();
    args.append("-P");

    args.append(prn_device.section(".",1,1));
    args.append(tr("-#%1").arg(print_copy));
    args.append(print_file);
    start_proc(client_uuid,"lpr",args,VPrn::job_PrintFile);

#elif defined(Q_OS_WIN)

    for (int i=0;i<print_copy;i++){
        args.clear();
        args.append("-S");
        args.append("192.168.112.2");
        args.append("-P QQQQ");
        args.append("-o l");
        args.append("-d");
        args.append(print_file);
        start_proc(client_uuid,"lpr",args,VPrn::job_PrintFile);
    }
#endif
}

void GS_plugin::mergeWithTemplate(const QString &client_uuid, const QStringList &t_files)
{
    // Поиск данных для клиента
    ClientData *c_data = findClientData(client_uuid);
    if (!QFile::exists(c_data->getFirstPage() ) ){
        emit error (VPrn::FileIOError,
                    QObject::trUtf8("Файл %1 не найден!")
                    .arg(c_data->getFirstPage()) );
        return;
    }

    if (t_files.size() == 0){
        emit error (VPrn::InternalPluginError,
                    QObject::trUtf8("Ошибка при подготовке шаблона!")     );

        return;
    }
    // Цикл по всем элементам шаблона, для каждого экземпляра
    QString backPdf;
    QString outPdf;
    for (int i=0; i< t_files.size();i++){
        backPdf = t_files.at(i);
        // spoolDir/client_uuid/t_firstpage.pdf
        QRegExp rx("/(.+)/(.+)/(.-copy)/(t_.+).pdf");
        if(rx.indexIn( backPdf ) != -1){
            // Наш файлик можно обрабатывать
            QString copy_num = rx.cap(3);
            QString page_type  = rx.cap(4);

            if ( page_type.compare("t_firstpage",Qt::CaseInsensitive) == 0){
                // Лицевая сторона i экз
                outPdf = QString("%1/%2/%3/firstpage_out.pdf")
                         .arg(spoolDir,client_uuid,copy_num);
                c_data->startMerge( i );
                mergePdf(client_uuid,c_data->getFirstPage(),backPdf,outPdf);
            }
            if ( page_type.compare("t_otherpage",Qt::CaseInsensitive) == 0  &&
                 c_data->getPageCount() >1    ){
                outPdf = QString("%1/%2/%3/otherpage_out.pdf")
                         .arg(spoolDir,client_uuid,copy_num);
                c_data->startMerge( i );
                // Лицевая сторона второго и последующих листов
                mergePdf(client_uuid,c_data->getOtherPage(),backPdf,outPdf);
            }
            if ( page_type.compare("t_oversidepage",Qt::CaseInsensitive) == 0){
                outPdf = QString("%1/%2/%3/oversidepage_out.pdf")
                         .arg(spoolDir,client_uuid,copy_num);
                QFile::copy(backPdf,outPdf);
            }
            if ( page_type.compare("t_lastpage",Qt::CaseInsensitive) == 0){
                outPdf = QString("%1/%2/%3/lastpage_out.pdf")
                         .arg(spoolDir,client_uuid,copy_num);
                QFile::copy(backPdf,outPdf);
            }

        }
    }
}

void GS_plugin::convertPdfToPng(const QString &client_uuid,
                                const QStringList &files)
{
    // Поиск данных для клиента
    ClientData *c_data = findClientData(client_uuid);

    if (files.size() == 0){
        emit error (VPrn::InternalPluginError,
                    QObject::trUtf8("Ошибка при поиске файлов для преобразования, ничего не найденно !")     );
        return;
    }
    if (c_data){
        QString file_name;
        for (int i=0; i<files.size();i++){
            //gs -q -sDEVICE=png16m -dBATCH -dNOPAUSE -dFirstPage=1 -dLastPage=5 -r300 -sOutputFile=out_%d.png in.pdf
            file_name = files.at(i);

            QRegExp rx("/(.+)/(.+)/(.-copy)/(.+)_out.pdf");
            if (rx.indexIn(file_name) != -1){
                // Наш файлик можно обрабатывать
                QString copy_num = rx.cap(3);
                QString page_type  = rx.cap(4);

                c_data->startConverPdf2Png( i );
                QStringList args;
                args.append("-q");
                args.append("-dQUIET");
                args.append("-dNOPAUSE");
                args.append("-dBATCH");
                args.append("-dPARANOIDSAFER");
                ///@todo Добавить вычисление разрешения max (106*72/max_x,106*72/max_y)

                args.append("-r150");
                args.append("-sDEVICE=png16m");
                args.append("-dFirstPage=1");
                //Формируем имя файлов для предпросмотра
                if (!c_data->getConvertMode()){
                    args.append("-dLastPage=1");
                    args.append(QString("-sOutputFile=%1/%2/%3/%4_0.png")
                                .arg(spoolDir,client_uuid,copy_num,page_type));
                }else{
                    args.append(QString("-sOutputFile=%1/%2/%3/%4_\%d.png")
                                .arg(spoolDir,client_uuid,copy_num,page_type));
                }
                args.append(file_name);
                start_proc(client_uuid,gsBin,args,VPrn::job_ConvertToPng);
            }else{
                emit error (VPrn::FileIOError,QObject::trUtf8("Ошибка в имени файла, при конвертации в pdf -> png!"));

            }
        }
    }
}

void GS_plugin::setConvertToPngMode(const QString &client_uuid, bool full_doc )
{
    ClientData *c_d(0);
    c_d = findClientData (client_uuid);
    if (c_d){
        c_d->setConvertMode(full_doc);
    }
}

QString GS_plugin::getUuid() const
{
    return QUuid::createUuid().toString().replace("{","").replace("}","");
}

void GS_plugin::createClientData(const QString &client_uuid)
{
    /**
    * @todo Кто будет реагировать на ошибку и когда ?
    * проверить что вызвающая функция сумеет обработать ситуацию когда не удалось создать
    * каталоги для клиента
    **/

    ClientData *c_d(0);
    c_d = findClientData (client_uuid);
    QDir w_dir;

    QString wrk_dir =QString("%1/%2").arg(this->spoolDir,client_uuid);
    bool Ok = true;
    {
        if (c_d){
            //w_dir = QDir(wrk_dir);

            //if ( w_dir.exists() ){
              //  recursiveDeletion( wrk_dir );
            //}

            // Формируем требуемые каталоги
            Ok &= w_dir.mkpath(wrk_dir);
            for (int i=1; i<6;i++){
                wrk_dir = QString("%1/%2/%3-copy")
                          .arg(this->spoolDir,client_uuid)
                          .arg(i,0,10);

                if ( QDir(wrk_dir).exists() ){
                    recursiveDeletion( wrk_dir );
                }

                Ok &= w_dir.mkpath(wrk_dir);
                if (!Ok){
                    emit error(VPrn::InternalPluginError,
                               QObject::trUtf8("Ошибка создания рабочего каталога для клиента!\n%1").arg(wrk_dir)
                               );
                    return;
                }
            }
        }else{
            Ok &=false;
            emit error(VPrn::InternalPluginError,
                       QObject::trUtf8("Ошибка создания внутренней учетной записи для клиента!")
                       );

        }
    }
}

void GS_plugin::deleteClientData(const QString &client_uuid)
{
    clients_data.remove(client_uuid);
}


//---------------------------- PRIVATE SLOTS --------------------------------------------------
void GS_plugin::threadFinish(const QString &jobKey,int code,
                             const QString &output_message)
{

    QString m_client_uuid = clients_list.take(jobKey);
    VPrn::Jobs m_job = jobs_list.take(jobKey);
    // Поиск данных для клиента
    ClientData *c_data = findClientData(m_client_uuid);

    if (code != 0 ){
        //Задача завершена с ошибкой, доложим наверх
        ///@todo переделать на сообщение об ошибке
        emit jobFinish(m_client_uuid, m_job,code,output_message);
        return;
    }

    // Формируем пути для файлов
    QString mainPDF       = c_data->getMainFile();

    // Посмотрим чем же таким важным был занят клиент
    QRegExp rx;
    switch (m_job){
    case VPrn::job_ConvertToPng:
        if ( c_data->isFinishedConvertPdf2Png() ){
            emit docReady4preview( m_client_uuid );
        }
        break;
    case VPrn::job_MergePdf:
        if (c_data->isFinishedMerge() ){
            // Все варианты документа объеденнены с шаблоном отправим сигнал
            emit docReady4print( m_client_uuid );
        }
        break;
    case VPrn::job_PrintFile:
        break;
    case VPrn::job_ConvertPs2Pdf: // Завершилась задача конвертирования ps в
        emit docConvertedToPdf(m_client_uuid);
        getPageCount(m_client_uuid,mainPDF);
        break;
    case VPrn::job_SplitPageFirst:
        break;
    case VPrn::job_SplitPageOther:
        if ( !c_data->getOtherPage().isEmpty()  && c_data->getPageCount() > 1  ){
            emit docReady4work(m_client_uuid,c_data->getPageCount());
        }
        break;
    case VPrn::job_CalcPageCount: // Завершилась задача получения dump_info из pdf документа
        int pagesCount; // Число страниц в конвертируемом документе
        if (!output_message.isEmpty()) {
            rx.setPattern("NumberOfPages:\\s+(\\d{1,}).*");
            //rx.setMinimal(true);
            if (rx.indexIn(output_message) != -1) {
                bool ok;
                qDebug() << Q_FUNC_INFO << rx.cap(0) << rx.cap(1);
                pagesCount = rx.cap(1).toInt(&ok, 10);

                if (ok && pagesCount >0 ){
                    c_data->setPageCount(pagesCount);

                    c_data->setFirstPage( QObject::trUtf8("%1/%2/firstpage.pdf")
                                          .arg( spoolDir, m_client_uuid ) );
                    if ( pagesCount ==1){
                        if ( QFile::exists(c_data->getFirstPage()) ){
                            QFile::remove( c_data->getFirstPage() );
                        }
                        if ( !QFile::copy(mainPDF,c_data->getFirstPage() )){
                            emit error(VPrn::InternalPluginError,
                                       QObject::trUtf8("Ошибка копирования документа PDF из %1 в %2")
                                       .arg(mainPDF,c_data->getFirstPage() )
                                       );
                        }else{
                            emit docReady4work(m_client_uuid,pagesCount);
                        }
                    }else{
                        c_data->setOtherPage( QObject::trUtf8("%1/%2/otherpage.pdf")
                                              .arg( spoolDir, m_client_uuid) );
                        // Делим файлы на 2 части
                        splitPdf(m_client_uuid,c_data->getMainFile(),
                                 c_data->getFirstPage(),
                                 c_data->getOtherPage());
                    }
                }else{
                    emit error(VPrn::InternalPluginError,
                               QObject::trUtf8("Ошибка разбора документа PDF %1")
                               .arg(mainPDF)
                               );
                }

            }
        }
        break;
    }

}

//----------------------------------- PRIVATE ---------------------------------------------

void GS_plugin::start_proc(const QString &client_uuid,const QString &bin,
                           QStringList &arg_list,VPrn::Jobs job_id)
{
    // Формируем ключ и запишем данные в списки
    QString j_key = this->getUuid();
    clients_list.insert(j_key,client_uuid);
    jobs_list.insert(j_key,job_id);

    ProcessT *proc = new ProcessT(this,j_key);
    // Обработчик сообщений файлового потока
    connect(proc, SIGNAL(jobFinish     (const QString &,int,const QString &)),
            this, SLOT  (threadFinish  (const QString &,int,const QString &))
            );
    proc->addToEnv(myEnv);
    proc->execute(bin, arg_list,QProcess::MergedChannels);
}

void GS_plugin::getPageCount(const QString &client_uuid,const QString &input_fn)
{
    ClientData *c_d(0);
    if (input_fn.isEmpty()){
         c_d = findClientData (client_uuid);
         emit docReady4work(client_uuid,c_d->getPageCount());
    }else{
        //pdfTk input_file dump_data
        QStringList args;
        args.append(input_fn);
        args.append("dump_data");
        start_proc(client_uuid,pdftkBin,args,VPrn::job_CalcPageCount);
    }
}



void GS_plugin::splitPdf(const QString &client_uuid,const QString &main_pdf,
                         const QString &first_page, const QString &other_pages)
{
    //pdftk %1 cat %2 output page_%2.pdf
    // Так как сюда попадаем только только если в документе больше 1 страницы,
    // То запускаем 2 потока формирирование 1 стр, и формирование последующих

    // Первую страницу
    QStringList args;
    args.append(main_pdf);
    args.append("cat");
    args.append("1");
    args.append("output");
    args.append(first_page);

    start_proc(client_uuid,pdftkBin,args,VPrn::job_SplitPageFirst);

    args.clear();
    args.append(main_pdf);
    args.append("cat");
    args.append("2-end");
    args.append("output");
    args.append(other_pages);

    start_proc(client_uuid,pdftkBin,args,VPrn::job_SplitPageOther);
}

void GS_plugin::mergePdf(const QString &client_uuid,const QString &in_pdf,
                         const QString &back_pdf,const QString &out_pdf)
{
    //pdftk in.pdf background back.pdf output out.pdf
    if (!QFile::exists(in_pdf)) {
        emit error(VPrn::FileIOError,
                   QObject::trUtf8("ERROR : Файл %1 не найден\n").arg(in_pdf));
        return;
    }

    if (!QFile::exists(back_pdf)) {
        emit error(VPrn::FileIOError,
                   QObject::trUtf8("ERROR : Файл %1 не найден\n").arg(back_pdf));
        return;
    }

    QStringList args;
    args.append(in_pdf);
    args.append("background");
    args.append(back_pdf);
    args.append("output");
    args.append(out_pdf);

    start_proc(client_uuid,pdftkBin,args,VPrn::job_MergePdf);
}

ClientData * GS_plugin::findClientData(const QString &client_uuid)
{
    ClientData *c_d(0);

    QMap<QString,ClientData *>::const_iterator i = clients_data.find(client_uuid);
    if ( i == clients_data.end() ){
        clients_data.insert(client_uuid,new ClientData());
    }
    c_d = clients_data.value(client_uuid);
    return c_d;
}

QStringList GS_plugin::findFiles(const QString &client_uuid,const QStringList &filters)
{
    QStringList files;
    QDir dir = QDir::current();

    // Формируем списк файлов которые надо подвергнуть преобразованию в png
    for (int i=1;i<6;i++){

        dir = QDir(QString ("%1/%2/%3-copy").arg(spoolDir,client_uuid).arg(i,0,10) );
        dir.setFilter(QDir::Files | QDir::Hidden | QDir::NoSymLinks);
        dir.setNameFilters(filters);
        QFileInfoList list = dir.entryInfoList();
        for (int i = 0; i < list.size(); ++i)
        {
            QFileInfo fileInfo = list.at(i);
            files.append(dir.absoluteFilePath(fileInfo.fileName()));
        }
    }
    return files;
}

void GS_plugin::recursiveDeletion(QString path)
{
    QDir dir(path);
    QStringList files = dir.entryList(QDir::Files);

    QStringList::Iterator itFile = files.begin();
    while (itFile != files.end())
    {
        QFile file(path + "/" + *itFile);
        if ( !file.remove()){
            emit error(VPrn::FileIOError,
                       QObject::trUtf8("Ошибка удаления файла %1!").arg(*itFile));
        }
        ++itFile;
    }

    QStringList dirs = dir.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
    QStringList::Iterator itDir = dirs.begin();
    while (itDir != dirs.end())
    {
        recursiveDeletion(path + "/" + *itDir);
        ++itDir;
    }

    if ( !dir.rmdir(path)){

        emit error(VPrn::FileIOError,
                   QObject::trUtf8("Ошибка удаления каталога %1!").arg(path));

    }
}
Q_EXPORT_PLUGIN2(Igs_plugin, GS_plugin)
;
