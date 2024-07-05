//
// Created by overji on 2024/6/28.
//

#include "Gadget.h"
#include "LinkGame.h"
#include "SelectChecker.h"
#include "SpecialAlgorithm.h"
#include <QWidget>
#include <QPainter>
#include <iostream>

Gadget::Gadget(LinkGame * game, int gameType)
{
    this->gadgetWidth = game->boxWidth / 3;
    this->gadgetHeight = game->boxHeight / 3;
    this->leftTopX = QRandomGenerator::global()->bounded(1, 800 - gadgetWidth - 1);
    int colLoc = specialDiv(leftTopX-game->passageWidth,game->boxWidth);
    randomRowLoc(game,colLoc);
    if(gameType == 0){
        this->gargetType = QRandomGenerator::global()->bounded(1,5);
    } else {
        this->gargetType = QRandomGenerator::global()->bounded(1,6);
        if(this->gargetType == 4){
            this->gargetType = 6;
        }
    }
    switch(gargetType){
        case 1:
            this->gadgetMap = QPixmap("images/GargetImages/plus1s.png");
            break;
        case 2:
            this->gadgetMap = QPixmap("images/GargetImages/shuffle.png");
            break;
        case 3:
            this->gadgetMap = QPixmap("images/GargetImages/hint.png");
            break;
        case 4:
            this->gadgetMap = QPixmap("images/GargetImages/flash.png");
            break;
        case 5:
            this->gadgetMap = QPixmap("images/GargetImages/freeze.png");
            break;
        case 6:
            this->gadgetMap = QPixmap("images/GargetImages/dizzy.png");
            break;
    }
}

void Gadget::randomRowLoc(LinkGame *game, int colLoc)
{
    QVector<QPair<int,int>>spaceRemain;
    int start = 0, end = 0,totalSpace = 0;
    if(colLoc < game->boxCol && colLoc >= 0){
        spaceRemain.push_back(QPair<int,int>(0, game->passageHeight - gadgetHeight));
        totalSpace += (game->passageHeight - gadgetHeight);
        start = game->passageHeight;
        for(int row = 0;row < game->boxRow;row ++){
            if(!game->boxMap[row][colLoc]->boxState.boxRemoved){
                start = game->boxMap[row][colLoc]->LeftTopY + game->boxHeight;
                continue;
            } else {
                end = game->boxMap[row][colLoc]->LeftTopY + game->boxHeight;
                spaceRemain.push_back(QPair<int,int>(start, end - gadgetHeight));
                totalSpace += (end - start - gadgetHeight);
                start = end;
            }
        }
        spaceRemain.push_back(QPair<int,int>(start, 600 - gadgetHeight));
        totalSpace += (600 - gadgetHeight - start - gadgetHeight);
    } else {
        spaceRemain.push_back(QPair<int,int>(0, 600 - gadgetHeight));
        totalSpace = 600 - gadgetHeight;
    }
    this->leftTopY = QRandomGenerator::global()->bounded(1,totalSpace-1);
    for(auto i:spaceRemain){
        this->leftTopY -= (i.second-i.first);
        if(this->leftTopY <= 0){
            this->leftTopY = this->leftTopY + i.second;
            break;
        }
    }
}

void Gadget::drawGarget(QPainter &painter) {
    painter.drawPixmap(leftTopX, leftTopY, gadgetWidth, gadgetHeight, gadgetMap);
}

void Gadget::singleGameGadgetEffect(LinkGame *game, Player *player) {
    switch(gargetType){
        case 1:
            plus1sEffect(game);
            break;
        case 2:
            shuffleEffect(game);
            break;
        case 3:
            hintEffect(game);
            break;
        case 4:
            flashEffect(game,player);
            break;
    }
}

void Gadget::doubleGameGadgetEffect(LinkGame *game, Player *anotherPlayer) {
    switch(gargetType){
        case 1:
            plus1sEffect(game);
            break;
        case 2:
            shuffleEffect(game);
            break;
        case 3:
            hintEffect(game);
            break;
        case 5:
            freezeEffect(game,anotherPlayer);
            break;
        case 6:
            dizzyEffect(game,anotherPlayer);
            break;
    }
}

void Gadget::plus1sEffect(LinkGame *game) {
    game->remainTime += 30;
}

void Gadget::shuffleEffect(LinkGame *game) {
    game->shuffleMap();
}

void Gadget::hintEffect(LinkGame *game) {
    game->hintTime = 10;
    game->hintTimerOn = true;
}

void Gadget::flashEffect(LinkGame *game,Player *player) {
    game->flashTime = 5;
    game->flashTimerOn = true;
    return;
}

void Gadget::freezeEffect(LinkGame *game,Player *player) {
    player->freezeTime = 3;
    game->freezeTimerOn = true;
}

void Gadget::dizzyEffect(LinkGame *game,Player *player) {
    player->dizzyTime = 10;
    game->dizzyTimerOn = true;
}

