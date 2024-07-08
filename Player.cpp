//
// Created by overji on 2024/6/20.
//

#include "Player.h"
#include "LinkGame.h"
#include "SelectChecker.h"
#include <QPainter>
#include <iostream>
#include "SpecialAlgorithm.h"

Player::Player(const int &playerLeftTopXInput, const int &playerLeftTopYInput, const int &playerWidthInput,
               const int &playerHeightInput, const int &playerSpeedInput,const int& playerSkin)
{
    //初始化玩家的各项参数
    this->playerHeight = playerHeightInput;
    this->playerLeftTopX = playerLeftTopXInput;
    this->playerLeftTopY = playerLeftTopYInput;
    this->playerSpeed = playerSpeedInput;
    this->playerWidth = playerWidthInput;
    this->freezeTime = 0;
    this->dizzyTime = 0;
    this->score = 0;
    this->scoreString = "分数: 0";
    //设置玩家贴图
    if(playerSkin == 1){
        this->playerImage = QPixmap("images/PlayerImages/20f432ebdfe483eee238d36d99e42a13.png");
    } else {
        this->playerImage = QPixmap("images/PlayerImages/1b49f5bd6f8f4d03052778304564cc38.png");
    }
}

Player::~Player()
{
    //析构函数
    this->currentSelected.clear();
}

void Player::leftMove(LinkGame * game)
{
    //玩家向左移动
    if(playerLeftTopX >= 0)
    {
        playerLeftTopX -= playerSpeed;
    }
    //检测向左移动的终点是否可到达
    if(playerLeftTopY + playerHeight > game->passageHeight && playerLeftTopY  < 600 - game->passageHeight)
    {
        //开始检测碰撞
        int currentTopY = (playerLeftTopY - game->passageHeight) / game->boxHeight;
        int currentBottomY = (playerLeftTopY + playerHeight - game->passageHeight) / game->boxHeight;
        BoxOfGame * topBox = nullptr;
        int topIndex = 0;
        BoxOfGame * bottomBox = nullptr;
        int bottomIndex = 0;
        for(int i = 0;i < game->boxCol; i ++)
        {
            //找到最右边没有被删除的箱子
            if(currentTopY < game->boxRow && currentTopY >= 0 && !game->boxMap[currentTopY][i]->boxState.boxRemoved)
            {
                if(game->boxMap[currentTopY][i]->LeftTopX > playerLeftTopX + game->boxWidth){
                    break;
                }
                topBox = game->boxMap[currentTopY][i];
                topIndex = i;
            }
            if(currentBottomY < game->boxRow && currentBottomY >= 0 && !game->boxMap[currentBottomY][i]->boxState.boxRemoved)
            {
                if(game->boxMap[currentBottomY][i]->LeftTopX > playerLeftTopX + game->boxWidth){
                    break;
                }
                bottomBox = game->boxMap[currentBottomY][i];
                bottomIndex = i;
            }
        }
        //获取箱子的右边界
        int topBoxRight = (topBox == nullptr) ? -1000 : topBox->LeftTopX + game->boxWidth;
        int bottomBoxRight = (bottomBox == nullptr) ? -1000 : bottomBox->LeftTopX + game->boxWidth;
        if(playerLeftTopX <= topBoxRight && playerLeftTopX >= (topBoxRight - this->playerSpeed))
        {
            //左边遇到箱子，限制移动，并且选中箱子
            topBox->boxState.boxSelected = true;
            playerLeftTopX = topBoxRight + 1;
            currentSelected.push_back(QPair<int,int>(currentTopY,topIndex));
            SelectChecker::checkSelected(game,this);
            return;
        }
        if(playerLeftTopX <= bottomBoxRight && playerLeftTopX >= (bottomBoxRight - this->playerSpeed))
        {
            //左边遇到箱子，限制移动，并且选中箱子
            bottomBox->boxState.boxSelected = true;
            playerLeftTopX = bottomBoxRight + 1;
            currentSelected.push_back(QPair<int,int>(currentBottomY,bottomIndex));
            SelectChecker::checkSelected(game,this);
            return;
        }
    }
    game->removeText = "";
    //限制玩家移动范围不得超过左边界
    if(playerLeftTopX < 0)
    {
        playerLeftTopX = 0;
    }
}

void Player::rightMove(LinkGame * game)
{
    //玩家向右移动
    if(playerLeftTopX + playerWidth <= 800)
    {
        playerLeftTopX += playerSpeed;
    }
    //检测向右移动的终点是否可到达
    if(playerLeftTopY + playerHeight >= game->passageHeight &&
    playerLeftTopY <= 600 - game->passageHeight){
        int currentTopY = (playerLeftTopY - game->passageHeight) / game->boxHeight;
        int currentBottomY = (playerLeftTopY + playerHeight - game->passageHeight) / game->boxHeight;
        BoxOfGame * topBox = nullptr;
        int topIndex = 0;
        BoxOfGame * bottomBox = nullptr;
        int bottomIndex = 0;
        for(int i = game->boxCol - 1; i >= 0; i --){
            //找到最左边没有被删除的箱子
            if(currentTopY < game->boxRow && currentTopY >= 0 && !game->boxMap[currentTopY][i]->boxState.boxRemoved){
                if(game->boxMap[currentTopY][i]->LeftTopX + game->boxWidth < playerLeftTopX){
                    break;
                }
                topBox = game->boxMap[currentTopY][i];
                topIndex = i;
            }
            if(currentBottomY < game->boxRow && currentBottomY >= 0 && !game->boxMap[currentBottomY][i]->boxState.boxRemoved){
                if(game->boxMap[currentBottomY][i]->LeftTopX + game->boxWidth < playerLeftTopX){
                    break;
                }
                bottomBox = game->boxMap[currentBottomY][i];
                bottomIndex = i;
            }
        }
        int topBoxLeft = (topBox == nullptr)? 1000 : topBox->LeftTopX;
        int bottomBoxLeft = (bottomBox == nullptr)? 1000 : bottomBox->LeftTopX;
        if(playerLeftTopX + playerWidth > topBoxLeft && playerLeftTopX + playerWidth <= topBoxLeft + playerSpeed && topBox){
            //右边遇到箱子，限制移动，并且选中箱子
            topBox->boxState.boxSelected = true;
            playerLeftTopX = topBoxLeft - playerWidth - 1;
            currentSelected.push_back(QPair<int,int>(currentTopY,topIndex));
            SelectChecker::checkSelected(game,this);
            return;
        }
        if(playerLeftTopX + playerWidth > bottomBoxLeft && playerLeftTopX + playerWidth <= bottomBoxLeft + playerSpeed && bottomBox){
            //右边遇到箱子，限制移动，并且选中箱子
            bottomBox->boxState.boxSelected = true;
            playerLeftTopX = bottomBoxLeft - playerWidth - 1;
            currentSelected.push_back(QPair<int,int>(currentBottomY,bottomIndex));
            SelectChecker::checkSelected(game,this);
            return;
        }
    }
    game->removeText = "";
    //限制玩家移动范围不得超过右边界
    if(playerLeftTopX + playerWidth > 800)
    {
        playerLeftTopX = 800 - playerWidth;
    }
}

void Player::upMove(LinkGame * game)
{
    //玩家向上移动
    if(playerLeftTopY >= 0)
    {
        playerLeftTopY -= playerSpeed;
    }
    //检测向上移动的终点是否可到达
    if(playerLeftTopX + playerWidth > game->passageWidth && playerLeftTopX < 800 - game->passageWidth) {
        //开始检测碰撞
        int currentLeftCol = (playerLeftTopX - game->passageWidth) / game->boxWidth;
        int currentRightCol = (playerLeftTopX + playerWidth - game->passageWidth) / game->boxWidth;
        BoxOfGame *leftBox = nullptr;
        int leftIndex = 0;
        BoxOfGame *rightBox = nullptr;
        int rightIndex = 0;
        for (int i = 0; i < game->boxRow; i++) {
            //找到最下边没有被删除的箱子
            if (currentLeftCol < game->boxCol && currentLeftCol >= 0 && !game->boxMap[i][currentLeftCol]->boxState.boxRemoved) {
                if(game->boxMap[i][currentLeftCol]->LeftTopY > playerLeftTopY){
                    break;
                }
                leftBox = game->boxMap[i][currentLeftCol];
                leftIndex = i;
            }
            if (currentRightCol < game->boxCol && currentRightCol >= 0 && !game->boxMap[i][currentRightCol]->boxState.boxRemoved) {
                if(game->boxMap[i][currentRightCol]->LeftTopY > playerLeftTopY){
                    break;
                }
                rightBox = game->boxMap[i][currentRightCol];
                rightIndex = i;
            }
        }
        //获取箱子的下边界
        int leftBoxBottom = (leftBox == nullptr) ? -1000 : leftBox->LeftTopY + game->boxHeight;
        int rightBoxBottom = (rightBox == nullptr) ? -1000 : rightBox->LeftTopY + game->boxHeight;
        //检测是否碰到箱子，如果碰到箱子还需限制移动
        if (playerLeftTopY <= leftBoxBottom && playerLeftTopY >= leftBoxBottom - playerSpeed - 1) {
            leftBox->boxState.boxSelected = true;
            playerLeftTopY = leftBoxBottom + 1;
            currentSelected.push_back(QPair<int,int>(leftIndex, currentLeftCol));
            SelectChecker::checkSelected(game,this);
            return;
        }
        if (playerLeftTopY <= rightBoxBottom && playerLeftTopY >= leftBoxBottom - playerSpeed - 1) {
            rightBox->boxState.boxSelected = true;
            playerLeftTopY = rightBoxBottom + 1;
            currentSelected.push_back(QPair<int,int>(rightIndex, currentRightCol));
            SelectChecker::checkSelected(game,this);
            return;
        }
    }
    game->removeText = "";
    if(playerLeftTopY < 0)
    {
        //限制玩家移动范围不得超过上边界
        playerLeftTopY = 0;
    }
}

void Player::downMove(LinkGame * game)
{
    //玩家向下移动
    if((playerLeftTopY + playerHeight ) <= 600)
    {
        playerLeftTopY += playerSpeed;
    }
    //检测向下移动的终点是否可到达
    if(playerLeftTopX + playerWidth > game->passageWidth && playerLeftTopX < 800 - game->passageWidth)
    {
        //开始检测碰撞
        int currentLeftX = (playerLeftTopX - game->passageWidth) / game->boxWidth;
        int currentRightX = (playerLeftTopX + playerWidth - game->passageWidth) / game->boxWidth;
        BoxOfGame * leftBox = nullptr;
        int leftIndex = 0;
        BoxOfGame * rightBox = nullptr;
        int rightIndex = 0;
        for(int i = game->boxRow - 1; i >= 0; i --)
        {
            //找到最上边没有被删除的箱子
            if(currentLeftX < game->boxCol && currentLeftX >= 0 && !game->boxMap[i][currentLeftX]->boxState.boxRemoved)
            {
                if(game->boxMap[i][currentLeftX]->LeftTopY + game->boxHeight < playerLeftTopY)
                {
                    break;
                }
                leftBox = game->boxMap[i][currentLeftX];
                leftIndex = i;
            }
            if(currentRightX < game->boxCol && currentRightX >= 0 && !game->boxMap[i][currentRightX]->boxState.boxRemoved)
            {
                if(game->boxMap[i][currentRightX]->LeftTopY + game->boxHeight < playerLeftTopY)
                {
                    break;
                }
                rightBox = game->boxMap[i][currentRightX];
                rightIndex = i;
            }
        }
        //获取箱子的上边界
        int leftBoxTop = (leftBox == nullptr) ? 1000 : leftBox->LeftTopY;
        int rightBoxTop = (rightBox == nullptr) ? 1000 : rightBox->LeftTopY;
        //检测是否碰到箱子，如果碰到箱子还需限制移动
        if(playerLeftTopY + playerHeight > leftBoxTop && playerLeftTopY + playerHeight <= leftBoxTop + playerSpeed)
        {
            leftBox->boxState.boxSelected = true;
            playerLeftTopY = leftBoxTop - playerHeight - 1;
            currentSelected.push_back(QPair<int,int>(leftIndex,currentLeftX));
            SelectChecker::checkSelected(game,this);
            return;
        }
        if(playerLeftTopY + playerHeight > rightBoxTop && playerLeftTopY + playerHeight <= rightBoxTop + playerSpeed)
        {
            rightBox->boxState.boxSelected = true;
            playerLeftTopY = rightBoxTop - playerHeight - 1;
            currentSelected.push_back(QPair<int,int>(rightIndex,currentRightX));
            SelectChecker::checkSelected(game,this);
            return;
        }
    }
    game->removeText = "";
    if((playerLeftTopY + playerHeight ) > 600)
    {
        //限制玩家移动范围不得超过下边界
        playerLeftTopY = 600 - playerHeight;
    }
}

void Player::resizePlayer(double xScaleRatio, double yScaleRatio)
{
    //窗口大小变化时，调整玩家大小
    this->playerHeight = (1*playerWidth*xScaleRatio)/yScaleRatio;
}

void Player::drawPlayer(QPainter &painter)
{
    //绘制玩家
    painter.save();
    painter.setBrush(QColor(142, 68, 173));
    //绘制玩家的四个顶点
    const QPoint playerPoints[4] = {
            QPoint(playerLeftTopX,playerLeftTopY),
            QPoint(playerLeftTopX+playerWidth,playerLeftTopY),
            QPoint(playerLeftTopX+playerWidth,playerLeftTopY+playerHeight),
            QPoint(playerLeftTopX,playerLeftTopY+playerHeight)
    };
    painter.drawConvexPolygon(playerPoints,4);
    painter.drawPixmap(playerLeftTopX,playerLeftTopY,playerWidth,playerHeight,this->playerImage);
    painter.restore();
}

void Player::playerMove(const int &direction,LinkGame * game,const int &xLoc,const int &yLoc)
{
    //玩家移动
    if(game->gamePause){
        //游戏暂停时，玩家无法移动
        return;
    }
    if(game->gameEnd){
        //游戏结束时，玩家无法移动
        return;
    }
    if(this->freezeTime != 0){
        //玩家被冻结时，无法移动
        return;
    }
    bool startDizzy = false;
    if(this->dizzyTime != 0){
        //玩家被眩晕时，方向相反
        startDizzy = true;
    }
    if(!SelectChecker::solutionExist(game,this)){
        //无解时，游戏结束
        game->removeText = "无解!";
        game->gameEnd = true;
        game->gamePause = false;
        game->summaryText = "游戏结束!";
        return;
    }
    if(!startDizzy){
        //正常移动
        switch (direction) {
            case 0:
                upMove(game);
                break;
            case 1:
                downMove(game);
                break;
            case 2:
                leftMove(game);
                break;
            case 3:
                rightMove(game);
                break;
            default:
                break;
        }
    } else {
        //眩晕时，方向相反
        switch (direction) {
            case 0:
                downMove(game);
                break;
            case 1:
                upMove(game);
                break;
            case 2:
                rightMove(game);
                break;
            case 3:
                leftMove(game);
                break;
            default:
                break;
        }
    }

    if(direction == -5){
        //flash移动
        flashMove(game,xLoc,yLoc);
    }
    //检查是否有相邻的两个方块可以消除
    this->scoreString = "分数: " + QString::number(score);
    checkGadgetHit(game);
}

void Player::checkClose(LinkGame * game)
{
    //检查是否有相邻的方块
    int currentLeftX = specialDiv((playerLeftTopX - playerSpeed - game->passageWidth),game->boxWidth);
    int currentTopY = specialDiv((playerLeftTopY - playerSpeed - game->passageHeight),game->boxHeight);
    int currentRightX = specialDiv((playerLeftTopX + playerWidth - game->passageWidth + playerSpeed),game->boxWidth);
    int currentBottomY = specialDiv((playerLeftTopY + playerHeight - game->passageHeight + playerSpeed),game->boxHeight);
    if(checkColIndexValid(game, currentLeftX) && checkRowIndexValid(game, currentTopY))
    {
        //左上方块
        game->boxMap[currentTopY][currentLeftX]->boxState.closeBox = true;
    }
    if(checkColIndexValid(game, currentLeftX) && checkRowIndexValid(game, currentBottomY))
    {
        //左下方块
        game->boxMap[currentBottomY][currentLeftX]->boxState.closeBox = true;
    }
    if(checkColIndexValid(game, currentRightX) && checkRowIndexValid(game, currentTopY))
    {
        //右上方块
        game->boxMap[currentTopY][currentRightX]->boxState.closeBox = true;
    }
    if(checkColIndexValid(game, currentRightX) && checkRowIndexValid(game, currentBottomY))
    {
        //右下方块
        game->boxMap[currentBottomY][currentRightX]->boxState.closeBox = true;
    }
}

void Player::clearClose(LinkGame *game)
{
    //清除玩家原先临近方块的临近情况
    for(int i = 0;i < game->boxRow; i ++)
    {
        for(int j = 0;j < game->boxCol; j ++)
        {
            game->boxMap[i][j]->boxState.closeBox = false;
        }
    }
}

void Player::clearSelected(LinkGame *game)
{
    //清除玩家选中的方块
    for(int i = 0;i < game->boxRow; i ++)
    {
        for(int j = 0;j < game->boxCol; j ++)
        {
            game->boxMap[i][j]->boxState.boxSelected = false;
        }
    }
}

bool Player::checkColIndexValid(const LinkGame *game, const int &xIndex)
{
    //检查列索引是否合法
    return xIndex >= 0 && xIndex < game->boxCol;
}

bool Player::checkRowIndexValid(const LinkGame *game, const int &yIndex)
{
    //检查行索引是否合法
    return yIndex >= 0 && yIndex < game->boxRow;
}

void Player::checkGadgetHit(LinkGame *game)
{
    //检查玩家是否碰到道具
    for(auto gadget:game->gadgets){
        if(gadget->leftTopX <= playerLeftTopX + playerWidth && gadget->leftTopX + gadget->gadgetWidth >= playerLeftTopX &&
           gadget->leftTopY <= playerLeftTopY + playerHeight && gadget->leftTopY + gadget->gadgetHeight >= playerLeftTopY){
            if(game->gameType == 0){
                gadget->singleGameGadgetEffect(game, this);
            } else {
                //双人游戏时，判断玩家1和玩家2，并且给对方加上效果
                if(this == game->player1){
                    gadget->doubleGameGadgetEffect(game, game->player2);
                } else {
                    gadget->doubleGameGadgetEffect(game, game->player1);
                }
            }
            game->gadgets.removeOne(gadget);
            delete gadget;
        }
    }
}

void Player::flashMove(LinkGame *game, int xLoc, int yLoc)
{
    //判断是否点击到了方块上
    int topRowLoc = specialDiv(yLoc - game->passageHeight - playerHeight / 2 ,game->boxWidth);
    int bottomRowLoc = specialDiv(yLoc - game->passageHeight + playerHeight / 2,game->boxWidth);
    int leftColLoc = specialDiv(xLoc - game->passageWidth - playerWidth / 2,game->boxWidth);
    int rightColLoc = specialDiv(xLoc - game->passageWidth + playerWidth / 2,game->boxWidth);
    if(checkColIndexValid(game,leftColLoc) && checkRowIndexValid(game,topRowLoc) && ! game->boxMap[topRowLoc][leftColLoc]->boxState.boxRemoved){
        //左上角有箱子
        xLoc += playerWidth / 2;
        yLoc += playerWidth / 2;
    }
    if(checkColIndexValid(game,rightColLoc) && checkRowIndexValid(game,topRowLoc) && ! game->boxMap[topRowLoc][rightColLoc]->boxState.boxRemoved){
        //右上角有箱子
        xLoc -= playerWidth / 2;
        yLoc += playerWidth / 2;
    }
    if(checkColIndexValid(game,leftColLoc) && checkRowIndexValid(game,bottomRowLoc) && ! game->boxMap[bottomRowLoc][leftColLoc]->boxState.boxRemoved){
        //左下角有箱子
        xLoc += playerWidth / 2;
        yLoc -= playerWidth / 2;
    }
    if(checkColIndexValid(game,rightColLoc) && checkRowIndexValid(game,bottomRowLoc) && ! game->boxMap[bottomRowLoc][rightColLoc]->boxState.boxRemoved){
        //右下角有箱子
        xLoc -= playerWidth / 2;
        yLoc -= playerWidth / 2;
    }
    int rowLoc = specialDiv(yLoc - game->passageHeight,game->boxHeight);
    int colLoc = specialDiv(xLoc - game->passageWidth,game->boxWidth);
    //判断是否点击到了箱子上
    if(checkColIndexValid(game,colLoc) && checkRowIndexValid(game,rowLoc)){
        if(game->boxMap[rowLoc][colLoc]->boxState.boxRemoved){
            //箱子已被清除
            this->playerLeftTopY = yLoc - (this->playerWidth / 2);
            this->playerLeftTopX = xLoc - (this->playerHeight / 2);
        } else {
            //点击到了箱子上，检测是否在箱子中或箱子边缘
            if(flashMoveCheckEdge(game,rowLoc,colLoc)){
                return;
            }
            if(flashMoveCheckCenter(game,rowLoc,colLoc)){
                return;
            }
        }
    } else {
        //点击到了箱子之外
        this->playerLeftTopY = yLoc - (this->playerWidth / 2);
        this->playerLeftTopX = xLoc - (this->playerHeight / 2);
        return;
    }
}

bool  Player::flashMoveCheckEdge(LinkGame *game, int rowLoc, int colLoc)
{
    //检查flash移动是否处于箱子边缘处，如果是就把玩家放到箱子边缘，并且选中箱子
    if(rowLoc == 0){
        //放到上面
        this->playerLeftTopY = game->boxMap[rowLoc][colLoc]->LeftTopY  - this->playerHeight - 2;
        this ->playerLeftTopX = game->boxMap[rowLoc][colLoc]->LeftTopX + (game->boxWidth / 2) - (this->playerWidth / 2);
        game->boxMap[rowLoc][colLoc]->boxState.boxSelected = true;
        currentSelected.push_back(QPair<int,int>(rowLoc,colLoc));
        SelectChecker::checkSelected(game,this);
        return true;
    }
    if(rowLoc == (game->boxRow - 1)){
        //放到下面
        this->playerLeftTopY = game->boxMap[rowLoc][colLoc]->LeftTopY + game->boxHeight + 2;
        this->playerLeftTopX = game->boxMap[rowLoc][colLoc]->LeftTopX + (game->boxWidth / 2) - (this->playerWidth / 2);
        game->boxMap[rowLoc][colLoc]->boxState.boxSelected = true;
        currentSelected.push_back(QPair<int,int>(rowLoc,colLoc));
        SelectChecker::checkSelected(game,this);
        return true;
    }
    if(colLoc == 0){
        //放到左边
        this->playerLeftTopY = game->boxMap[rowLoc][colLoc]->LeftTopY + (game->boxHeight / 2) - (this->playerHeight / 2);
        this->playerLeftTopX = game->boxMap[rowLoc][colLoc]->LeftTopX - this->playerWidth - 2;
        game->boxMap[rowLoc][colLoc]->boxState.boxSelected = true;
        currentSelected.push_back(QPair<int,int>(rowLoc,colLoc));
        SelectChecker::checkSelected(game,this);
        return true;
    }
    if(colLoc == (game->boxCol - 1)){
        //放到右边
        this->playerLeftTopY = game->boxMap[rowLoc][colLoc]->LeftTopY + (game->boxHeight / 2) - (this->playerHeight / 2);
        this->playerLeftTopX = game->boxMap[rowLoc][colLoc]->LeftTopX + game->boxWidth + 2;
        game->boxMap[rowLoc][colLoc]->boxState.boxSelected = true;
        currentSelected.push_back(QPair<int,int>(rowLoc,colLoc));
        SelectChecker::checkSelected(game,this);
        return true;
    }
    return false;
}

bool Player::flashMoveCheckCenter(LinkGame *game, int rowLoc,int colLoc)
{
    //检查flash移动是否处于箱子中间，并且点击的箱子周边是否还有空位，如果是就把玩家放到箱子边缘，并且选中箱子
    if(checkColIndexValid(game,colLoc-1) && checkRowIndexValid(game,rowLoc)) {
        //左边有空位
        if(game->boxMap[rowLoc][colLoc - 1]->boxState.boxRemoved){
            this->playerLeftTopY = game->boxMap[rowLoc][colLoc - 1]->LeftTopY + (game->boxHeight / 2) - (this->playerWidth / 2);
            this->playerLeftTopX = game->boxMap[rowLoc][colLoc - 1]->LeftTopX + game->boxWidth - this->playerWidth - 2;
            game->boxMap[rowLoc][colLoc]->boxState.boxSelected = true;
            currentSelected.push_back(QPair<int,int>(rowLoc,colLoc));
            SelectChecker::checkSelected(game,this);
            return true;
        }
    }
    if(checkColIndexValid(game,colLoc+1) && checkRowIndexValid(game,rowLoc)) {
        //右边有空位
        if(game->boxMap[rowLoc][colLoc + 1]->boxState.boxRemoved){
            this->playerLeftTopY = game->boxMap[rowLoc][colLoc + 1]->LeftTopY + (game->boxHeight / 2) - (this->playerWidth / 2);
            this->playerLeftTopX = game->boxMap[rowLoc][colLoc + 1]->LeftTopX +  2;
            game->boxMap[rowLoc][colLoc]->boxState.boxSelected = true;
            currentSelected.push_back(QPair<int,int>(rowLoc,colLoc));
            SelectChecker::checkSelected(game,this);
            return true;
        }
    }
    if(checkColIndexValid(game,colLoc) && checkRowIndexValid(game,rowLoc - 1)) {
        //上面有空位
        if(game->boxMap[rowLoc - 1][colLoc]->boxState.boxRemoved){
            this->playerLeftTopY = game->boxMap[rowLoc - 1][colLoc]->LeftTopY + game->boxHeight - this->playerHeight - 2;
            this->playerLeftTopX = game->boxMap[rowLoc - 1][colLoc]->LeftTopX + (game->boxWidth / 2) - (this->playerWidth / 2);
            game->boxMap[rowLoc][colLoc]->boxState.boxSelected = true;
            currentSelected.push_back(QPair<int,int>(rowLoc,colLoc));
            SelectChecker::checkSelected(game,this);
            return true;
        }
    }
    if(checkColIndexValid(game,colLoc) && checkRowIndexValid(game,rowLoc + 1)) {
        //下面有空位
        if(game->boxMap[rowLoc + 1][colLoc]->boxState.boxRemoved){
            this->playerLeftTopY = game->boxMap[rowLoc + 1][colLoc]->LeftTopY + 2;
            this->playerLeftTopX = game->boxMap[rowLoc + 1][colLoc]->LeftTopX + (game->boxWidth / 2) - (this->playerWidth / 2);
            game->boxMap[rowLoc][colLoc]->boxState.boxSelected = true;
            currentSelected.push_back(QPair<int,int>(rowLoc,colLoc));
            SelectChecker::checkSelected(game,this);
            return true;
        }
    }
    return false;
}