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
    this->playerHeight = playerHeightInput;
    this->playerLeftTopX = playerLeftTopXInput;
    this->playerLeftTopY = playerLeftTopYInput;
    this->playerSpeed = playerSpeedInput;
    this->playerWidth = playerWidthInput;
    this->score = 0;
    this->scoreString = "分数: 0";
    if(playerSkin == 1){
        this->playerImage = QPixmap("images/PlayerImages/20f432ebdfe483eee238d36d99e42a13.png");
    }
}

void Player::leftMove(LinkGame * game)
{
    if(playerLeftTopX >= 0)
    {
        playerLeftTopX -= playerSpeed;
    }
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
        int topBoxRight = (topBox == nullptr) ? -1000 : topBox->LeftTopX + game->boxWidth;
        int bottomBoxRight = (bottomBox == nullptr) ? -1000 : bottomBox->LeftTopX + game->boxWidth;
        if(playerLeftTopX < topBoxRight && playerLeftTopX > (topBoxRight - this->playerSpeed))
        {
            //左边遇到箱子，限制移动，并且选中箱子
            topBox->boxState.boxSelected = true;
            playerLeftTopX = topBoxRight + 1;
            currentSelected.push_back(QPair<int,int>(currentTopY,topIndex));
            SelectChecker::checkSelected(game,this);
            return;
        }
        if(playerLeftTopX < bottomBoxRight && playerLeftTopX > (bottomBoxRight - this->playerSpeed))
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
    if(playerLeftTopX < 0)
    {
        playerLeftTopX = 0;
    }
}

void Player::rightMove(LinkGame * game)
{
    if(playerLeftTopX + playerWidth <= 800)
    {
        playerLeftTopX += playerSpeed;
    }
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
    if(playerLeftTopX + playerWidth > 800)
    {
        playerLeftTopX = 800 - playerWidth;
    }
}

void Player::upMove(LinkGame * game)
{
    if(playerLeftTopY >= 0)
    {
        playerLeftTopY -= playerSpeed;
    }
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
        int leftBoxBottom = (leftBox == nullptr) ? -1000 : leftBox->LeftTopY + game->boxHeight;
        int rightBoxBottom = (rightBox == nullptr) ? -1000 : rightBox->LeftTopY + game->boxHeight;
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
        playerLeftTopY = 0;
    }
}

void Player::downMove(LinkGame * game)
{
    if((playerLeftTopY + playerHeight ) <= 600)
    {
        playerLeftTopY += playerSpeed;
    }
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
        int leftBoxTop = (leftBox == nullptr) ? 1000 : leftBox->LeftTopY;
        int rightBoxTop = (rightBox == nullptr) ? 1000 : rightBox->LeftTopY;
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
        playerLeftTopY = 600 - playerHeight;
    }
}

void Player::resizePlayer(double xScaleRatio, double yScaleRatio)
{
    this->playerHeight = (1*playerWidth*xScaleRatio)/yScaleRatio;
}

void Player::drawPlayer(QPainter &painter)
{
    painter.save();
    painter.setBrush(QColor(142, 68, 173));
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
    if(game->gamePause){
        return;
    }
    if(game->gameEnd){
        return;
    }
    if(!SelectChecker::solutionExist(game,this)){
        game->removeText = "无解!";
        game->gameEnd = true;
        game->gamePause = false;
        game->summaryText = "游戏结束!";
        return;
    }
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
    if(direction == -5){
        flashMove(game,xLoc,yLoc);
    }
    this->scoreString = "分数: " + QString::number(score);
    checkGadgetHit(game);
}

void Player::checkClose(LinkGame * game)
{
    int currentLeftX = specialDiv((playerLeftTopX - game->passageWidth),game->boxWidth);
    int currentTopY = specialDiv((playerLeftTopY - game->passageHeight),game->boxHeight);
    if(checkColIndexValid(game, currentLeftX) && checkRowIndexValid(game, currentTopY))
    {
        game->boxMap[currentTopY][currentLeftX]->boxState.closeBox = true;
    }
    if(checkColIndexValid(game, currentLeftX) && checkRowIndexValid(game, currentTopY - 1))
    {
        game->boxMap[currentTopY-1][currentLeftX]->boxState.closeBox = true;
    }
    if(checkColIndexValid(game, currentLeftX - 1) && checkRowIndexValid(game, currentTopY))
    {
        game->boxMap[currentTopY][currentLeftX-1]->boxState.closeBox = true;
    }
    if(checkColIndexValid(game, currentLeftX + 1) && checkRowIndexValid(game, currentTopY))
    {
        game->boxMap[currentTopY][currentLeftX+1]->boxState.closeBox = true;
    }
    if(checkColIndexValid(game, currentLeftX) && checkRowIndexValid(game, currentTopY + 1))
    {
        game->boxMap[currentTopY+1][currentLeftX]->boxState.closeBox = true;
    }
}

void Player::clearClose(LinkGame *game)
{
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
    return xIndex >= 0 && xIndex < game->boxCol;
    return xIndex >= 0 && xIndex < game->boxCol;
}

bool Player::checkRowIndexValid(const LinkGame *game, const int &yIndex)
{
    return yIndex >= 0 && yIndex < game->boxRow;
}

void Player::checkGadgetHit(LinkGame *game)
{
    for(auto gadget:game->gadgets){
        if(gadget->leftTopX <= playerLeftTopX + playerWidth && gadget->leftTopX + gadget->gadgetWidth >= playerLeftTopX &&
           gadget->leftTopY <= playerLeftTopY + playerHeight && gadget->leftTopY + gadget->gadgetHeight >= playerLeftTopY){
            gadget->singleGameGadgetEffect(game, this);
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
    if(checkColIndexValid(game,colLoc) && checkRowIndexValid(game,rowLoc)){
        if(game->boxMap[rowLoc][colLoc]->boxState.boxRemoved){
            this->playerLeftTopY = yLoc - (this->playerWidth / 2);
            this->playerLeftTopX = xLoc - (this->playerHeight / 2);
        } else {
            if(flashMoveCheckEdge(game,rowLoc,colLoc)){
                return;
            }
            if(flashMoveCheckCenter(game,rowLoc,colLoc)){
                return;
            }
        }
    } else {
        this->playerLeftTopY = yLoc - (this->playerWidth / 2);
        this->playerLeftTopX = xLoc - (this->playerHeight / 2);
        return;
    }
}

bool  Player::flashMoveCheckEdge(LinkGame *game, int rowLoc, int colLoc)
{
    if(rowLoc == 0){
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