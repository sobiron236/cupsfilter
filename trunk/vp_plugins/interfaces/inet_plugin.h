#ifndef INET_PLUGIN_H
#define INET_PLUGIN_H

//#include <QString>
class QString;

//! [0]
class Inet_plugin{

public:
    virtual ~Inet_plugin() {}
    virtual void init(const QString &host,int port,const QString &sid)=0;
    virtual void sendData(const QString &cmd)=0;
    //virtual bool state()=0;

};


QT_BEGIN_NAMESPACE
Q_DECLARE_INTERFACE(Inet_plugin,"com.technoserv.Plugin.Inet_plugin/1.0");
QT_END_NAMESPACE

//! [0]
#endif