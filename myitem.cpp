#include "myitem.h"
#include<QGraphicsItem>
#include<QGraphicsSceneEvent>
#include<QPen>
#include<QDebug>

MyItem::MyItem()
{
    father=NULL;
    state=0;
    if_wall=-1;
    g=0;
    h=0;
    f=0;
    setFlag(QGraphicsRectItem::ItemIsFocusable);
    QPen pen;   // 定义一个画笔，设置画笔颜色和宽度
    pen.setColor(QColor(100, 100, 100));
    pen.setWidth(3);
    setPen(pen);
    setBrush(QBrush(QColor(150, 150, 150)));
}


void MyItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(state==1)
    {
        if(this->if_wall==-1)
        {
            if_wall*=-1;
            setBrush(QBrush(QColor(0, 0, 0)));
        }
        else
        {
            if_wall*=-1;
            setBrush(QBrush(QColor(150, 150, 150)));
        }
    }
    else if(state==2)
    {
        if(this->if_wall==-1)
        {
            if(MyItem::start_item)
            {
                MyItem::start_item->setBrush(QBrush(QColor(150, 150, 150)));
            }
            setBrush(QBrush(QColor(0, 0, 255)));          //起点颜色
            MyItem::start_item=this;
        }
    }
    else if(state==3)
    {
        if(this->if_wall==-1)
        {
            if(MyItem::end_item)
            {
                MyItem::end_item->setBrush(QBrush(QColor(150, 150, 150)));
            }
            setBrush(QBrush(QColor(255, 0, 0)));         //终点颜色
            MyItem::end_item=this;
        }
    }
}
