#ifndef MYSCENE_H
#define MYSCENE_H
#include<QGraphicsScene>


class MyScene:public QGraphicsScene
{
public:
    MyScene();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

#endif // MYSCENE_H
