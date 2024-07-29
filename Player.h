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
public slots:
    void removeBox(); //消除方块
public:
    explicit Player(const int &playerLeftTopXInput, const int &playerLeftTopYInput, const int &playerWidthInput,
                    const int &playerHeightInput, const int &playerSpeedInput,LinkGame * game,const int& playerSkin = 1); //构造函数
    ~Player(); //析构函数
    void resizePlayer(double xScaleRatio, double yScaleRatio); //调整玩家大小
    void drawPlayer(QPainter & painter); //绘制玩家
    void playerMove(const int &direction,const int &xLoc = 0,const int &yLoc = 0); //玩家移动
    void leftMove(); //玩家向左移动
    void rightMove(); //玩家向右移动
    void upMove(); //玩家向上移动
    void downMove(); //玩家向下移动
    void flashMove(int xLoc,int yLoc); //玩家闪移
    void checkGadgetHit(); //检查玩家是否碰到道具
    void drawLine(QPainter & painter); //绘制线
private:
    void checkClose(); //检查是否有相邻的方块
    void clearSelected(); //消除选中的方块
    bool checkColIndexValid(const int & xIndex);     //检查列索引是否合法
    bool checkRowIndexValid(const int & yIndex);    //检查行索引是否合法
    bool flashMoveCheckEdge(int rowLoc,int colLoc); //检查flash移动是否处于箱子边缘处
    bool flashMoveCheckCenter(int rowLoc,int colLoc); //检查flash移动是否处于箱子中间
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

    QVector<QPoint> linePath; //路径
    QVector<QPair<int,int>> toBeRemovedBox; //待消除箱子
    QTimer * removeBoxTimer; //消除计时器
    bool removeTimerOn; //消除计时器是否开启
    LinkGame * game;
};

