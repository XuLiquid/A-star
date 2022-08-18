#include "maze_ui.h"
#include "ui_maze_ui.h"
#include "QGraphicsItem"
#include"myitem.h"
#include"myscene.h"
#include<QList>
#include<QPoint>
#include"queue.h"
#include"math.h"
#include<QMutex>
#include<iostream>
using namespace std;


int MyItem::state=0;
MyItem* MyItem::start_item;
MyItem* MyItem::end_item;

void maze_ui::paint()    //初始化
{
    int i=0;
    int j=0;
    for(i=0;i<30;i++)
    {
        for(j=0;j<20;j++)
        {
            rect[i][j].setRect(i*30, j*30, 30, 30);

            text[i][j].setTextWidth(30);
            text[i][j].setPos(i*30,j*30);
            text[i][j].setDefaultTextColor(QColor(255, 255, 255));  // 文本色
            QFont font = text[i][j].font();
            font.setPixelSize(8);
            text[i][j].setFont(font);
            scene->addItem(&rect[i][j]);
            scene->addItem(&text[i][j]);
        }
    }

    ui->graphicsView->setScene(scene);
    ui->graphicsView->show();

}

maze_ui::maze_ui(QWidget *parent) : QWidget(parent)  , ui(new Ui::maze_ui)
{
    setWindowFlags(Qt::FramelessWindowHint|windowFlags());
    state=0;
    ui->setupUi(this);
    ui->set->hide();
    ui->cancle->hide();
    paint();
}

maze_ui::~maze_ui()
{
    delete ui;
}





/************************************************* A* ********************************************************************/
int maze_ui::Astar_isContains(QList<MyItem*>* Nodelist ,MyItem* item)   //判断item是否在表中，是的话返回在表中的位置
{
    for (int i = 0;i < Nodelist->size();i++)
    {
        if (Nodelist->at(i)->rect().x() == item->rect().x() && Nodelist->at(i)->rect().y() == item->rect().y())
        {
            return i;
        }
    }
    return -1;
}

void maze_ui::Astar_countGHF(MyItem* sNode,MyItem* eNode,int g)   //计算g、h、f
{
    if(ui->comboBox->currentIndex()==3)       //判断是哪种类型的距离
    {
        sNode->h = sqrt(((sNode->rect().x()/30 - eNode->rect().x()/30) * 10)*((sNode->rect().x()/30 - eNode->rect().x()/30) * 10) + ((sNode->rect().y()/30 - eNode->rect().y()/30) * 10)*((sNode->rect().y()/30 - eNode->rect().y()/30) * 10));
    }
    else if(ui->comboBox->currentIndex()==4)
    {
        sNode->h = abs(sNode->rect().x()/30 - eNode->rect().x()/30) * 10 + abs(sNode->rect().y()/30 - eNode->rect().y()/30) * 10;
    }
    else if(ui->comboBox->currentIndex()==5)
    {
        if(abs(sNode->rect().x()/30 - eNode->rect().x()/30) * 10>abs(sNode->rect().y()/30 - eNode->rect().y()/30) * 10)
        {
            sNode->h = abs(sNode->rect().x()/30 - eNode->rect().x()/30) * 10;
        }
        else
        {
            sNode->h = abs(sNode->rect().y()/30 - eNode->rect().y()/30) * 10;
        }
    }

    sNode->g = sNode->father->g + g;
    sNode->f = sNode->g + sNode->h;
}
void maze_ui::Astar_checkPoit(MyItem* item,MyItem* father,int g)   //判断该点是否可以通过
{
    if (item->if_wall==1 || ((rect[int(item->rect().x()/30)][int(father->rect().y()/30)].if_wall==1) && (rect[int(father->rect().x()/30)][int(item->rect().y()/30)].if_wall==1)))
        return;
    if (Astar_isContains(&closeList,item) != -1)    //如果坐标超出范围或在closelist就return
        return;

    int index;
    if ((index = Astar_isContains(&openList,item)) != -1)    //如果已经存在openlist就对比当前节点到遍历到的那个节点的G值和当前节点到原来父节点的G值
    {
        MyItem* point = openList[index];
        if (point->g > father->g + g)                 //如果原来的G值比较大 不用管 否则重新赋值G值 父节点 和f
        {
            point->father = father;
            point->g = father->g + g;
            point->f = point->g + point->h;
        }
    }
    else                 //如果是新节点 加入到openlist
    {
        MyItem* point;
        point = item;
        point->father = father;
        Astar_countGHF(point,MyItem::end_item,g);
        openList.push_back(point);
    }

}
void maze_ui::Astar_NextStep(MyItem* current)          //探索周围的节点
{
    if (current->rect().x()/30 != 0)
    {
        Astar_checkPoit(&rect[int(current->rect().x()/30)-1][int(current->rect().y()/30)],current,10);//左
        if(current->rect().y()/30 != 0)
        {
            Astar_checkPoit(&rect[int(current->rect().x()/30)-1][int(current->rect().y()/30)-1],current,14);//左上
        }
        if(current->rect().y()/30 != 19)
        {
            Astar_checkPoit(&rect[int(current->rect().x()/30)-1][int(current->rect().y()/30)+1],current,14);//左下
        }
    }
    if(current->rect().x()/30 != 29)
    {
        Astar_checkPoit(&rect[int(current->rect().x()/30)+1][int(current->rect().y()/30)],current,10);//右
        if(current->rect().y()/30 != 0)
        {
            Astar_checkPoit(&rect[int(current->rect().x()/30)+1][int(current->rect().y()/30)-1],current,14);//右上
        }
        if(current->rect().y()/30 != 19)
        {
            Astar_checkPoit(&rect[int(current->rect().x()/30)+1][int(current->rect().y()/30)+1],current,14);//右下
        }
    }
    if(current->rect().y()/30 != 0)
    {
        Astar_checkPoit(&rect[int(current->rect().x()/30)][int(current->rect().y()/30)-1],current,10);//上
    }
    if(current->rect().y()/30 != 19)
    {
        Astar_checkPoit(&rect[int(current->rect().x()/30)][int(current->rect().y()/30)+1],current,10);//下
    }
}
void maze_ui::Astar_sort()         //按f从小到大的顺序排序openList
{
    int i,j;
    for(i=0;i<openList.length();i++)
    {
        for(j=0;j<openList.count()-i-1;j++)
        {
            if(openList.at(j)->f>openList.at(j+1)->f)
            {
                openList.swapItemsAt(j,j+1);
            }
        }
    }
}
void maze_ui::Astar_printPath(MyItem* current)
{
    int i;
    for(i=0;i<closeList.count();i++)
    {
        QString str=QString::number(closeList.at(i)->h,10)+'\n'+QString::number(closeList.at(i)->g,10)+'\n'+QString::number(closeList.at(i)->f,10);
        text[int(closeList.at(i)->rect().x()/30)][int(closeList.at(i)->rect().y()/30)].setPlainText(str);
        rect[int(closeList.at(i)->rect().x()/30)][int(closeList.at(i)->rect().y()/30)].setBrush(QBrush(QColor(0,180,200)));
    }
    for(i=0;i<openList.count();i++)
    {
        QString str=QString::number(openList.at(i)->h,10)+'\n'+QString::number(openList.at(i)->g,10)+'\n'+QString::number(openList.at(i)->f,10);
        text[int(openList.at(i)->rect().x()/30)][int(openList.at(i)->rect().y()/30)].setPlainText(str);
        rect[int(openList.at(i)->rect().x()/30)][int(openList.at(i)->rect().y()/30)].setBrush(QBrush(QColor(120,120,120)));
    }
    while(current)
    {
        current->setBrush(QBrush(QColor(0,200,0)));
        QString str=QString::number(current->h,10)+'\n'+QString::number(current->g,10)+'\n'+QString::number(current->f,10);
        text[int(current->rect().x()/30)][int(current->rect().y()/30)].setPlainText(str);
        current=current->father;
    }
    MyItem::start_item->setBrush(QBrush(QColor(0, 0, 255)));
    MyItem::end_item->setBrush(QBrush(QColor(255, 0, 0)));
}
void maze_ui::Astar_search()
{
    MyItem* current;
    MyItem* start=MyItem::start_item;
    MyItem* end=MyItem::end_item;
    openList.push_back(start);
    if(ui->comboBox->currentIndex()==3)
    {
        start->h = sqrt(((start->rect().x()/30 - end->rect().x()/30) * 10)*((start->rect().x()/30 - end->rect().x()/30) * 10) + ((start->rect().y()/30 - end->rect().y()/30) * 10)*((start->rect().y()/30 - end->rect().y()/30) * 10));
    }
    else if(ui->comboBox->currentIndex()==4)
    {
        start->h = abs(start->rect().x()/30 - end->rect().x()/30) * 10 + abs(start->rect().y()/30 - end->rect().y()/30) * 10;
    }
    else if(ui->comboBox->currentIndex()==5)
    {
        if(abs(start->rect().x()/30 - end->rect().x()/30) * 10 > abs(start->rect().y()/30 - end->rect().y()/30) * 10)
        {
            start->h = abs(start->rect().x()/30 - end->rect().x()/30) * 10;
        }
        else
        {
            start->h = abs(start->rect().y()/30 - end->rect().y()/30) * 10;
        }
    }
    start->f=start->h;
    start->g=0;
    //把开始的节点放入openlist后开始查找旁边的8个节点，如果坐标超出范围或在closelist就return
    //如果已经存在openlist就对比当前节点到遍历到的那个节点的G值和当前节点到原来父节点的G值
    //如果原来的G值比较大 不用管 否则重新赋值G值 父节点 和f 如果是新节点 加入到openlist直到opellist为空或找到终点
    while(openList.size() > 0)
    {
        current = openList.at(0);
        if (current->rect().x() == end->rect().x() && current->rect().y() == end->rect().y())
        {
            Astar_printPath(current);
            openList.clear();
            closeList.clear();
            break;
        }
        Astar_NextStep(current);
        closeList.push_back(current);
        openList.erase(openList.begin());
        Astar_sort();
    }

}
void maze_ui::Astar_step_search()
{
    MyItem* current;
    MyItem* start=MyItem::start_item;
    MyItem* end=MyItem::end_item;
    openList.push_back(start);
    if(ui->comboBox->currentIndex()==3)
    {
        start->h = sqrt(((start->rect().x()/30 - end->rect().x()/30) * 10)*((start->rect().x()/30 - end->rect().x()/30) * 10) + ((start->rect().y()/30 - end->rect().y()/30) * 10)*((start->rect().y()/30 - end->rect().y()/30) * 10));
    }
    else if(ui->comboBox->currentIndex()==4)
    {
        start->h = abs(start->rect().x()/30 - end->rect().x()/30) * 10 + abs(start->rect().y()/30 - end->rect().y()/30) * 10;
    }
    else if(ui->comboBox->currentIndex()==5)
    {
        if(abs(start->rect().x()/30 - end->rect().x()/30) * 10 > abs(start->rect().y()/30 - end->rect().y()/30) * 10)
        {
            start->h = abs(start->rect().x()/30 - end->rect().x()/30) * 10;
        }
        else
        {
            start->h = abs(start->rect().y()/30 - end->rect().y()/30) * 10;
        }
    }
    start->f=start->h;
    start->g=0;
    for(int i=0;i<step;i++)
    {
        current = openList.at(0);
        if (current->rect().x() == end->rect().x() && current->rect().y() == end->rect().y())
        {
            Astar_printPath(current);
            openList.clear();
            closeList.clear();
            break;
        }
        Astar_NextStep(current);
        closeList.push_back(current);
        openList.erase(openList.begin());
        Astar_sort();
    }
    Astar_printPath(current);
    openList.clear();
    closeList.clear();

}
/************************************************* A* ********************************************************************/

/************************************************** DFS **************************************************************/
int maze_ui::end_near(MyItem* current)
{
    if(MyItem::end_item==&rect[int(current->rect().x()/30)-1][int(current->rect().y()/30)]||
    MyItem::end_item==&rect[int(current->rect().x()/30)+1][int(current->rect().y()/30)]||
    MyItem::end_item==&rect[int(current->rect().x()/30)][int(current->rect().y()/30)-1]||
    MyItem::end_item==&rect[int(current->rect().x()/30)][int(current->rect().y()/30)+1])
    {
        return 10;
    }
    else if(
    MyItem::end_item==&rect[int(current->rect().x()/30)-1][int(current->rect().y()/30)-1]||
    MyItem::end_item==&rect[int(current->rect().x()/30)-1][int(current->rect().y()/30)+1]||
    MyItem::end_item==&rect[int(current->rect().x()/30)+1][int(current->rect().y()/30)-1]||
    MyItem::end_item==&rect[int(current->rect().x()/30)+1][int(current->rect().y()/30)+1])
    {
        return 14;
    }
    else
    {
        return 0;
    }

}
void maze_ui::DFS_CheckPoit(MyItem *item, MyItem *father,int g)
{
    if (item->if_wall==1 || ((rect[int(item->rect().x()/30)][int(father->rect().y()/30)].if_wall==1) && (rect[int(father->rect().x()/30)][int(item->rect().y()/30)].if_wall==1)))
    {
        return;
    }
    if (Astar_isContains(&closeList,item) != -1)
    {
        if (father->g+g<item->g)
        {
            item->g=father->g+g;
            item->father=father;
            openList.push_back(item);
        }
    }
    else
    {
        item->g=father->g+g;
        item->father = father;
        openList.push_back(item);
        closeList.push_back(item);
    }

}
void maze_ui::DFS_NextStep(MyItem *current)
{
    if(MyItem::end_item->g<=current->g)
    {
        return;
    }
    int en=end_near(current);
    if(en!=0)
    {
        if(current->g+en<MyItem::end_item->g)
        {
            MyItem::end_item->g=current->g+en;
            MyItem::end_item->father=current;
            current=MyItem::end_item;
        }
        return;
    }


    if (current->rect().x()/30 != 0)
    {
        DFS_CheckPoit(&rect[int(current->rect().x()/30)-1][int(current->rect().y()/30)],current,10);//左
        if(current->rect().y()/30 != 0)
        {
            DFS_CheckPoit(&rect[int(current->rect().x()/30)-1][int(current->rect().y()/30)-1],current,14);//左上
        }
        if(current->rect().y()/30 != 19)
        {
            DFS_CheckPoit(&rect[int(current->rect().x()/30)-1][int(current->rect().y()/30)+1],current,14);//左下
        }
    }
    if(current->rect().x()/30 != 29)
    {
        DFS_CheckPoit(&rect[int(current->rect().x()/30)+1][int(current->rect().y()/30)],current,10);//右
        if(current->rect().y()/30 != 0)
        {
            DFS_CheckPoit(&rect[int(current->rect().x()/30)+1][int(current->rect().y()/30)-1],current,14);//右上
        }
        if(current->rect().y()/30 != 19)
        {
            DFS_CheckPoit(&rect[int(current->rect().x()/30)+1][int(current->rect().y()/30)+1],current,14);//右下
        }
    }
    if(current->rect().y()/30 != 0)
    {
        DFS_CheckPoit(&rect[int(current->rect().x()/30)][int(current->rect().y()/30)-1],current,10);//上
    }
    if(current->rect().y()/30 != 19)
    {
        DFS_CheckPoit(&rect[int(current->rect().x()/30)][int(current->rect().y()/30)+1],current,10);//下
    }
}
void maze_ui::DFS_printPath(MyItem *current)
{
    int i=0;
    for(i=0;i<closeList.count();i++)
    {
        QString str='\n'+QString::number(closeList.at(i)->g,10)+'\n';
        text[int(closeList.at(i)->rect().x()/30)][int(closeList.at(i)->rect().y()/30)].setPlainText(str);
        rect[int(closeList.at(i)->rect().x()/30)][int(closeList.at(i)->rect().y()/30)].setBrush(QBrush(QColor(0,180,200)));
    }
    for(i=0;i<openList.count();i++)
    {
        QString str='\n'+QString::number(openList.at(i)->g,10)+'\n';
        text[int(openList.at(i)->rect().x()/30)][int(openList.at(i)->rect().y()/30)].setPlainText(str);
        rect[int(openList.at(i)->rect().x()/30)][int(openList.at(i)->rect().y()/30)].setBrush(QBrush(QColor(120,120,120)));
    }
    while(current)
    {
        current->setBrush(QBrush(QColor(0,200,0)));
        QString str='\n'+QString::number(current->g,10)+'\n';
        text[int(current->rect().x()/30)][int(current->rect().y()/30)].setPlainText(str);
        current=current->father;
    }
    MyItem::start_item->setBrush(QBrush(QColor(0, 0, 255)));
    MyItem::end_item->setBrush(QBrush(QColor(255, 0, 0)));
}
void maze_ui::DFS_search()
{
    MyItem* current;
    MyItem* start=MyItem::start_item;
    MyItem* end=MyItem::end_item;
    openList.push_back(start);
    closeList.push_back(start);
    start->g=0;
    end->g=INT_MAX;
    while(openList.size() > 0)
    {
        current = openList.back();
        openList.pop_back();
        DFS_NextStep(current);
    }
    DFS_printPath(end);
    openList.clear();
    closeList.clear();
}
void maze_ui::DFS_step_search()
{
    MyItem* current;
    MyItem* start=MyItem::start_item;
    MyItem* end=MyItem::end_item;
    openList.push_back(start);
    closeList.push_back(start);
    start->g=0;
    end->g=INT_MAX;
    for(int i=0;i<step;i++)
    {
        current = openList.back();
        openList.pop_back();
        DFS_NextStep(current);
        if(openList.empty())
        {
            current=end;
            break;
        }
    }
    DFS_printPath(current);
    openList.clear();
    closeList.clear();
}

/************************************************** DFS **************************************************************/


/************************************************** BFS ********************************************************/
int maze_ui::BFS_isContains(QList<MyItem*>* Nodelist ,MyItem* item)   //判断item是否在表中，是的话返回在表中的位置
{
    for (int i = 0;i < Nodelist->size();i++)
    {
        if (Nodelist->at(i)->rect().x() == item->rect().x() && Nodelist->at(i)->rect().y() == item->rect().y())
        {
            return i;
        }
    }
    return -1;
}

void maze_ui::BFS_countGHF(MyItem* sNode,int g)   //计算g、h、f
{
    sNode->g = sNode->father->g + g;
    sNode->f = sNode->g;
}
void maze_ui::BFS_checkPoit(MyItem* item,MyItem* father,int g)   //判断该点是否可以通过
{
    if (item->if_wall==1 || ((rect[int(item->rect().x()/30)][int(father->rect().y()/30)].if_wall==1) && (rect[int(father->rect().x()/30)][int(item->rect().y()/30)].if_wall==1)))
        return;
    if (Astar_isContains(&closeList,item) != -1)    //如果坐标超出范围或在closelist就return
        return;
    int index;
    if ((index = Astar_isContains(&openList,item)) != -1)    //如果已经存在openlist就对比当前节点到遍历到的那个节点的G值和当前节点到原来父节点的G值
    {
        MyItem* point = openList[index];
        if (point->g > father->g + g)                 //如果原来的G值比较大 不用管 否则重新赋值G值 父节点 和f
        {
            point->father = father;
            point->g = father->g + g;
            point->f = point->g + point->h;
        }
    }
    else                 //如果是新节点 加入到openlist
    {
        MyItem* point;
        point = item;
        point->father = father;
        BFS_countGHF(point,g);
        openList.push_back(point);
    }

}
void maze_ui::BFS_NextStep(MyItem* current)          //探索周围的节点
{
    if (current->rect().x()/30 != 0)
    {
        BFS_checkPoit(&rect[int(current->rect().x()/30)-1][int(current->rect().y()/30)],current,10);//左
        if(current->rect().y()/30 != 0)
        {
            BFS_checkPoit(&rect[int(current->rect().x()/30)-1][int(current->rect().y()/30)-1],current,14);//左上
        }
        if(current->rect().y()/30 != 19)
        {
            BFS_checkPoit(&rect[int(current->rect().x()/30)-1][int(current->rect().y()/30)+1],current,14);//左下
        }
    }
    if(current->rect().x()/30 != 29)
    {
        BFS_checkPoit(&rect[int(current->rect().x()/30)+1][int(current->rect().y()/30)],current,10);//右
        if(current->rect().y()/30 != 0)
        {
            BFS_checkPoit(&rect[int(current->rect().x()/30)+1][int(current->rect().y()/30)-1],current,14);//右上
        }
        if(current->rect().y()/30 != 19)
        {
            BFS_checkPoit(&rect[int(current->rect().x()/30)+1][int(current->rect().y()/30)+1],current,14);//右下
        }
    }
    if(current->rect().y()/30 != 0)
    {
        BFS_checkPoit(&rect[int(current->rect().x()/30)][int(current->rect().y()/30)-1],current,10);//上
    }
    if(current->rect().y()/30 != 19)
    {
        BFS_checkPoit(&rect[int(current->rect().x()/30)][int(current->rect().y()/30)+1],current,10);//下
    }
}
void maze_ui::BFS_sort()         //按f从小到大的顺序排序openList
{
    int i,j;
    for(i=0;i<openList.length();i++)
    {
        for(j=0;j<openList.count()-i-1;j++)
        {
            if(openList.at(j)->f>openList.at(j+1)->f)
            {
                openList.swapItemsAt(j,j+1);
            }
        }
    }
}
void maze_ui::BFS_printPath(MyItem* current)
{
    int i;
    for(i=0;i<closeList.count();i++)
    {
        QString str='\n'+QString::number(closeList.at(i)->f,10);
        text[int(closeList.at(i)->rect().x()/30)][int(closeList.at(i)->rect().y()/30)].setPlainText(str);
        rect[int(closeList.at(i)->rect().x()/30)][int(closeList.at(i)->rect().y()/30)].setBrush(QBrush(QColor(0,180,200)));
    }
    for(i=0;i<openList.count();i++)
    {
        QString str='\n'+QString::number(openList.at(i)->f,10);
        text[int(openList.at(i)->rect().x()/30)][int(openList.at(i)->rect().y()/30)].setPlainText(str);
        rect[int(openList.at(i)->rect().x()/30)][int(openList.at(i)->rect().y()/30)].setBrush(QBrush(QColor(120,120,120)));
    }
    while(current)
    {
        current->setBrush(QBrush(QColor(0,200,0)));
        QString str='\n'+QString::number(current->f,10);
        text[int(current->rect().x()/30)][int(current->rect().y()/30)].setPlainText(str);
        current=current->father;
    }
    MyItem::start_item->setBrush(QBrush(QColor(0, 0, 255)));
    MyItem::end_item->setBrush(QBrush(QColor(255, 0, 0)));
}
void maze_ui::BFS_search()
{
    MyItem* current;
    MyItem* start=MyItem::start_item;
    MyItem* end=MyItem::end_item;
    openList.push_back(start);
    start->f=0;
    start->g=0;
    //把开始的节点放入openlist后开始查找旁边的8个节点，如果坐标超出范围或在closelist就return
    //如果已经存在openlist就对比当前节点到遍历到的那个节点的G值和当前节点到原来父节点的G值
    //如果原来的G值比较大 不用管 否则重新赋值G值 父节点 和f 如果是新节点 加入到openlist直到opellist为空或找到终点
    while(openList.size() > 0)
    {
        current = openList.at(0);
        if (current->rect().x() == end->rect().x() && current->rect().y() == end->rect().y())
        {
            BFS_printPath(current);
            openList.clear();
            closeList.clear();
            break;
        }
        BFS_NextStep(current);
        closeList.push_back(current);
        openList.erase(openList.begin());
        BFS_sort();
    }

}
void maze_ui::BFS_step_search()
{
    MyItem* current;
    MyItem* start=MyItem::start_item;
    MyItem* end=MyItem::end_item;
    openList.push_back(start);
    start->f=0;
    start->g=0;
    //把开始的节点放入openlist后开始查找旁边的8个节点，如果坐标超出范围或在closelist就return
    //如果已经存在openlist就对比当前节点到遍历到的那个节点的G值和当前节点到原来父节点的G值
    //如果原来的G值比较大 不用管 否则重新赋值G值 父节点 和f 如果是新节点 加入到openlist直到opellist为空或找到终点
    for(int i=0;i<step;i++)
    {
        current = openList.at(0);
        if (current->rect().x() == end->rect().x() && current->rect().y() == end->rect().y())
        {
            BFS_printPath(current);
            openList.clear();
            closeList.clear();
            break;
        }
        BFS_NextStep(current);
        closeList.push_back(current);
        openList.erase(openList.begin());
        BFS_sort();
    }
    BFS_printPath(current);
    openList.clear();
    closeList.clear();
}

/************************************************** BFS ********************************************************/




/************************************************** Greedy ********************************************************/
int maze_ui::Greedy_isContains(QList<MyItem*>* Nodelist ,MyItem* item)   //判断item是否在表中，是的话返回在表中的位置
{
    for (int i = 0;i < Nodelist->size();i++)
    {
        if (Nodelist->at(i)->rect().x() == item->rect().x() && Nodelist->at(i)->rect().y() == item->rect().y())
        {
            return i;
        }
    }
    return -1;
}

void maze_ui::Greedy_countGHF(MyItem* sNode,MyItem* eNode)   //计算g、h、f
{
    sNode->h = sqrt(((sNode->rect().x()/30 - eNode->rect().x()/30) * 10)*((sNode->rect().x()/30 - eNode->rect().x()/30) * 10) + ((sNode->rect().y()/30 - eNode->rect().y()/30) * 10)*((sNode->rect().y()/30 - eNode->rect().y()/30) * 10));
    sNode->g = 0;
    sNode->f = sNode->g + sNode->h;
}
void maze_ui::Greedy_checkPoit(MyItem* item,MyItem* father)   //判断该点是否可以通过
{
    if (item->if_wall==1 || ((rect[int(item->rect().x()/30)][int(father->rect().y()/30)].if_wall==1) && (rect[int(father->rect().x()/30)][int(item->rect().y()/30)].if_wall==1)))
        return;
    if (Greedy_isContains(&closeList,item) != -1)    //如果坐标超出范围或在closelist就return
        return;
    if (Greedy_isContains(&openList,item) == -1)    //如果已经存在openlist就对比当前节点到遍历到的那个节点的G值和当前节点到原来父节点的G值
    {
        MyItem* point;
        point = item;
        point->father = father;
        Greedy_countGHF(point,MyItem::end_item);
        openList.push_back(point);
    }
}
void maze_ui::Greedy_NextStep(MyItem* current)          //探索周围的节点
{
    if (current->rect().x()/30 != 0)
    {
        Greedy_checkPoit(&rect[int(current->rect().x()/30)-1][int(current->rect().y()/30)],current);//左
        if(current->rect().y()/30 != 0)
        {
            Greedy_checkPoit(&rect[int(current->rect().x()/30)-1][int(current->rect().y()/30)-1],current);//左上
        }
        if(current->rect().y()/30 != 19)
        {
            Greedy_checkPoit(&rect[int(current->rect().x()/30)-1][int(current->rect().y()/30)+1],current);//左下
        }
    }
    if(current->rect().x()/30 != 29)
    {
        Greedy_checkPoit(&rect[int(current->rect().x()/30)+1][int(current->rect().y()/30)],current);//右
        if(current->rect().y()/30 != 0)
        {
            Greedy_checkPoit(&rect[int(current->rect().x()/30)+1][int(current->rect().y()/30)-1],current);//右上
        }
        if(current->rect().y()/30 != 19)
        {
            Greedy_checkPoit(&rect[int(current->rect().x()/30)+1][int(current->rect().y()/30)+1],current);//右下
        }
    }
    if(current->rect().y()/30 != 0)
    {
        Greedy_checkPoit(&rect[int(current->rect().x()/30)][int(current->rect().y()/30)-1],current);//上
    }
    if(current->rect().y()/30 != 19)
    {
        Greedy_checkPoit(&rect[int(current->rect().x()/30)][int(current->rect().y()/30)+1],current);//下
    }
}
void maze_ui::Greedy_sort()         //按f从小到大的顺序排序openList
{
    int i,j;
    for(i=0;i<openList.length();i++)
    {
        for(j=0;j<openList.count()-i-1;j++)
        {
            if(openList.at(j)->f>openList.at(j+1)->f)
            {
                openList.swapItemsAt(j,j+1);
            }
        }
    }
}
void maze_ui::Greedy_printPath(MyItem* current)
{
    int i;
    for(i=0;i<closeList.count();i++)
    {
        QString str='\n'+QString::number(closeList.at(i)->f,10);
        text[int(closeList.at(i)->rect().x()/30)][int(closeList.at(i)->rect().y()/30)].setPlainText(str);
        rect[int(closeList.at(i)->rect().x()/30)][int(closeList.at(i)->rect().y()/30)].setBrush(QBrush(QColor(0,180,200)));
    }
    for(i=0;i<openList.count();i++)
    {
        QString str='\n'+QString::number(openList.at(i)->f,10);
        text[int(openList.at(i)->rect().x()/30)][int(openList.at(i)->rect().y()/30)].setPlainText(str);
        rect[int(openList.at(i)->rect().x()/30)][int(openList.at(i)->rect().y()/30)].setBrush(QBrush(QColor(120,120,120)));
    }
    while(current)
    {
        current->setBrush(QBrush(QColor(0,200,0)));
        QString str='\n'+QString::number(current->f,10);
        text[int(current->rect().x()/30)][int(current->rect().y()/30)].setPlainText(str);
        current=current->father;
    }
    MyItem::start_item->setBrush(QBrush(QColor(0, 0, 255)));
    MyItem::end_item->setBrush(QBrush(QColor(255, 0, 0)));
}
void maze_ui::Greedy_search()
{
    MyItem* current;
    MyItem* start=MyItem::start_item;
    MyItem* end=MyItem::end_item;
    openList.push_back(start);
    start->h = sqrt(((start->rect().x()/30 - end->rect().x()/30) * 10)*((start->rect().x()/30 - end->rect().x()/30) * 10) + ((start->rect().y()/30 - end->rect().y()/30) * 10)*((start->rect().y()/30 - end->rect().y()/30) * 10));
    start->f=start->h;
    start->g=0;
    //把开始的节点放入openlist后开始查找旁边的8个节点，如果坐标超出范围或在closelist就return
    //如果已经存在openlist就对比当前节点到遍历到的那个节点的G值和当前节点到原来父节点的G值
    //如果原来的G值比较大 不用管 否则重新赋值G值 父节点 和f 如果是新节点 加入到openlist直到opellist为空或找到终点
    while(openList.size() > 0)
    {
        current = openList.at(0);
        if (current->rect().x() == end->rect().x() && current->rect().y() == end->rect().y())
        {
            Greedy_printPath(current);
            openList.clear();
            closeList.clear();
            break;
        }
        Greedy_NextStep(current);
        closeList.push_back(current);
        openList.erase(openList.begin());
        Greedy_sort();
    }
}
void maze_ui::Greedy_step_search()
{
    MyItem* current;
    MyItem* start=MyItem::start_item;
    MyItem* end=MyItem::end_item;
    openList.push_back(start);
    start->h = sqrt(((start->rect().x()/30 - end->rect().x()/30) * 10)*((start->rect().x()/30 - end->rect().x()/30) * 10) + ((start->rect().y()/30 - end->rect().y()/30) * 10)*((start->rect().y()/30 - end->rect().y()/30) * 10));
    start->f=start->h;
    start->g=0;
    //把开始的节点放入openlist后开始查找旁边的8个节点，如果坐标超出范围或在closelist就return
    //如果已经存在openlist就对比当前节点到遍历到的那个节点的G值和当前节点到原来父节点的G值
    //如果原来的G值比较大 不用管 否则重新赋值G值 父节点 和f 如果是新节点 加入到openlist直到opellist为空或找到终点
    for(int i=0;i<step;i++)
    {
        current = openList.at(0);
        if (current->rect().x() == end->rect().x() && current->rect().y() == end->rect().y())
        {
            Greedy_printPath(current);
            openList.clear();
            closeList.clear();
            break;
        }
        Greedy_NextStep(current);
        closeList.push_back(current);
        openList.erase(openList.begin());
        Greedy_sort();
    }
    Greedy_printPath(current);
    openList.clear();
    closeList.clear();
}
/************************************************** Greedy ********************************************************/




void maze_ui::delete_wall()    //清空所有障碍
{
    int i=0;
    int j=0;
    for(i=0;i<30;i++)
    {
        for(j=0;j<20;j++)
        {
            rect[i][j].if_wall=-1;
            rect[i][j].setBrush(QBrush(QColor(150, 150, 150)));
            text[i][j].setPlainText("");
        }
    }
}

void maze_ui::on_set_maze_clicked()  //建迷宫
{
    if(state==0)
    {
        state=1;
        MyItem::state=1;
        ui->set->show();
        ui->cancle->show();
    }
}

void maze_ui::on_set_clicked()   //确认键
{
    MyItem::state=0;
    ui->set->hide();
    ui->cancle->hide();
    state=0;
}

void maze_ui::on_cancle_clicked()   //清空键
{
    if(state==1)
    {
        delete_wall();
        MyItem::state=0;
        ui->set->hide();
        ui->cancle->hide();
        state=0;
    }
    else
    {
        for(int i=0;i<30;i++)
        {
            for(int j=0;j<20;j++)
            {
                if(&rect[i][j]==MyItem::start_item)
                {
                    rect[i][j].setBrush(QBrush(QColor(0, 0, 255)));
                    text[i][j].setPlainText("");
                }
                else if(&rect[i][j]== MyItem::end_item)
                {
                    rect[i][j].setBrush(QBrush(QColor(255, 0, 0)));
                    text[i][j].setPlainText("");
                }
                else if(rect[i][j].if_wall==-1)
                {
                    rect[i][j].setBrush(QBrush(QColor(150, 150, 150)));
                    text[i][j].setPlainText("");
                }
            }
        }
        MyItem::state=0;
        ui->cancle->hide();
        state=0;
        step=0;
    }
}

void maze_ui::on_set_start_point_clicked()   //设置起点
{
    if(state==0)
    {
        state=2;
        MyItem::state=2;
        ui->set->show();
    }
}

void maze_ui::on_set_end_point_clicked()    //设置终点
{
    if(state==0)
    {
        state=3;
        MyItem::state=3;
        ui->set->show();
    }
}

void maze_ui::on_fast_start_clicked()   //寻路算法
{
    if(state==0&&MyItem::start_item!=nullptr&&MyItem::end_item!=nullptr)
    {
        state=4;
        if(ui->comboBox->currentIndex()==0)
        {
            BFS_search();
            ui->cancle->show();
        }
        else if(ui->comboBox->currentIndex()==1)
        {
            DFS_search();
            ui->cancle->show();
        }
        else if(ui->comboBox->currentIndex()==2)
        {
            Greedy_search();
            ui->cancle->show();
        }
        else if(ui->comboBox->currentIndex()==3)
        {
            Astar_search();
            ui->cancle->show();
        }
        else if(ui->comboBox->currentIndex()==4)
        {
            Astar_search();
            ui->cancle->show();
        }
        else if(ui->comboBox->currentIndex()==5)
        {
            Astar_search();
            ui->cancle->show();
        }
    }
}

void maze_ui::on_slow_start_clicked()    //逐步寻路
{
    if((state==5||state==0)&&MyItem::start_item!=nullptr&&MyItem::end_item!=nullptr)
    {
        state=5;
        step++;
        if(ui->comboBox->currentIndex()==0)
        {
            BFS_step_search();
            ui->cancle->show();
        }
        else if(ui->comboBox->currentIndex()==1)
        {
            DFS_step_search();
            ui->cancle->show();
        }
        else if(ui->comboBox->currentIndex()==2)
        {
            Greedy_step_search();
            ui->cancle->show();
        }
        else if(ui->comboBox->currentIndex()==3)
        {
            Astar_step_search();
            ui->cancle->show();
        }
        else if(ui->comboBox->currentIndex()==4)
        {
            Astar_step_search();
            ui->cancle->show();
        }
        else if(ui->comboBox->currentIndex()==5)
        {
            Astar_step_search();
            ui->cancle->show();
        }
    }
}

void maze_ui::on_close_clicked()
{
    this->close();
}
