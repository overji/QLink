//
// Created by overji on 2024/6/20.
//

#pragma once

#include <QWidget>
#include "Player.h"
#include "SaveSystem.h"

class Player;
class Gadget;
class SaveSystem;

struct BoxState
{
    bool boxRemoved;
    bool boxSelected;
    bool closeBox;
    bool boxToBeRemoved;
    bool boxHinted;
};

class BoxOfGame : public QWidget
{
    Q_OBJECT
    friend class Player;
    friend class SelectChecker;
    friend class SimpleTest;
    friend class LinkGame;
    friend class Gadget;
    friend class SaveSystem;
public:
    explicit BoxOfGame(const int & boxWid , const int & boxHeit,const int & LeftTopX ,const int & LeftTopY  ,const QColor boxColorInput
                       , const QColor boarderColorInput, const int TypeOfBoxInput , QWidget * parent = nullptr );
    void drawBox(QPainter & painter);
    void resizeBox(const int & boxWid , const int & boxHeit,const int & leftTopX ,const int & leftTopY);
    void changeBoxColor(const QColor &color);
    void changeBoarderColor(const QColor &color);
    void changeBoxRemovedState(const bool & state);
    void static swapBox(BoxOfGame * box1,BoxOfGame * box2);
    int addBoxScore();

private:
    //确定箱子边框大小、箱子大小、箱子含边界时候左上角位置
    void checkBoxBoarderColor();
    int boxWidth;
    int boxHeight;
    int boarderWidth;
    int boarderHeight;
    int LeftTopX;
    int LeftTopY;
    int typeOfBox;
    int moveCondition;
    QColor boxColor;
    QColor boarderColor;
    BoxState boxState;
};