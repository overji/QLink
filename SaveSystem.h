//
// Created by overji on 2024/7/3.
//
#pragma once
#include "LinkGame.h"
#include "Player.h"
#include "Gadget.h"
#include "BoxOfGame.h"

class LinkGame;
class Player;
class Gadget;
class BoxOfGame;


class SaveSystem {
public:
    static void saveGame(LinkGame *game);
    static LinkGame * loadGame();
private:
    static void savePlayerData(QDataStream &out, Player *player);
    static void loadPlayerData(QDataStream &in, Player *player);
};


