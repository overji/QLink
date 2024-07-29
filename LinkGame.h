//
// Created by overji on 2024/6/20.
//

#pragma once

#include <QWidget>
#include <QTime>
#include <QPair>
#include <QVector>
#include <QQueue>
#include "BoxOfGame.h"
#include "Player.h"
#include "Gadget.h"
#include "SaveSystem.h"
#include "SelectChecker.h"
#include "SpecialAlgorithm.h"
#include "MainPage.h"

class Player;
class BoxOfGame;
class Gadget;
class SaveSystem;

class LinkGame : public QWidget
{
    Q_OBJECT
    //设置友元类，方便其他类访问私有成员
    friend class Player;
    friend class SelectChecker;
    friend class SimpleTest;
    friend class Gadget;
    friend class SaveSystem;
public:
    explicit LinkGame(const int& M = 10, const int& N = 10,int gameTypeInput = 0,const int &remainTimeInput = 0 ,QWidget * parent = nullptr); //构造函数
    ~LinkGame();    //析构函数
    void shuffleMap(); //重排地图
    void setGamePause(); //设置游戏暂停或继续
public slots:
    void updateRemainTime(); //更新剩余时间
    void updateHintTime(); //更新hint时间
    void updateFlashTime(); //更新flash时间
    void updateFreezeTime(); //更新freeze时间
    void updateDizzyTime(); //更新dizzy时间
    void startNewGame(); //开始新游戏
    void loadGame(); //加载游戏
private:
    void paintEvent(QPaintEvent *event) override; //绘制事件
    void resizeEvent(QResizeEvent *event) override; //重绘事件
    void keyPressEvent(QKeyEvent * event) override; //键盘事件
    void mousePressEvent(QMouseEvent * event) override; //鼠标点击事件
    void drawMap(QPainter & painter); //绘制地图
    void drawEndPage(QPainter & painter); //绘制结束页面
    void drawPausePage(QPainter & painter); //绘制暂停页面
    void initGlobalBox(const int& M,const int& N); //初始化箱子
    void initMap(); //初始化地图
    void initTimers(const int &remainTimeInput); //初始化计时器
    void initBoxType(); //初始化箱子类型
    void initPlayer(int gameTypeInput); //初始化玩家
    void gameUpdate(); //游戏更新
    void clearClose(); //消除相邻的两个方块

    void checkGameEnd(); //检查游戏是否结束
    void generateGadget(); //生成道具
    void hintBox(); //提示箱子
    void clearHintBox(); //清除提示箱子


    int gameFps; //游戏帧率
    int remainBoxNumber; //剩余箱子数量
    int boxCol; //箱子列数
    int boxRow; //箱子行数
    int boxWidth; //箱子宽度
    int boxHeight; //箱子高度
    int passageWidth; //通道宽度
    int passageHeight; //通道高度
    double xScaleRatio; //x轴缩放比例
    double yScaleRatio; //y轴缩放比例
    bool gameEnd; //游戏是否结束
    bool gamePause; //游戏是否暂停
    bool noSolution; //无解
    int remainTime; //剩余时间
    int gameType;//0是单人游戏，1是双人游戏

    QVector<QColor> typeToColor; //各个类型对应箱子颜色
    QVector<QVector<int>> boxType; //箱子类型
    QVector<QVector<BoxOfGame *>> boxMap; //箱子地图
    QString removeText; //消除提示字符串
    QString leftTimeText; //剩余时间提示字符串
    QString summaryText; //总结字符串
    int types; //箱子类型数

    QTimer * remainTimeTimer; //剩余时间计时器
    QTimer * flashTimeTimer; //flash计时器
    QTimer * hintTimeTimer; //hint计时器
    QTimer * freezeTimeTimer; //freeze计时器
    QTimer * dizzyTimeTimer; //dizzy计时器

    QVector<Gadget *> gadgets; //道具
    int maxGadgetNumber; //最大道具数量
    double gadgetSummonPossibility; //道具生成概率

    int hintTime; //提示时间
    bool hintTimerOn; //提示计时器是否开启
    QVector<QPair<int,int>>hintedBoxes; //提示箱子

    int flashTime; //flash时间
    bool flashTimerOn; //flash计时器是否开启

    bool dizzyTimerOn; //dizzy计时器是否开启
    bool freezeTimerOn; //freeze计时器是否开启

    Player * player1; //玩家1
    Player * player2; //玩家2
};

