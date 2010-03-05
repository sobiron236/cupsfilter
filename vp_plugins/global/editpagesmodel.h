#ifndef EDITPAGESMODEL_H
#define EDITPAGESMODEL_H

#include <QSqlQueryModel>
#include "mytypes.h"

/**
  * @class EditPagesModel ������������� SQl ������
  * @brief ��������� ���������� ������ �  ��������� �������:
  * ���  �   ��������
  * r  | 0  |ID
  * r  | 1  |���������� �����
  * r  | 2  |��� ��������;
  * r  | 3  |�������� ��������
  * rw | 4  |����� �������� ��/��� 1/0
  */

using namespace VPrn;

class EditPagesModel : public QSqlQueryModel
{
Q_OBJECT
public:
    explicit EditPagesModel(QObject *parent = 0);

    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    void refresh();
private:
    bool setPagesName   (int id, QString &name);
    bool setPagesNumber (int id, int number);
    bool setPagesType   (int id, int type);
    bool setPageVisible (int id, int page_visible);
};

#endif // EDITPAGESMODEL_H
