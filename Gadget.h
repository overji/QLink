//
// Created by overji on 2024/6/28.
//

#pragma once
#include <QWidget>
#include "LinkGame.h"
#include "SaveSystem.h"

class LinkGame;
class Player;
class SaveSystem;

class Gadget: public QWidget
{
    Q_OBJECT
    //设置为友元类，方便访问LinkGame的私有成员
    friend class LinkGame;
    friend class Player;
    friend class SaveSystem;
public:
    void drawGarget(QPainter & painter); //绘制道具
    explicit Gadget(LinkGame * game, int gameType = 1); //构造函数
    int gargetType;// 1:plus1s 2:shuffle 3:hint 4:flash 5:freeze 6:dizzy
    void singleGameGadgetEffect(LinkGame * game, Player * player); //单人游戏的道具，效果从Player方面传入
    void doubleGameGadgetEffect(LinkGame * game, Player * anotherPlayer); //双人游戏的道具，效果从Player方面传入
private:
    void randomRowLoc(LinkGame * game,int colLoc); //随机生成道具的位置
    void plus30sEffect(LinkGame * game); //加30秒效果
    void shuffleEffect(LinkGame * game); //shuffle效果
    void hintEffect(LinkGame * game); //hint效果
    void flashEffect(LinkGame *game,Player *player);    //flash效果
    void freezeEffect(LinkGame *game,Player * player); //freeze效果(双人游戏)
    void dizzyEffect(LinkGame *game,Player * player); //dizzy效果(双人游戏)
    int leftTopX;
    int leftTopY;
    int gadgetWidth;
    int gadgetHeight;
    QPixmap gadgetMap;
};

