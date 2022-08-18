#ifndef MYITEM_H
#define MYITEM_H
#include<QGraphicsItem>
#include<QGraphicsSceneEvent>


class MyItem:public QGraphicsRectItem
{
public:
    MyItem();
    static MyItem* start_item;
    static MyItem* end_item;
    static int state;
    int if_wall;
    MyItem *father;
    int g;
    int h;
    int f;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MYITEM_H
