#include "mytextitem.h"
#include <QFont>

myTextItem::myTextItem(QGraphicsTextItem * parent)
        :QGraphicsTextItem(parent)
{
    // �������������� ���������
    this->setFont(QFont("Times", 10, QFont::Bold));    
}

