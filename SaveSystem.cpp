//
// Created by overji on 2024/7/3.
//

#include <QDir>
#include <iostream>
#include "SaveSystem.h"

class LinkGame;
class Player;
class Gadget;
class BoxOfGame;

void SaveSystem::saveGame(LinkGame * game)
{
    // 如果save文件夹不存在，创建save文件夹
    QDir dir;
    if(!dir.exists("save")){
        dir.mkdir("save");
    }
    // 保存游戏数据，如果gameData.sav文件不存在，创建gameData.sav文件
    QFile saveFile("save/gameData.sav");
    saveFile.open(QIODevice::WriteOnly);
    QDataStream out(&saveFile);
    out << game->boxRow << game->boxCol;
    out << game->gameFps << game->remainBoxNumber << game->boxWidth << game->boxHeight <<  game->passageWidth <<  game->passageHeight;
    out << game->xScaleRatio << game->yScaleRatio;
    out << game->gameEnd << game->gamePause << game->noSolution;

    out << game->remainTime << game->gameType;
    savePlayerData(out,game->player1);
    if(game->gameType != 0){
        std::cout << "ok!" << std::endl;
        savePlayerData(out,game->player2);
    }
    out << game->removeText << game->leftTimeText << game->summaryText;
    out << game->linePath.size();
    for(auto i :game->linePath){
        out << i.x() <<i.y();
    }

    out << game->toBeRemovedBox.size();
    for(auto i :game->toBeRemovedBox){
        out << i.first << i.second;
    }
    out << game->removeTimerOn;

    out << game->gadgets.size();
    for(auto i : game->gadgets){
        out << i->gadgetWidth <<  i->gadgetHeight << i->leftTopX << i->leftTopY << i->gargetType;
    }
    out << game->maxGadgetNumber << game->gadgetSummonPossibility;
    out << game->hintTime << game->hintTimerOn;
    out << game->hintedBoxes.size();
    for(auto i : game->hintedBoxes){
        out << i.first << i.second;
    }
    out << game->flashTime << game->flashTimerOn;
    out << game->dizzyTimerOn << game->freezeTimerOn;

    for(int row = 0;row < game->boxRow;row ++){
        for(int col = 0;col < game->boxCol;col ++){
            out << game->boxMap[row][col]->boxState.boxRemoved << game->boxMap[row][col]->boxState.boxSelected << game->boxMap[row][col]->boxState.closeBox  <<
            game->boxMap[row][col]->boxState.boxToBeRemoved << game->boxMap[row][col]->boxState.boxHinted;
            out << game->boxMap[row][col]->LeftTopX  << game->boxMap[row][col]->LeftTopY << game->boxMap[row][col]->typeOfBox
                << game->boxMap[row][col]->boxColor << game->boxMap[row][col]->boarderColor;
        }
    }

    std::cout << "Save Game Success" << std::endl;

}

void SaveSystem::savePlayerData(QDataStream &out, Player *player)
{
    out << player->playerLeftTopX << player->playerLeftTopY << player->playerWidth << player->playerHeight
        << player->playerSpeed << player->score << player->scoreString;
    out << player->freezeTime << player->dizzyTime;
    out << player->currentSelected.size();
    for(auto i : player->currentSelected){
        out << i.first << i.second;
    }
}

LinkGame * SaveSystem::loadGame()
{
    LinkGame * game = new LinkGame();
    QFile saveFile("save/gameData.sav");
    saveFile.open(QIODevice::ReadOnly);
    QDataStream in(&saveFile);
    in >> game->boxRow >> game->boxCol;
    in >> game->gameFps >> game->remainBoxNumber >> game->boxWidth >> game->boxHeight >>  game->passageWidth >>  game->passageHeight;
    in >> game->xScaleRatio >> game->yScaleRatio;
    in >> game->gameEnd >> game->gamePause >> game->noSolution;

    in >> game->remainTime >> game->gameType;
    loadPlayerData(in,game->player1);
    if(game->gameType != 0){
        game->player2 = new Player(0,0,0,0,0,0);
        loadPlayerData(in,game->player2);
    }
    in >> game->removeText >> game->leftTimeText >> game->summaryText;
    qsizetype linePathSize;
    in >> linePathSize;
    for(int i = 0;i < linePathSize;i ++){
        int x,y;
        in >> x >> y;
        game->linePath.push_back(QPoint(x,y));
    }

    qsizetype toBeRemovedBoxSize;
    in >> toBeRemovedBoxSize;
    for(int i = 0;i < toBeRemovedBoxSize;i ++){
        int x,y;
        in >> x >> y;
        game->toBeRemovedBox.push_back(QPair<int,int>(x,y));
    }


    in >> game->removeTimerOn;
    qsizetype gadgetSize;
    in >> gadgetSize;
    for(int i = 0;i < gadgetSize;i ++){
        int gadgetWidth,gadgetHeight,leftTopX,leftTopY,gargetType;
        in >> gadgetWidth >> gadgetHeight >> leftTopX >> leftTopY >> gargetType;
        Gadget * gadget = new Gadget(game);
        gadget->leftTopX = leftTopX;
        gadget->leftTopY = leftTopY;
        gadget->gadgetWidth = gadgetWidth;
        gadget->gadgetHeight = gadgetHeight;
        gadget->gargetType = gargetType;
        game->gadgets.push_back(gadget);
    }
    in >> game->maxGadgetNumber >> game->gadgetSummonPossibility;
    in >> game->hintTime >> game->hintTimerOn;
    qsizetype hintedBoxesSize;
    in >> hintedBoxesSize;
    for(int i = 0;i < hintedBoxesSize;i ++){
        int x,y;
        in >> x >> y;
        game->hintedBoxes.push_back(QPair<int,int>(x,y));
    }
    in >> game->flashTime >> game->flashTimerOn;
    in >> game->dizzyTimerOn >> game->freezeTimerOn;
    //所有箱子初始化
    game->initGlobalBox(game->boxRow,game->boxCol);
    for(int row = 0;row < game->boxRow;row ++){
        for(int col = 0;col < game->boxCol;col ++){
            in >> game->boxMap[row][col]->boxState.boxRemoved >> game->boxMap[row][col]->boxState.boxSelected >> game->boxMap[row][col]->boxState.closeBox  >>
            game->boxMap[row][col]->boxState.boxToBeRemoved >> game->boxMap[row][col]->boxState.boxHinted;
            in >> game->boxMap[row][col]->LeftTopX  >> game->boxMap[row][col]->LeftTopY >> game->boxMap[row][col]->typeOfBox
                >> game->boxMap[row][col]->boxColor >> game->boxMap[row][col]->boarderColor;
        }
    }
    if(game->gamePause){
        game->setGamePause();
    }
    return game;
}

void SaveSystem::loadPlayerData(QDataStream &in, Player *player) {
    in >> player->playerLeftTopX
    >> player->playerLeftTopY
    >> player->playerWidth
    >> player->playerHeight
       >> player->playerSpeed
       >> player->score >> player->scoreString;
    in >> player->freezeTime >> player->dizzyTime;
    qsizetype currentSelectedSize;
    in >> currentSelectedSize;
    for (int i = 0; i < currentSelectedSize; i++) {
        int x, y;
        in >> x >> y;
        player->currentSelected.push_back(QPair<int, int>(x, y));
    }
}