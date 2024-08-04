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
    // 保存游戏数据
    out << game->boxRow << game->boxCol;
    out << game->gameFps << game->remainBoxNumber << game->boxWidth << game->boxHeight <<  game->passageWidth <<  game->passageHeight;
    out << game->xScaleRatio << game->yScaleRatio;
    out << game->gameEnd << game->gamePause << game->noSolution;

    out << game->remainTime << game->gameType;
    //保存玩家数据
    savePlayerData(out,game->player1);
    if(game->gameType != 0){
        savePlayerData(out,game->player2);
    }
    //保存游戏文本
    out << game->removeText << game->leftTimeText << game->summaryText;
    //保存道具
    out << game->gadgets.size();
    for(auto i : game->gadgets){
        out << i->gadgetWidth <<  i->gadgetHeight << i->leftTopX << i->leftTopY << i->gargetType << i->gadgetMap;
    }
    out << game->maxGadgetNumber << game->gadgetSummonPossibility;
    //保存道具相关计时情况
    out << game->hintTime << game->hintTimerOn;
    out << game->hintedBoxes.size();
    for(auto i : game->hintedBoxes){
        out << i.first << i.second;
    }
    out << game->flashTime << game->flashTimerOn;
    out << game->dizzyTimerOn << game->freezeTimerOn;

    //保存所有箱子
    for(int row = 0;row < game->boxRow;row ++){
        for(int col = 0;col < game->boxCol;col ++){
            out << game->boxMap[row][col]->getBoxState().boxRemoved
                << game->boxMap[row][col]->getBoxState().boxSelected
                << game->boxMap[row][col]->getBoxState().closeBox
                << game->boxMap[row][col]->getBoxState().boxToBeRemoved
                << game->boxMap[row][col]->getBoxState().boxHinted;
            out << game->boxMap[row][col]->getLeftTopX()
                << game->boxMap[row][col]->getLeftTopY()
                << game->boxMap[row][col]->getTypeOfBox()
                << game->boxMap[row][col]->getBoxColor()
                << game->boxMap[row][col]->getBoarderColor();
            out << game->boxMap[row][col]->getBoxPixmap();
        }
    }
    std::cout << "Save Game Success" << std::endl;
}

void SaveSystem::savePlayerData(QDataStream &out, Player *player)
{
    //保存玩家数据
    out << player->playerLeftTopX << player->playerLeftTopY << player->playerWidth << player->playerHeight
        << player->playerSpeed << player->score << player->scoreString;
    out << player->freezeTime << player->dizzyTime;
    out << player->playerImage;
    out << player->currentSelected.size();
    for(auto i : player->currentSelected){
        out << i.first << i.second;
    }

    out << player->toBeRemovedBox.size();
    for(auto i :player->toBeRemovedBox){
        out << i.first << i.second;
    }
    out << player->linePath.size();
    for(auto i :player->linePath){
        out << i.x() <<i.y();
    }
    out << player->removeTimerOn;
}

LinkGame * SaveSystem::loadGame()
{
    //加载游戏数据
    LinkGame * game = new LinkGame();
    QFile saveFile("save/gameData.sav");
    saveFile.open(QIODevice::ReadOnly);
    QDataStream in(&saveFile);
    //加载游戏数据
    in >> game->boxRow >> game->boxCol;
    in >> game->gameFps >> game->remainBoxNumber >> game->boxWidth >> game->boxHeight >>  game->passageWidth >>  game->passageHeight;
    in >> game->xScaleRatio >> game->yScaleRatio;
    in >> game->gameEnd >> game->gamePause >> game->noSolution;

    in >> game->remainTime >> game->gameType;
    //加载玩家数据
    loadPlayerData(in,game->player1,game);
    if(game->gameType != 0){
        game->player2 = new Player(0,0,0,0,0,0);
        loadPlayerData(in,game->player2,game);
    }
    //加载游戏文本
    in >> game->removeText >> game->leftTimeText >> game->summaryText;

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
        in >> gadget->gadgetMap;
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
            BoxState tempBoxState;
            in >> tempBoxState.boxRemoved >> tempBoxState.boxSelected >> tempBoxState.closeBox
               >> tempBoxState.boxToBeRemoved >> tempBoxState.boxHinted;
            game->boxMap[row][col]->setBoxRemoved(tempBoxState.boxRemoved);
            game->boxMap[row][col]->setBoxSelected(tempBoxState.boxSelected);
            game->boxMap[row][col]->setBoxClose(tempBoxState.closeBox);
            game->boxMap[row][col]->setBoxToBeRemoved(tempBoxState.boxToBeRemoved);
            game->boxMap[row][col]->setBoxHinted(tempBoxState.boxHinted);

            int tempLeftTopX, tempLeftTopY, tempTypeOfBox;
            QColor tempBoxColor, tempBoarderColor;
            in >> tempLeftTopX >> tempLeftTopY >> tempTypeOfBox >> tempBoxColor >> tempBoarderColor;
            game->boxMap[row][col]->setLeftTopX(tempLeftTopX);
            game->boxMap[row][col]->setLeftTopY(tempLeftTopY);
            game->boxMap[row][col]->setTypeOfBox(tempTypeOfBox);
            game->boxMap[row][col]->setBoxColor(tempBoxColor);
            game->boxMap[row][col]->setBoarderColor(tempBoarderColor);

            QPixmap tempBoxPixmap;
            in >> tempBoxPixmap;
            game->boxMap[row][col]->setBoxPixmap(tempBoxPixmap);
        }
    }
    if(game->gamePause){
        game->setGamePause();
    }
    return game;
}

void SaveSystem::loadPlayerData(QDataStream &in, Player *player,LinkGame * game) {
    //加载玩家数据
    in >> player->playerLeftTopX
    >> player->playerLeftTopY
    >> player->playerWidth
    >> player->playerHeight
       >> player->playerSpeed
       >> player->score >> player->scoreString;
    in >> player->freezeTime >> player->dizzyTime;
    in >> player->playerImage;
    qsizetype currentSelectedSize;
    in >> currentSelectedSize;
    for (int i = 0; i < currentSelectedSize; i++) {
        int x, y;
        in >> x >> y;
        player->currentSelected.push_back(QPair<int, int>(x, y));
    }
    //加载待消除箱子
    qsizetype toBeRemovedBoxSize;
    in >> toBeRemovedBoxSize;
    for(int i = 0;i < toBeRemovedBoxSize;i ++){
        int x,y;
        in >> x >> y;
        player->toBeRemovedBox.push_back(QPair<int,int>(x,y));
    }

    qsizetype linePathSize;
    in >> linePathSize;
    for(int i = 0;i < linePathSize;i ++){
        int x,y;
        in >> x >> y;
        player->linePath.push_back(QPoint(x,y));
    }
    //加载道具
    in >> player->removeTimerOn;
    player->game = game;
}