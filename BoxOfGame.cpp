//
// Created by overji on 2024/6/20.
//

#include "BoxOfGame.h"
#include <iostream>
#include <QPainter>

BoxOfGame::BoxOfGame(const int & boxWid , const int & boxHeit,const int & LeftTopX ,const int & LeftTopY  ,const QColor boxColorInput
        , const QColor boarderColorInput,const int TypeOfBoxInput, QWidget * parent): QWidget(parent)
{
    //BoxOfGame的构造函数
    //确定箱子边框大小、箱子大小、箱子含坐标时候左上角位置
    this->boxWidth = boxWid;
    this->boxHeight = boxHeit;
    this->boarderWidth = boxWidth/20;
    this->boarderHeight = boxHeight/20;
    this->LeftTopX = LeftTopX;
    this->LeftTopY = LeftTopY;
    this->boxColor = boxColorInput;
    this->boarderColor = boarderColorInput;
    this->typeOfBox = TypeOfBoxInput;
    this->moveCondition = 0;
    this->boxState.boxSelected = false;
    this->boxState.boxToBeRemoved = false;
    this->boxState.closeBox = false;
    boxState.boxRemoved = false;
    boxState.boxHinted = false;
}

void BoxOfGame::drawBox(QPainter &painter)
{
    if(boxState.boxRemoved){
        return;
    }
    this->checkBoxBoarderColor();
    const QPoint Boarders[4] = {
            QPoint(LeftTopX,LeftTopY),
            QPoint(LeftTopX+boxWidth,LeftTopY),
            QPoint(LeftTopX+boxWidth,LeftTopY+boxHeight),
            QPoint(LeftTopX,LeftTopY+boxHeight)
    };
    const QPoint InnerBox[4] = {
            QPoint(LeftTopX+boarderWidth,LeftTopY+boarderHeight),
            QPoint(LeftTopX+boxWidth-boarderWidth,LeftTopY+boarderHeight),
            QPoint(LeftTopX+boxWidth-boarderWidth,LeftTopY+boxHeight-boarderHeight),
            QPoint(LeftTopX+boarderWidth,LeftTopY+boxHeight-boarderHeight)
    };
    //绘制箱子
    painter.save();
    painter.setPen(Qt::NoPen);
    painter.setBrush(boarderColor);
    painter.drawConvexPolygon(Boarders,4);
    painter.setBrush(boxColor);
    painter.drawConvexPolygon(InnerBox,4);
    painter.restore();
}

void BoxOfGame::resizeBox(const int &boxWid, const int &boxHeit, const int &leftTopX, const int &leftTopY)
{
    this->boxWidth = boxWid;
    this->boxHeight = boxHeit;
    this->LeftTopX = leftTopX;
    this->LeftTopY = leftTopY;
    this->boarderWidth = boxWid/20;
    this->boarderHeight = boxHeit/20;
}

void BoxOfGame::changeBoarderColor(const QColor &color)
{
    this->boarderColor = color;
}

void BoxOfGame::changeBoxColor(const QColor &color)
{
    this->boxColor = color;
}

void BoxOfGame::changeBoxRemovedState(const bool &state)
{
    this->boxState.boxRemoved = state;
}

void BoxOfGame::checkBoxBoarderColor() {
    if(boxState.boxSelected){
        boarderColor = QColor(255,0,0);
    }  else if(boxState.boxToBeRemoved) {
        boarderColor = QColor(255,0,0);
    } else if(boxState.closeBox){
        boarderColor = QColor(255,255,0);
    } else if(boxState.boxHinted) {
        boarderColor = QColor(255, 13, 167);
    }else {
        boarderColor = QColor(255, 255, 255);
    }
}

int BoxOfGame::addBoxScore()
{
    switch(typeOfBox){
        case 0:
            return 10;
        case 1:
            return 20;
        case 2:
            return 30;
        case 3:
            return 50;
        case 4:
            return 75;
        case 5:
            return 100;
        default:
            return 200;
    }
}

void BoxOfGame::swapBox(BoxOfGame * box1, BoxOfGame * box2)
{
    std::swap(box1->boxColor,box2->boxColor);
    std::swap(box1->boarderColor,box2->boarderColor);
    std::swap(box1->typeOfBox,box2->typeOfBox);
    std::swap(box1->boxState,box2->boxState);
    box1->boxState.closeBox = false;
    box2->boxState.closeBox = false;
    box1->boxState.boxSelected = false;
    box2->boxState.boxSelected = false;
    std::swap(box1->moveCondition,box2->moveCondition);
}