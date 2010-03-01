#ifndef MYSCENE_H
#define MYSCENE_H


#include "mytextitem.h"
#include "mytypes.h"

class QGraphicsSceneMouseEvent;
class QGraphicsSceneContextMenuEvent;
class QUndoStack;
class QMenu;
class QAction;
#include <QGraphicsScene>

using namespace VPrn;

class myScene : public QGraphicsScene
{
    Q_OBJECT
public:
    myScene(int Number,QUndoStack*, QObject *parent = 0 );
    /**
      * @brief ������� �� ����� �������� � ���������
      */
    void  createPage(qreal width,qreal height,qreal m_top,qreal m_bottom,
                     qreal m_right,qreal m_left);

    /**
      * @fn   QGraphicsItem * getPaperItem();
      * @brief ���� �� �������� �������� ������� ������� ��� Paper � ����������
      * ��������� �� ����
      */
    QGraphicsItem * getPaperItem();

    QUndoStack *undoStack() const;
    int getNumber(){return m_Number;};
    void  setAngle(qreal Angle) {m_angle = Angle;};
    qreal getAngle(){return m_angle;};

protected:

    void  mouseReleaseEvent( QGraphicsSceneMouseEvent* );      // receive mouse release events
    void  contextMenuEvent( QGraphicsSceneContextMenuEvent* ); // receive context menu events

signals:
    void  message( QString );                   // info text message signal

public slots:
    void selectItems();     // records selected Item & positions

    /**
      * @fn addBaseElem(const QString &tag = QString("[XXX]"),const QString &text=QString(),
      *                 const QPointF &ps  = QPointF(100.0,100.0),
      *                 const QFont   &fnt = QFont("Times", 14, QFont::Bold),
      *                 const QColor  &col = Qt::black);
      *
      * @brief  ������� ����� ������� TextItem � ��������� ��� �� ������� �����
      */

    void addBaseElem(const QString &tag = QString("[XXX]"),const QString &text=QString(),
                     const QPointF &ps  = QPointF(100.0,100.0),
                     const QFont   &fnt = QFont("Times", 14, QFont::Bold),
                     const QColor  &col = Qt::black);



private:
    qreal m_angle;
    typedef QPair<myTextItem*,QPointF>     ItemPos;
    QList<ItemPos>   m_TextItems;        // currently selected textitem & start positions
    QUndoStack*      m_undoStack;        // undo stack for undo & redo of commands

    int m_Number;                        // ��� �������� ����� ��� �������� ����� �� 0 �� 8
    //���� ��� ���������
    QAction *changeFontAction;
    QAction *changeColorAction;
    QAction *rotateRightAction;
    QAction *rotateLeftAction;
    QAction *setTextAction;
    QAction *delElemAction;
};

#endif // MYSCENE_H
