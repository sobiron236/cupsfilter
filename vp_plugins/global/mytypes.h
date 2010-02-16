#ifndef MY_TYPES_H
#define MY_TYPES_H

#include <QMetaType>

namespace VPrn{

#define POINT_TO_CM(cm) ((cm)/28.3465058)
#define POINT_TO_MM(mm) ((mm)/2.83465058)     ////////  0.352777778
#define POINT_TO_DM(dm) ((dm)/283.465058)
#define POINT_TO_INCH(inch) ((inch)/72.0)

#define MM_TO_POINT(mm) ((mm)*2.83465058)
#define CM_TO_POINT(cm) ((cm)*28.3465058)     ///// 28.346456693
#define DM_TO_POINT(dm) ((dm)*283.465058)
#define INCH_TO_POINT(inch) ((inch)*72.0)

static const int ObjectName = 0;

enum pSizeColumnOrder{
        pSize_id     = 0, // ID
        pSize_page   = 1, // ������ �����
        pSize_code   = 2, // ���
        pSize_width  = 3, // ������ (��)
        pSize_height = 4  // ������ (��)

};

enum tInfoColumnOrder{
            tInfo_id     = 0,  //ID
            tInfo_name   = 1,  //��� �������
            tInfo_desc   = 2,  //�������� �������;
            tInfo_pageID = 3,  //������ �� ������� ������� �������
            tInfo_angle  = 4,  //������� (����.)
            tInfo_ctime  = 5,  //����� ��������
            tInfo_mtime  = 6,  //����� ���������
            tInfo_author = 7, //�����
            tInfo_mtop   = 8, //������ ������ (��)
            tInfo_mbottom = 9,//������ ����� (��)
            tInfo_mleft   = 10,//������ ����� (��)
            tInfo_mright  = 11 //������ ������ (��)

};

enum pluginsError {
        NoError,
        FileNotFound,
        FileIOError,
        UnknownError,
        DriverNotLoad,
        NotConnectedToDB,
	DBOpeningError,
        SQLQueryError,
        SQLCommonError,
        InternalPluginError
};

enum pageType{
     FirstPage  = 1,
     SecondPage = 2,
     ThirdPage  = 3,
     FourthPage = 4
};

}


Q_DECLARE_METATYPE(VPrn::pSizeColumnOrder);
Q_DECLARE_METATYPE(VPrn::tInfoColumnOrder);

#endif

