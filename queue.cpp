#include "queue.h"

Queue::Queue(int nCount)
{
    *m_pData = new QGraphicsRectItem[nCount];
    m_nHead = m_nTail = 0;
    m_nCount = nCount;
}

void Queue::push(QGraphicsRectItem *value)
{
    if(!isFull())
    {

        m_pData[m_nTail++] = value;

        if(m_nTail >= m_nCount)
          m_nTail = 0;
    }
}

QGraphicsRectItem * Queue::pop ()
{
    if(isEmpty())
        return NULL;
    QGraphicsRectItem *value = m_pData[m_nHead++];

    if(m_nHead >= m_nCount)
       m_nHead = 0;

    return value;
}

bool Queue::isFull()
{
     return ((m_nTail+1)%m_nCount == m_nHead);
}

bool Queue::isEmpty()
{
     return m_nHead == m_nTail;
}
