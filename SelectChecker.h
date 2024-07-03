//
// Created by overji on 2024/6/24.
//
#pragma once
#include "LinkGame.h"
#include "Player.h"
class LinkGame;
class Player;

class SelectChecker
{
public:
    static void checkSelected(LinkGame * game, Player * player);
    static bool solutionExist(LinkGame * game,Player * player);
    static bool checkExist(LinkGame * game,Player * player,int x,int y);
    static bool checkValidReturnBool(LinkGame * game,Player * player);
    static QPair<QPair<int,int>,QPair<int,int>> searchPair(LinkGame * game);
private:
    static void checkNumber(LinkGame * game, Player * player);
    static QVector<QPair<int,int>> checkValid(LinkGame * game,Player * player);
    static bool checkLine(LinkGame * game,Player * player,int direction,int start,int end,int anotherLocation);
    static QVector<QPair<int,int>> twoTwistCheck(LinkGame * game,Player * player,int x1,int y1,int x2,int y2);
    static QVector<QPair<int,int>> getEdgeBox(LinkGame * game,Player * player);
    static bool isEdge(LinkGame * game, Player * player, int colLoc, int rowLoc);

};

