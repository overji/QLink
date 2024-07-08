//
// Created by overji on 2024/6/20.
//

#pragma once
#include "LinkGame.h"
#include "SimpleTest.h"
#include "SaveSystem.h"
#include "Gadget.h"
#include <QWidget>

class LinkGame;
class SelectChecker;
class SaveSystem;
class Gadget;

class Player:public QWidget
{
    Q_OBJECT
    //设置为友元类，以便访问私有成员
    friend class LinkGame;
    friend class SelectChecker;
    friend class Gadget;
    friend class SimpleTest;
    friend class SaveSystem;
public:
    explicit Player(const int & playerLeftTopXInput , const int & playerLeftTopYInput , const int & playerWidthInput , const int & playerHeightInput , const int & playerSpeedInput,
    const int& playerSkin = 1); //构造函数
    ~Player(); //析构函数
    void resizePlayer(double xScaleRatio, double yScaleRatio); //调整玩家大小
    void drawPlayer(QPainter & painter); //绘制玩家
    void playerMove(const int &direction,LinkGame * game,const int &xLoc = 0,const int &yLoc = 0); //玩家移动
    void leftMove(LinkGame * game); //玩家向左移动
    void rightMove(LinkGame * game); //玩家向右移动
    void upMove(LinkGame * game); //玩家向上移动
    void downMove(LinkGame * game); //玩家向下移动
    void flashMove(LinkGame * game,int xLoc,int yLoc); //玩家闪移
    void checkGadgetHit(LinkGame * game); //检查玩家是否碰到道具
private:
    void checkClose(LinkGame * game); //检查是否有相邻的两个方块可以消除
    void clearClose(LinkGame * game); //消除相邻的两个方块
    void clearSelected(LinkGame * game); //消除选中的方块
    bool checkColIndexValid(const LinkGame * game, const int & xIndex);     //检查列索引是否合法
    bool checkRowIndexValid(const LinkGame * game, const int & yIndex);    //检查行索引是否合法
    bool flashMoveCheckEdge(LinkGame * game,int rowLoc,int colLoc); //检查flash移动是否处于箱子边缘处
    bool flashMoveCheckCenter(LinkGame * game,int rowLoc,int colLoc); //检查flash移动是否处于箱子中间
    int playerLeftTopX; //玩家左上角x坐标
    int playerLeftTopY; //玩家左上角y坐标
    int playerWidth; //玩家宽度
    int playerHeight; //玩家高度
    int playerSpeed; //玩家速度
    int score; //玩家分数
    int freezeTime; //玩家冻结时间
    int dizzyTime; //玩家眩晕时间
    QString scoreString; //玩家分数字符串
    QPixmap playerImage; //玩家图片
    QVector<QPair<int,int>> currentSelected;    //当前选中的方块
};

