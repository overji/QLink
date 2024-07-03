//
// Created by overji on 2024/6/20.
//

#include "LinkGame.h"
#include "Gadget.h"
#include "Player.h"

#include <iostream>
#include <QPainter>
#include <QKeyEvent>
#include <QTimer>
#include <QRandomGenerator>

LinkGame::LinkGame(const int& M , const int& N, QWidget * parent,const int &remainTimeInput): QWidget(parent)
{
    if((M * N) % 2 != 0){
        throw std::runtime_error("In M and N there must be an even number!");
    }
    if(M > 16 || N >= 16){
        throw std::runtime_error("M and N must be a number less than 17.");
    }
    //LinkGame的构造函数
    //构建一个计时器

    setWindowTitle(tr("连连看")); //设置窗口标题
    resize(800,600); //设置窗口大小

    this->initGlobalBox(M,N);
    this->removeText = "";
    this->summaryText = "";
    this->gameEnd = false;
    this->gamePause = false;

    this->initTimers(remainTimeInput);
    this->leftTimeText = QString::number(remainTime) + "s";
    this->gadgetSummonPossibility = 0.01;
    this->maxGadgetNumber = 3;

    this->hintedBoxes.resize(2);
    initMap();
    initPlayer();
};

void LinkGame::initGlobalBox(const int& M,const int& N)
{
    this->boxCol = N; //N列
    this->boxRow = M; //M行
    types = 7; //箱子种类
    this->typeToColor.push_back(QColor(32, 178, 170));//青
    this->typeToColor.push_back(QColor(255, 182, 193));//粉
    this->typeToColor.push_back(QColor(0, 255, 255));//蓝
    this->typeToColor.push_back(QColor(0, 255, 127));//绿
    this->typeToColor.push_back(QColor(255, 99, 71));//红
    this->typeToColor.push_back(QColor(138, 43, 226));//紫
    this->typeToColor.push_back(QColor(255, 215, 0));//金
    this->boxType.resize(M);
    for (int i = 0; i < M; ++i) {
        boxType[i].resize(N);
    }
    this->boxMap.resize(M);
    for (int i = 0; i < M; ++i) {
        boxMap[i].resize(N);
    }
}

void LinkGame::initTimers(const int &remainTimeInput)
{
    QTimer *gameFlashTimer = new QTimer(this);
    //将计时器的超时事件与LinkGame的update函数绑定，即一旦计时器时间到了，就会调用一次update函数
    connect(gameFlashTimer,&QTimer::timeout,this,QOverload<>::of(&LinkGame::update));
    gameFps = 60;
    int gameFlashMs = 1000/gameFps; //每秒刷新gameFps次
    gameFlashTimer->start(gameFlashMs); //计时器开始计时

    removeBoxTimer = new QTimer(this);  // 初始化QTimer对象
    connect(removeBoxTimer, SIGNAL(timeout()), this, SLOT(removeBox()));  // 连接QTimer的timeout信号和槽函数
    removeTimerOn = false;  // 初始化定时器关闭状态
    removeBoxTimer->setSingleShot(true);  // 设置定时器为单次触发模式

    remainTime = (remainTimeInput == 0) ? 3 * boxRow * boxCol : remainTimeInput;
    remainTimeTimer = new QTimer();
    connect(remainTimeTimer,SIGNAL(timeout()),this,SLOT(updateRemainTime()));
    remainTimeTimer->start(1000);

    this->hintTime = 0;
    hintTimeTimer = new QTimer();
    connect(hintTimeTimer,SIGNAL(timeout()),this,SLOT(updateHintTime()));
    hintTimeTimer->start(1000);
    hintTimerOn = false;

    this->flashTime = 0;
    flashTimeTimer = new QTimer();
    connect(flashTimeTimer,SIGNAL(timeout()),this,SLOT(updateFlashTime()));
    flashTimeTimer->start(1000);
    flashTimerOn = false;
}

void LinkGame::paintEvent(QPaintEvent *event)
{
    gameUpdate();
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    //设置坐标变换，变换大小便于后续绘图
    QSize size = this->size();
    this->xScaleRatio = size.width()/800.0;
    this->yScaleRatio = size.height()/600.0;
    painter.scale(xScaleRatio,yScaleRatio);

    drawMap(painter);
    player1->drawPlayer(painter);
    drawLine(painter);
    for(auto i:gadgets){
        i->drawGarget(painter);
    }

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 20));
    painter.drawText(QRect(200,0,200,this->passageHeight),Qt::AlignCenter,this->removeText);
    painter.drawText(QRect(400,0,200,this->passageHeight),Qt::AlignCenter,this->leftTimeText);
    painter.setFont(QFont("Arial", 12));
    painter.drawText(QRect(0,550,100,50),Qt::AlignCenter,player1->scoreString);
    if(gamePause){
        drawPauseRect(painter);
    }
    if(gameEnd){
        drawEndRect(painter);
    }
}

void LinkGame::drawLine(QPainter &painter)
{
    if(linePath.size() == 0)return;
    painter.setPen(QPen(QColor(255,0,0),5));
    for(int i = 0; i < linePath.size() - 1; i ++){
        painter.drawLine(linePath[i],linePath[i+1]);
    }
    for(auto i : toBeRemovedBox){
        boxMap[i.first][i.second]->boxState.boxToBeRemoved = true;
    }
    if(!removeTimerOn)
    {
        removeTimerOn = true;
        removeBoxTimer->start(200);
    }
}

void LinkGame::resizeEvent(QResizeEvent *event)
{
    QSize size = this->size();
    this->xScaleRatio = size.width()/800.0;
    this->yScaleRatio = size.height()/600.0;
    double displayWidth = qMin((800*this->xScaleRatio)/(boxCol + 2), (600 * this->yScaleRatio) / (boxRow + 2));
    //确定箱子大小和边框宽度，最后确定全部箱子的位置
    this->boxWidth = displayWidth/xScaleRatio;
    this->boxHeight = displayWidth/yScaleRatio;
    this->passageWidth = (800- boxWidth * boxCol) / 2;
    this->passageHeight = (600- boxHeight * boxRow) / 2;
    for(int row = 0; row < boxRow; row ++){
        for(int col = 0; col < boxCol; col ++){
            int LeftTopX = passageWidth + col*boxWidth;
            int LeftTopY = passageHeight + row*boxHeight;
            boxMap[row][col]->resizeBox(boxWidth,boxHeight,LeftTopX,LeftTopY);
        }
    }
    player1->resizePlayer(xScaleRatio,yScaleRatio);
}

void LinkGame::drawMap(QPainter &painter) {
    for(int x = 0; x < boxRow; x ++){
        for(int y = 0; y < boxCol; y ++){
            boxMap[x][y]->drawBox(painter);
        }
    }
}

void LinkGame::drawEndRect(QPainter &painter)
{
    painter.setBrush(QColor(58, 255, 222));
    QRect rect(200,200,400,200);
    painter.drawConvexPolygon(rect);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial",20));
    painter.drawText(rect,Qt::AlignCenter,summaryText);
}

void LinkGame::drawPauseRect(QPainter &painter)
{
    painter.setBrush(QColor(245, 143, 208));
    QRect rect(200,200,400,200);
    painter.drawConvexPolygon(rect);
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial",20));
    painter.drawText(rect,Qt::AlignCenter,"游戏暂停");
}

void LinkGame::initBoxType()
{
    this->remainBoxNumber = boxCol * boxRow;
    QVector<int> boxTypeList(remainBoxNumber);
    int cur = 0;
    for(int i = 0;i < remainBoxNumber;i ++)
    {
        boxTypeList[i] = -1;
    }
    for(int i = 0;i < remainBoxNumber;i ++)
    {
        if(boxTypeList[i] != -1)continue;
        int randomNum = QRandomGenerator::global()->bounded(1,1001);
        int randomType = 0;
        if(randomNum <= 250){
            randomType = 0;
        } else if(randomNum <= 475){
            randomType = 1;
        } else if(randomNum <= 625){
            randomType = 2;
        } else if(randomNum <= 750){
            randomType = 3;
        } else if(randomNum <= 850){
            randomType = 4;
        } else if(randomNum <= 950){
            randomType = 5;
        } else {
            randomType = 6;
        }
        boxTypeList[i] = randomType;
        int randomLoc = QRandomGenerator::global()->bounded(i+1,remainBoxNumber);
        while(boxTypeList[randomLoc] != -1)
        {
            randomLoc = QRandomGenerator::global()->bounded(i+1,remainBoxNumber);
        }
        boxTypeList[randomLoc] = randomType;
    }
    for(int i = 0; i < boxRow; i ++){
        for(int j = 0; j < boxCol; j ++){
            boxType[i][j] = boxTypeList[cur++];
        }
    }
}

void LinkGame::initMap()
{
    QSize size = this->size();
    setMinimumSize(QSize(400,300));
    this->xScaleRatio = size.width()/800.0;
    this->yScaleRatio = size.height()/600.0;
    double displayWidth = qMin((800*(this->xScaleRatio))/(boxCol + 2), (600 * (this->yScaleRatio)) / (boxRow + 2));
    //确定箱子大小和边框宽度，最后确定全部箱子的位置
    this->boxWidth = displayWidth/xScaleRatio;
    this->boxHeight = displayWidth/yScaleRatio;
    this->passageWidth = (800- boxWidth * boxCol) / 2;
    this->passageHeight = (600- boxHeight * boxRow) / 2;
    initBoxType();
    for(int row = 0; row < boxRow; row ++){
        for(int col = 0; col < boxCol; col ++){
            int LeftTopX = passageWidth + col*boxWidth;
            int LeftTopY = passageHeight + row*boxHeight;
            QColor boxColor = typeToColor[boxType[row][col]];
            QColor boarderColor = QColor(255,255,255);
            //初始化箱子
            BoxOfGame * box = new BoxOfGame(boxWidth,boxHeight,LeftTopX,LeftTopY,boxColor,boarderColor,boxType[row][col]);
            boxMap[row][col] = box;
        }
    }
}

void LinkGame::initPlayer()
{
    int playerWidth = 20;
    int playerHeight = (1*playerWidth*this->xScaleRatio)/this->yScaleRatio;
    int initialSpeed = 15;
    player1 = new Player(playerWidth,300-playerHeight/2,playerWidth,playerHeight,initialSpeed,1);
}

void LinkGame::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
        case Qt::Key_W:
            player1->playerMove(0,this);
            break;
        case Qt::Key_S:
            player1->playerMove(1,this);
            break;
        case Qt::Key_A:
            player1->playerMove(2,this);
            break;
        case Qt::Key_D:
            player1->playerMove(3,this);
            break;
        case Qt::Key_Space:
            setGamePause();
            break;
        case Qt::Key_Escape:
            SaveSystem::saveGame(this);
            break;
        default:
            break;
    }
}

void LinkGame::removeBox()
{
    if(gamePause){
        return;
    }
    for(auto i:toBeRemovedBox){
        boxMap[i.first][i.second]->boxState.boxRemoved = true;
        remainBoxNumber--;
    }
    toBeRemovedBox.clear();
    player1->currentSelected.clear();
    linePath.clear();
    removeTimerOn = false;
    checkGameEnd();
}

void LinkGame::checkGameEnd()
{
    if(remainBoxNumber <= 0)
    {
        this->gameEnd = true;
        this->gamePause = false;
        this->summaryText = "游戏结束!";
    }
    //剩余时间为0，游戏结束
}

void LinkGame::updateRemainTime()
{
    if(gamePause){
        return;
    }
    remainTime--;
    this->leftTimeText = QString::number(remainTime) + "s";
    if(remainTime == 0){
        gameEnd = true;
        gamePause = false;
        this->summaryText = "时间到，游戏结束!";
    }
    if(gameEnd){
        this->remainTimeTimer->stop();
    }
}

void LinkGame::generateGadget()
{
    if (gadgets.size() >= maxGadgetNumber) {
        return;
    }
    double randomPossibility = QRandomGenerator::global()->bounded(100000000)/100000000.0;
    if(randomPossibility < gadgetSummonPossibility) {
        auto *garget = new Gadget(this, 1);
        gadgets.push_back(garget);
    }
}

void LinkGame::shuffleMap()
{
    std::cout << "shuffle!" << std::endl;
    QVector<QPair<int,int>> boxesOfMap;
    //找出所有没有被删除的箱子
    for(int row = 0;row < boxRow;row ++){
        for(int col = 0;col < boxCol;col ++){
            if(!boxMap[row][col]->boxState.boxRemoved){
                boxesOfMap.push_back(QPair<int,int>(row,col));
            }
        }
    }
    while(!boxesOfMap.empty()){
        int randomIndex = QRandomGenerator::global()->bounded(0,boxesOfMap.size());
        int firstRow = boxesOfMap[randomIndex].first;
        int firstCol = boxesOfMap[randomIndex].second;
        boxesOfMap.removeAt(randomIndex);
        int secondRandonIndex = QRandomGenerator::global()->bounded(0,boxesOfMap.size());
        int secondRow = boxesOfMap[secondRandonIndex].first;
        int secondCol = boxesOfMap[secondRandonIndex].second;
        boxesOfMap.removeAt(secondRandonIndex);
        BoxOfGame::swapBox(boxMap[firstRow][firstCol],boxMap[secondRow][secondCol]);
    }
}

void LinkGame::hintBox()
{
    if(!hintTimerOn){
        if(!hintedBoxes.empty()){
            boxMap[hintedBoxes[0].first][hintedBoxes[0].second]->boxState.boxHinted = false;
            boxMap[hintedBoxes[1].first][hintedBoxes[1].second]->boxState.boxHinted = false;
            if(!boxMap[hintedBoxes[0].first][hintedBoxes[0].second]->boxState.boxRemoved &&
               !boxMap[hintedBoxes[1].first][hintedBoxes[1].second]->boxState.boxRemoved){
                boxMap[hintedBoxes[0].first][hintedBoxes[0].second]->boxState.boxHinted = false;
                boxMap[hintedBoxes[1].first][hintedBoxes[1].second]->boxState.boxHinted = false;
            }
            hintedBoxes.clear();
        }
        return;
    }
    if(!hintedBoxes.empty()){
        if(boxMap[hintedBoxes[0].first][hintedBoxes[0].second]->boxState.boxRemoved ||
           boxMap[hintedBoxes[1].first][hintedBoxes[1].second]->boxState.boxRemoved){
            hintedBoxes.clear();
        }
        else{
            return;
        }
    }
    auto pairs = SelectChecker::searchPair(this);
    this->boxMap[pairs.first.first][pairs.first.second]->boxState.boxHinted = true;
    this->boxMap[pairs.second.first][pairs.second.second]->boxState.boxHinted = true;
    hintedBoxes.push_back(pairs.first);
    hintedBoxes.push_back(pairs.second);
}

void LinkGame::updateHintTime()
{
    if(gamePause){
        return;
    }
    if(!hintTimerOn){
        return;
    }
    hintTime--;
    if(hintTime <= 0){
        hintTimerOn = false;
    }
}

void LinkGame::updateFlashTime()
{
    if(gamePause){
        return;
    }
    if(!flashTimerOn){
        return;
    }
    flashTime--;
    if(flashTime <= 0){
        flashTimerOn = false;
    }
}

void LinkGame::mousePressEvent(QMouseEvent *event)
{
    if(gamePause){
        return;
    }
    if(!flashTimerOn){
        return;
    }
    int x = event->x()/xScaleRatio;
    int y = event->y()/yScaleRatio;
    player1->playerMove(-5,this,x,y);
}

void LinkGame::setGamePause()
{
    gamePause = !gamePause;
}

void LinkGame::gameUpdate()
{
    player1->clearClose(this);
    player1->checkClose(this);
    if(gameEnd || gamePause)return;
    generateGadget();
    hintBox();
}

