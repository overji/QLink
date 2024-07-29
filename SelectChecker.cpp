//
// Created by overji on 2024/6/24.
//

#include "SelectChecker.h"
#include <iostream>
#include <QWidget>
#include "SpecialAlgorithm.h"


void SelectChecker::checkSelected(LinkGame *game, Player *player)
{
    checkNumber(game,player);
}

void SelectChecker::checkNumber(LinkGame *game, Player *player)
{
    QVector<QPair<int,int>>path;
    //如果选中的箱子数为2，那么我们就可以进行消除操作了
    if(player->currentSelected.size() == 2) {
        if (player->currentSelected[0] == player->currentSelected[1]) {
            player->currentSelected.pop_back();
            return;
        }
        //清除箱子的被选择情况
        for (auto i: player->currentSelected) {
            game->boxMap[i.first][i.second]->boxState.boxSelected = false;
        }
        //检查是否可以消除，返回的是一个路径的vector，可以消除的话直接给玩家加分，过0.2s后再让箱子和路径消失不见
        path = checkValid(game, player);
        if (path.size() == 1) {
            if (path[0].first == -1 && path[0].second == -1) {
                //这里输出"错误！"
                game->removeText = "错误!";
            } else if (path[0].first == -2 && path[0].second == -2) {
                //零折线情况
                for (auto i: player->currentSelected) {
                    player->toBeRemovedBox.push_back(i);
                    player->score += game->boxMap[i.first][i.second]->addBoxScore();
                }
                game->removeText = "消除成功!";
            } else {
                //一折线情况
                for (auto i: player->currentSelected) {
                    player->toBeRemovedBox.push_back(i);
                    player->score += game->boxMap[i.first][i.second]->addBoxScore();
                }
                game->removeText = "消除成功!";
            }
        } else if (path.size() == 2){
            //二折线情况
            for (auto i: player->currentSelected) {
                player->toBeRemovedBox.push_back(i);
                player->score += game->boxMap[i.first][i.second]->addBoxScore();
            }
            game->removeText = "消除成功!";
        } else {
            game->removeText = "错误!";
        }
        player->currentSelected.clear();
    }
}

QVector<QPair<int,int>> SelectChecker::checkValid(LinkGame *game, Player *player)
{
    //寻找可行的路径
    QVector<QPair<int,int>>path;
    BoxOfGame* box1 = game->boxMap[player->currentSelected[0].first][player->currentSelected[0].second];
    BoxOfGame* box2 = game->boxMap[player->currentSelected[1].first][player->currentSelected[1].second];
    if(box1->typeOfBox != box2->typeOfBox)return path;
    int x1 = box1->LeftTopX + game->boxWidth/2;
    int y1 = box1->LeftTopY + game->boxHeight/2;
    int x2 = box2->LeftTopX + game->boxWidth/2;
    int y2 = box2->LeftTopY + game->boxHeight/2;

    //零折线
    if(x1 == x2){
        if(checkLine(game,player,2,y1,y2,x1)){
            path.push_back(QPair<int,int>(-2,-2));
            player->linePath.push_back(QPoint(x1,y1));
            player->linePath.push_back(QPoint(x2,y2));
            return path;
        }
    } else if(y1 == y2){
        if(checkLine(game,player,1,x1,x2,y1)){
            path.push_back(QPair<int,int>(-2,-2));
            player->linePath.push_back(QPoint(x1,y1));
            player->linePath.push_back(QPoint(x2,y2));
            return path;
        }
    }
    //一折线
    /*一折线，交点两个，一个是(x1,y2)，一个是(x2,y1)，那么判断这两段线中间有无障碍，就可以
    判断是否为一折线的情况了，并且还需要判断一下这两个点处是不是有方块*/
    if(checkLine(game,player,2,y1,y2,x1) &&
    checkLine(game,player,1,x1,x2,y2) &&
            !checkExist(game,player,x1,y2)){
        //(x1,y1)->(x1,y2)->(x2,y2)
        player->linePath.push_back(QPoint(x1,y1));
        player->linePath.push_back(QPoint(x1,y2));
        player->linePath.push_back(QPoint(x2,y2));
        path.push_back(QPair<int,int>(x1,y2));
        return path;
    }
    if(checkLine(game,player,1,x1,x2,y1) &&
    checkLine(game,player,2,y1,y2,x2) &&
            !checkExist(game,player,x2,y1)){
        //(x1,y1)->(x2,y1)->(x2,y2)
        path.push_back(QPair<int,int>(x2,y1));
        player->linePath.push_back(QPoint(x1,y1));
        player->linePath.push_back(QPoint(x2,y1));
        player->linePath.push_back(QPoint(x2,y2));
        return path;
    }
    //二折线，假定两个箱子各自发出的平行线都沿着x方向，那么遍历y，这里的y确定为箱子坐标，看看是否有一条线可以连接
    path = twoTwistCheck(game,player,x1,y1,x2,y2);
    //所有可能情况都被遍历到了，但是没有找到合适的路径，返回(-1,-1)
    if(path.empty()){
        path.push_back(QPair<int,int>(-1,-1));
    }
    return path;
}

bool SelectChecker::checkLine(LinkGame *game, Player *player, int direction, int start, int end,int anotherLocation)
{
    //检查路径是否可行，没有障碍存在
    //应当注意start和end传入的是画面的坐标，相对应与800,600
    //dirction为1时是横线，y大小不变
    //dirction为2时是竖线，x大小不变
    if(start > end){
        int temp = start;
        start = end;
        end = temp;
    }
    if(direction == 1){
        int p = start + game->boxWidth;
        for(;p < end;p += game->boxWidth){
            //遍历这一条线上所有的箱子，看看是否有未被消除的箱子在这条线上
            int colLoc = specialDiv(p - game->passageWidth, game->boxWidth);
            int rowLoc = specialDiv(anotherLocation - game->passageHeight, game->boxHeight);
            if(player->checkColIndexValid(colLoc) && player->checkRowIndexValid( rowLoc)){
                if(!game->boxMap[rowLoc][colLoc]->boxState.boxRemoved && abs(p - end) >= (game->boxWidth / 2)){
                    return false;
                }
            }
        }
    } else {
        int p = start + game->boxHeight;
        for (; p < end; p += game->boxHeight) {
            //遍历这一条线上所有的箱子，看看是否有未被消除的箱子在这条线上
            int colLoc = specialDiv(anotherLocation - game->passageWidth, game->boxWidth);
            int rowLoc = specialDiv(p - game->passageHeight, game->boxHeight);
            if (player->checkColIndexValid( colLoc) && player->checkRowIndexValid( rowLoc)) {
                if (!game->boxMap[rowLoc][colLoc]->boxState.boxRemoved && abs(p - end) >= (game->boxHeight / 2)){
                    return false;
                }
            }
        }
    }
    return true;
}

bool SelectChecker::checkExist(LinkGame *game, Player *player, int x, int y)
{
    //检查(x,y)处是否有箱子
    int colLoc = specialDiv(x - game->passageWidth, game->boxWidth);
    int rowLoc = specialDiv(y - game->passageHeight, game->boxHeight);
    if (player->checkColIndexValid( colLoc) && player->checkRowIndexValid( rowLoc)) {
        if (!game->boxMap[rowLoc][colLoc]->boxState.boxRemoved) {
            return true;
        }
    }
    return false;
}

QVector<QPair<int,int>> SelectChecker::twoTwistCheck(LinkGame * game,Player * player,int x1,int y1,int x2,int y2)
{
    //寻找二折线的路径
    QVector<QPair<int,int>>path;
    //首先看x方向的情况
    //第一步我们确定x与y开始的地方
    int yStart = 5;
    int xStart = 5;
    //然后我们从xStart开始，一直到800，看是否有一条线可以连接y1和y2之间的部分而不经过任何一个箱子
    //需要判断一下交点有无方块
    for(int lineX = xStart; lineX < 800;){
        if(checkLine(game, player, 1, x1, lineX, y1) &&
           checkLine(game, player, 1, x2, lineX, y2)){
            if(checkLine(game, player, 2, y1, y2, lineX)
            && !checkExist(game, player, lineX, y1) && !checkExist(game, player, lineX, y2)){
                path.push_back(QPair<int,int>(lineX, y1));
                path.push_back(QPair<int,int>(lineX, y2));
                player->linePath.push_back(QPoint(x1,y1));
                player->linePath.push_back(QPoint(lineX,y1));
                player->linePath.push_back(QPoint(lineX,y2));
                player->linePath.push_back(QPoint(x2,y2));
                return path;
            }
        }
        if(lineX <= 750){
            lineX += game->boxWidth;
        } else {
            lineX += 2;
        }
    }
    //同样，我们从yStart开始，一直到600，看是否有一条线可以连接x1和x2之间的部分而不经过任何一个箱子
    for(int lineY = yStart; lineY < 600;){
        if(checkLine(game, player, 2, y1, lineY, x1) &&
           checkLine(game, player, 2, y2, lineY, x2)){
            if(checkLine(game, player, 1, x1, x2, lineY) && !checkExist(game, player, x1, lineY) && !checkExist(game, player, x2, lineY)){
                path.push_back(QPair<int,int>(x1, lineY));
                path.push_back(QPair<int,int>(x2, lineY));
                player->linePath.push_back(QPoint(x1,y1));
                player->linePath.push_back(QPoint(x1,lineY));
                player->linePath.push_back(QPoint(x2,lineY));
                player->linePath.push_back(QPoint(x2,y2));
                return path;
            }
        }
        if(lineY <= 550){
            lineY += game->boxHeight;
        } else {
            lineY += 2;
        }
    }
    return path;
}

QVector<QPair<int,int>> SelectChecker::getEdgeBox(LinkGame *game, Player *player)
{
    //获取周边存在一处没有被包围起来的箱子
    QVector<QPair<int,int>>EdgeBox;
    for(int row = 0; row < game->boxRow; row ++){
        for(int col = 0; col < game->boxCol; col ++){
           if(isEdge(game, player, col, row)){
               EdgeBox.push_back(QPair<int,int>(row,col));
           }
        }
    }
    return EdgeBox;
}

bool SelectChecker::isEdge(LinkGame *game, Player *player, int colLoc, int rowLoc)
{
    //判断是否是边缘箱子
    if(player->checkColIndexValid( colLoc) && player->checkRowIndexValid( rowLoc) && game->boxMap[rowLoc][colLoc]->boxState.boxRemoved){
        return false;
    }
    //在地图边缘必然是边缘箱子
    if(colLoc == 0 || rowLoc == 0 || colLoc == game->boxCol - 1 || rowLoc == game->boxRow - 1){
        return true;
    }
    //判断是否有一个方向有箱子被消除了
    if(player->checkColIndexValid( colLoc - 1) && player->checkRowIndexValid( rowLoc) && game->boxMap[rowLoc][colLoc - 1]->boxState.boxRemoved){
        return true;
    } else if(player->checkColIndexValid( colLoc + 1) && player->checkRowIndexValid( rowLoc) && game->boxMap[rowLoc][colLoc + 1]->boxState.boxRemoved){
        return true;
    } else if(player->checkColIndexValid( colLoc) && player->checkRowIndexValid( rowLoc - 1) && game->boxMap[rowLoc - 1][colLoc]->boxState.boxRemoved){
        return true;
    } else if(player->checkColIndexValid( colLoc) && player->checkRowIndexValid( rowLoc + 1) && game->boxMap[rowLoc + 1][colLoc]->boxState.boxRemoved){
        return true;
    }
    return false;
}

bool SelectChecker::solutionExist(LinkGame *game, Player *player)
{
    //检查是否有解
    QVector<QPair<int,int>>edgeBox = getEdgeBox(game,player);
    Player * tmpPlayer = new Player(0,0,0,0,0,game);
    for(int i = 0;i < edgeBox.size();i ++){
        for(int j = i+1;j < edgeBox.size();j ++){
            //遍历所有的箱子，看看是否有两个箱子可以消除
            tmpPlayer->currentSelected.push_back(QPair<int,int>(edgeBox[i].first,edgeBox[i].second));
            tmpPlayer->currentSelected.push_back(QPair<int,int>(edgeBox[j].first,edgeBox[j].second));
            if(tmpPlayer->currentSelected.size() != 2){
                tmpPlayer->currentSelected.clear();
                continue;
            }
            if(checkValidReturnBool(game,tmpPlayer)){
                return true;
            } else {
                tmpPlayer->currentSelected.clear();
            }
        }
    }
    game->noSolution = true;
    return false;
}

bool SelectChecker::checkValidReturnBool(LinkGame *game, Player *player)
{
    //检查是否有解，返回bool值
    BoxOfGame* box1 = game->boxMap[player->currentSelected[0].first][player->currentSelected[0].second];
    BoxOfGame* box2 = game->boxMap[player->currentSelected[1].first][player->currentSelected[1].second];
    if(box1->typeOfBox != box2->typeOfBox)return false;
    int x1 = box1->LeftTopX + game->boxWidth/2;
    int y1 = box1->LeftTopY + game->boxHeight/2;
    int x2 = box2->LeftTopX + game->boxWidth/2;
    int y2 = box2->LeftTopY + game->boxHeight/2;

    //零折线
    if(x1 == x2){
        if(checkLine(game,player,2,y1,y2,x1)){
            return true;
        }
    } else if(y1 == y2){
        if(checkLine(game,player,1,x1,x2,y1)){
            return true;
        }
    }
    //一折线
    /*一折线，交点两个，一个是(x1,y2)，一个是(x2,y1)，那么判断这两段线中间有无障碍，就可以
    判断是否为一折线的情况了，并且还需要判断一下这两个点处是不是有方块*/
    if(checkLine(game,player,2,y1,y2,x1) &&
       checkLine(game,player,1,x1,x2,y2) &&
       !checkExist(game,player,x1,y2)){
        //(x1,y1)->(x1,y2)->(x2,y2)
        return true;
    }
    if(checkLine(game,player,1,x1,x2,y1) &&
       checkLine(game,player,2,y1,y2,x2) &&
       !checkExist(game,player,x2,y1)){
        //(x1,y1)->(x2,y1)->(x2,y2)
        return true;
    }
    //二折线，假定两个箱子各自发出的平行线都沿着x方向，那么遍历y，这里的y确定为箱子坐标，看看是否有一条线可以连接
    //首先看x方向的情况
    //第一步我们确定x与y开始的地方
    int yStart = 5;
    int xStart = 5;
    //然后我们从xStart开始，一直到800，看是否有一条线可以连接y1和y2之间的部分而不经过任何一个箱子
    //需要判断一下交点有无方块
    for(int lineX = xStart; lineX < 800;){
        if(checkLine(game, player, 1, x1, lineX, y1) &&
           checkLine(game, player, 1, x2, lineX, y2)){
            if(checkLine(game, player, 2, y1, y2, lineX)
               && !checkExist(game, player, lineX, y1) && !checkExist(game, player, lineX, y2)){
                return true;
            }
        }
        if(lineX <= 750){
            lineX += game->boxWidth;
        } else {
            lineX += 2;
        }
    }
    //同样，我们从yStart开始，一直到600，看是否有一条线可以连接x1和x2之间的部分而不经过任何一个箱子
    for(int lineY = yStart; lineY < 600;){
        if(checkLine(game, player, 2, y1, lineY, x1) &&
           checkLine(game, player, 2, y2, lineY, x2)){
            if(checkLine(game, player, 1, x1, x2, lineY) && !checkExist(game, player, x1, lineY) && !checkExist(game, player, x2, lineY)){
                return true;
            }
        }
        if(lineY <= 550){
            lineY += game->boxHeight;
        } else {
            lineY += 2;
        }
    }
    //所有可能情况都被遍历到了，但是没有找到合适的路径，返回错误
    return false;
}

QPair<QPair<int,int>,QPair<int,int>> SelectChecker::searchPair(LinkGame *game)
{
    //搜索一对可以消除的箱子，思路同上
    Player * tmpPlayer = new Player(0,0,0,0,0,game);
    QVector<QPair<int,int>>edgeBox = getEdgeBox(game,tmpPlayer);
    for(int i = 0;i < edgeBox.size();i ++){
        for(int j = i+1;j < edgeBox.size();j ++){
            tmpPlayer->currentSelected.push_back(QPair<int,int>(edgeBox[i].first,edgeBox[i].second));
            tmpPlayer->currentSelected.push_back(QPair<int,int>(edgeBox[j].first,edgeBox[j].second));
            if(tmpPlayer->currentSelected.size() != 2){
                tmpPlayer->currentSelected.clear();
                continue;
            }
            if(checkValidReturnBool(game,tmpPlayer)){
                return QPair<QPair<int,int>,QPair<int,int>>(tmpPlayer->currentSelected[0],tmpPlayer->currentSelected[1]);
            } else {
                tmpPlayer->currentSelected.clear();
            }
        }
    }
    return QPair<QPair<int,int>,QPair<int,int>>(QPair<int,int>(-1,-1),QPair<int,int>(-1,-1));
}