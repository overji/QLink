//
// Created by overji on 2024/6/20.
//

#pragma once

#include <QWidget>
#include "Player.h"
#include "SaveSystem.h"

class Player;
class Gadget;
class SaveSystem;

struct BoxState
{
    //箱子状态
    bool boxRemoved;
    bool boxSelected;
    bool closeBox;
    bool boxToBeRemoved;
    bool boxHinted;
};

class BoxOfGame : public QWidget
{
    Q_OBJECT
    //设置友元
    friend class Player;
    friend class SelectChecker;
    friend class SimpleTest;
    friend class LinkGame;
    friend class Gadget;
    friend class SaveSystem;
public:
    explicit BoxOfGame(const int & boxWid , const int & boxHeit,const int & LeftTopX ,const int & LeftTopY  ,const QColor boxColorInput
                       , const QColor boarderColorInput, const int TypeOfBoxInput , QWidget * parent = nullptr ); //构造函数
    void drawBox(QPainter & painter); //绘制箱子
    void resizeBox(const int & boxWid , const int & boxHeit,const int & leftTopX ,const int & leftTopY); //页面大小变换时调整箱子大小
    void static swapBox(BoxOfGame * box1,BoxOfGame * box2); //交换两个箱子
    int addBoxScore(); //返回箱子分数

private:
    //确定箱子边框大小、箱子大小、箱子含边界时候左上角位置
    void checkBoxBoarderColor(); //检查箱子边框颜色
    void initBoxPixMap(); //初始化箱子图片
    int boxWidth; //箱子宽度
    int boxHeight; //箱子高度
    int boarderWidth; //箱子边框宽度
    int boarderHeight; //箱子边框高度
    int LeftTopX;   //箱子左上角X坐标
    int LeftTopY;  //箱子左上角Y坐标
    int typeOfBox; //箱子类型
    QColor boxColor; //箱子颜色
    QColor boarderColor; //箱子边框颜色
    BoxState boxState; //箱子状态
    QPixmap boxPixmap;
};