#ifndef QUEUE_H
#define QUEUE_H
#include"myitem.h"


class Queue
{
public:
    Queue(int nCount=10);
    QGraphicsRectItem* *m_pData;
    int m_nHead,m_nTail;
    int m_nCount;
    bool isEmpty();
    bool isFull();
    void push(QGraphicsRectItem *v);
    QGraphicsRectItem * pop();
};

#endif // QUEUE_H
