#ifndef MAZE_UI_H
#define MAZE_UI_H

#include <QWidget>
#include<QGraphicsSceneMouseEvent>
#include"myitem.h"
#include"myscene.h"
#include<QMutex>

QT_BEGIN_NAMESPACE
namespace Ui { class maze_ui; }
QT_END_NAMESPACE

class maze_ui : public QWidget
{
    Q_OBJECT

public:
    maze_ui(QWidget *parent = nullptr);
    ~maze_ui();
    void paint();
    MyScene *scene = new MyScene;
    MyItem rect[30][20];
    QGraphicsTextItem text[30][20];
    QGraphicsItemGroup group[30][20];
    int step=0;

protected:
    int state;
    void delete_wall();
    void set_state(int t);

    //A_star
    void Astar_search();
    void Astar_step_search();
    void Astar_checkPoit(MyItem* item,MyItem* father,int g);
    void Astar_NextStep(MyItem* currentPoint);
    int Astar_isContains(QList<MyItem*>* Nodelist ,MyItem* item);
    void Astar_countGHF(MyItem* sNode,MyItem* eNode,int g);
    void Astar_printPath(MyItem* current);
    void Astar_sort();

    void DFS_search();
    void DFS_step_search();
    void DFS_NextStep(MyItem* current);
    void DFS_CheckPoit(MyItem* item,MyItem* father,int g);
    void DFS_printPath(MyItem* current);
    int end_near(MyItem* current);

    void BFS_search();
    void BFS_step_search();
    void BFS_checkPoit(MyItem* item,MyItem* father,int g);
    void BFS_NextStep(MyItem* currentPoint);
    int BFS_isContains(QList<MyItem*>* Nodelist ,MyItem* item);
    void BFS_countGHF(MyItem* sNode,int g);
    void BFS_printPath(MyItem* current);
    void BFS_sort();

    void Greedy_search();
    void Greedy_step_search();
    void Greedy_checkPoit(MyItem* item,MyItem* father);
    void Greedy_NextStep(MyItem* currentPoint);
    int Greedy_isContains(QList<MyItem*>* Nodelist ,MyItem* item);
    void Greedy_countGHF(MyItem* sNode,MyItem* eNode);
    void Greedy_printPath(MyItem* current);
    void Greedy_sort();

    void print_step(MyItem* current);

    QList<MyItem*> openList;
    QList<MyItem*> closeList;

private slots:
    void on_set_maze_clicked();

    void on_set_clicked();

    void on_cancle_clicked();

    void on_set_start_point_clicked();

    void on_set_end_point_clicked();

    void on_fast_start_clicked();

    void on_slow_start_clicked();

    void on_close_clicked();

private:
    Ui::maze_ui *ui;
};
#endif // MAZE_UI_H
