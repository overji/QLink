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
    friend class LinkGame;
    friend class Player;
    friend class SaveSystem;
public:
    void drawGarget(QPainter & painter);;
    explicit Gadget(LinkGame * game, int gameType = 1);
    int gargetType;// 1:plus1s 2:shuffle 3:hint 4:flash 5:freeze 6:dizzy
    void singleGameGadgetEffect(LinkGame * game, Player * player); //单人游戏的道具，效果从Player方面传入
    void doubleGameGadgetEffect(LinkGame * game, Player * anotherPlayer); //双人游戏的道具，效果从Player方面传入
private:
    void randomRowLoc(LinkGame * game,int colLoc);
    void plus1sEffect(LinkGame * game);
    void shuffleEffect(LinkGame * game);
    void hintEffect(LinkGame * game);
    void flashEffect(LinkGame *game,Player *player);
    void freezeEffect(LinkGame *game,Player * player);
    void dizzyEffect(LinkGame *game,Player * player);
    int leftTopX;
    int leftTopY;
    int gadgetWidth;
    int gadgetHeight;
    QPixmap gadgetMap;
};

