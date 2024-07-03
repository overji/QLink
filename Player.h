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
    friend class LinkGame;
    friend class SelectChecker;
    friend class Gadget;
    friend class SimpleTest;
    friend class SaveSystem;
public:
    explicit Player(const int & playerLeftTopXInput , const int & playerLeftTopYInput , const int & playerWidthInput , const int & playerHeightInput , const int & playerSpeedInput,
    const int& playerSkin = 1);
    void resizePlayer(double xScaleRatio, double yScaleRatio);
    void drawPlayer(QPainter & painter);
    void playerMove(const int &direction,LinkGame * game,const int &xLoc = 0,const int &yLoc = 0);
    void leftMove(LinkGame * game);
    void rightMove(LinkGame * game);
    void upMove(LinkGame * game);
    void downMove(LinkGame * game);
    void flashMove(LinkGame * game,int xLoc,int yLoc);
    void checkGadgetHit(LinkGame * game);
private:
    void checkClose(LinkGame * game);
    void clearClose(LinkGame * game);
    void clearSelected(LinkGame * game);
    bool checkColIndexValid(const LinkGame * game, const int & xIndex);
    bool checkRowIndexValid(const LinkGame * game, const int & yIndex);
    bool flashMoveCheckEdge(LinkGame * game,int rowLoc,int colLoc);
    bool flashMoveCheckCenter(LinkGame * game,int rowLoc,int colLoc);
    int playerLeftTopX;
    int playerLeftTopY;
    int playerWidth;
    int playerHeight;
    int playerSpeed;
    int score;
    QString scoreString;
    QPixmap playerImage;
    QVector<QPair<int,int>> currentSelected;
};

