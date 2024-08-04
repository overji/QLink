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
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>

LinkGame::LinkGame(const int& M , const int& N, int gameTypeInput ,const int &remainTimeInput, QWidget * parent): QWidget(parent)
{
    //检查输入是否合法
    if((M * N) % 2 != 0){
        throw std::runtime_error("In M and N there must be an even number!");
    }
    if(M > 16 || N >= 16){
        throw std::runtime_error("M and N must be a number less than 17.");
    }
    //LinkGame的构造函数

    //设置窗口标题和大小
    setWindowIcon(QIcon("images/icon.png"));
    setWindowTitle(tr("连连看")); //设置窗口标题
    resize(800,600); //设置窗口大小

    //初始化LinkGame的各个参数
    this->initGlobalBox(M,N);
    this->removeText = "";
    this->summaryText = "";
    this->gameEnd = false;
    this->gamePause = false;
    this->gameType = gameTypeInput;

    this->initTimers(remainTimeInput);
    this->leftTimeText = QString::number(remainTime) + "s";
    this->gadgetSummonPossibility = 0.01;
    this->maxGadgetNumber = 3;

    this->hintedBoxes.resize(2);
    initMap();
    initPlayer(gameTypeInput);
};

LinkGame:: ~LinkGame()
{
    //析构函数
    for(int i = 0; i < boxRow; i ++){
        for(int j = 0; j < boxCol; j ++){
            delete boxMap[i][j];
        }
    }
    delete player1;
    if(player2 != nullptr){
        delete player2;
    }
    for(auto i:gadgets){
        delete i;
    }

    delete remainTimeTimer;
    delete hintTimeTimer;
    delete flashTimeTimer;
    delete dizzyTimeTimer;
    delete freezeTimeTimer;
}

void LinkGame::initGlobalBox(const int& M,const int& N)
{
    //初始化箱子的种类对应颜色，初始化boxMap和boxType大小
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
    //初始化计时器
    QTimer *gameFlashTimer = new QTimer(this);
    //将计时器的超时事件与LinkGame的update函数绑定，即一旦计时器时间到了，就会调用一次update函数
    connect(gameFlashTimer,&QTimer::timeout,this,QOverload<>::of(&LinkGame::update));
    gameFps = 60;
    int gameFlashMs = 1000/gameFps; //每秒刷新gameFps次
    gameFlashTimer->start(gameFlashMs); //计时器开始计时

    remainTime = (remainTimeInput == 0) ? 3 * boxRow * boxCol : remainTimeInput; //初始化剩余时间
    remainTimeTimer = new QTimer(); //初始化计时器
    connect(remainTimeTimer,SIGNAL(timeout()),this,SLOT(updateRemainTime())); //连接计时器的timeout信号和槽函数
    remainTimeTimer->start(1000); //开始计时

    this->hintTime = 0; //初始化提示时间
    hintTimeTimer = new QTimer(); //初始化计时器
    connect(hintTimeTimer,SIGNAL(timeout()),this,SLOT(updateHintTime())); //连接计时器的timeout信号和槽函数
    hintTimeTimer->start(1000); //开始计时
    hintTimerOn = false;    //初始化定时器关闭状态

    this->flashTime = 0; //初始化flash时间
    flashTimeTimer = new QTimer(); //初始化计时器
    connect(flashTimeTimer,SIGNAL(timeout()),this,SLOT(updateFlashTime())); //连接计时器的timeout信号和槽函数
    flashTimeTimer->start(1000); //开始计时
    flashTimerOn = false;   //初始化定时器关闭状态

    this->dizzyTimerOn = false; //初始化dizzy时间
    dizzyTimeTimer = new QTimer(); //初始化计时器
    connect(dizzyTimeTimer,SIGNAL(timeout()),this,SLOT(updateDizzyTime())); //连接计时器的timeout信号和槽函数
    dizzyTimeTimer->start(1000); //开始计时

    this->freezeTimerOn = false; //初始化freeze时间
    freezeTimeTimer = new QTimer(); //初始化计时器
    connect(freezeTimeTimer,SIGNAL(timeout()),this,SLOT(updateFreezeTime())); //连接计时器的timeout信号和槽函数
    freezeTimeTimer->start(1000); //开始计时
}

void LinkGame::paintEvent(QPaintEvent *event)
{
    //绘制函数
    gameUpdate(); //更新游戏状态
    QPainter painter(this); //创建一个QPainter对象
    painter.setRenderHint(QPainter::Antialiasing); //设置抗锯齿
    //设置坐标变换，变换大小便于后续绘图
    QSize size = this->size();
    this->xScaleRatio = size.width()/800.0;
    this->yScaleRatio = size.height()/600.0;
    painter.scale(xScaleRatio,yScaleRatio);

    //绘制地图，玩家，线路，道具等
    drawMap(painter);
    player1->drawPlayer(painter);
    if(player2 != nullptr){
        player2->drawPlayer(painter);
    }
    player1->drawLine(painter);
    if(player2 != nullptr){
        player2->drawLine(painter);
    }
    for(auto i:gadgets){
        i->drawGarget(painter);
    }

    //绘制游戏消除提示语和剩余时间提示语
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 20));
    painter.drawText(QRect(200,0,200,this->passageHeight),Qt::AlignCenter,this->removeText);
    painter.drawText(QRect(400,0,200,this->passageHeight),Qt::AlignCenter,this->leftTimeText);
    //提示玩家分数
    painter.setFont(QFont("Arial", 12));
    painter.drawText(QRect(0,550,100,50),Qt::AlignCenter,player1->scoreString);
    if(player2 != nullptr){
        painter.drawText(QRect(700,550,100,50),Qt::AlignCenter,player2->scoreString);
    }
    //绘制游戏结束页面和游戏暂停页面
    if(gamePause){
        drawPausePage(painter);
    }
    if(gameEnd){
        this->removeText = "";
        drawEndPage(painter);
    }
}



void LinkGame::resizeEvent(QResizeEvent *event)
{
    //再窗口大小变换时，重置箱子大小和边框宽度，最后确定全部箱子的位置
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
    //调整玩家大小
    player1->resizePlayer(xScaleRatio,yScaleRatio);
    if(player2 != nullptr){
        player2->resizePlayer(xScaleRatio,yScaleRatio);
    }
}

void LinkGame::drawMap(QPainter &painter) {
    //绘制地图
    for(int x = 0; x < boxRow; x ++){
        for(int y = 0; y < boxCol; y ++){
            boxMap[x][y]->drawBox(painter);
        }
    }
}

void LinkGame::drawEndPage(QPainter &painter)
{
    //绘制游戏结束页面
    //绘制背景
    painter.setBrush(QColor(61, 199, 255 ,233));
    QRect rect(0,0,800,600);
    painter.drawConvexPolygon(rect);
    if(this->layout() != nullptr){
        return;
    }
    //按钮和标签
    QGridLayout *layout = new QGridLayout;
    QPushButton *newGameButton = new QPushButton("新游戏");
    QPushButton *loadButton = new QPushButton("加载游戏");
    QPushButton *exitButton = new QPushButton("退出游戏");
    QLabel * pauseLabel = new QLabel("游戏结束");
    //判断是否无解
    if(noSolution){
        pauseLabel->setText("无解，游戏结束");
    }
    //玩家分数的提示语
    QLabel * player1Score = new QLabel(player1->scoreString);
    QLabel * player2Score;
    if(player2 != nullptr){
        player2Score = new QLabel(QString("玩家2") + player2->scoreString);
        player1Score = new QLabel(QString("玩家1") + player1->scoreString);
    } else {
        player2Score = new QLabel("");
    }
    //设置标签和按钮的样式
    QVector<QLabel*>labels = {pauseLabel,player1Score,player2Score};
    for(auto label:labels){
        label->setStyleSheet("font-size: 30px;");
        label->setAlignment(Qt::AlignCenter);
    }
    QVector<QPushButton*>buttons = {newGameButton,loadButton,exitButton};
    for(auto i:buttons){
        i->setStyleSheet("font-size: 20px;"
                         "background-color: rgb(255, 255, 255);"
                         "border: 2px solid black;"
                         "border-radius: 6px;"
                         "padding: 3px;");
    }
    //连接按钮的槽函数
    connect(newGameButton,&QPushButton::clicked,this,&LinkGame::startNewGame);
    connect(loadButton,&QPushButton::clicked,this,&LinkGame::loadGame);
    connect(exitButton,&QPushButton::clicked,[this](){
        this->close();
    });
    //添加标签和按钮到layout中
    layout->addWidget(pauseLabel,0,0,1,8);
    layout->addWidget(player1Score,1,0,1,4);
    layout->addWidget(player2Score,1,4,1,4);
    layout->addWidget(newGameButton,2,2,1,4);
    layout->addWidget(loadButton,3,2,1,4);
    layout->addWidget(exitButton,4,2,1,4);
    layout->addWidget(new QLabel,5,0,1,8);
    //设置layout
    this->setLayout(layout);
}

void LinkGame::drawPausePage(QPainter &painter)
{
    //绘制游戏暂停页面
    //绘制背景
    painter.setBrush(QColor(255, 241, 250,210));
    QRect rect(0,0,800,600);
    painter.drawConvexPolygon(rect);
    if(this->layout() != nullptr){
        return;
    }
    //按钮和标签
    QGridLayout *layout = new QGridLayout;
    QPushButton *resumeButton = new QPushButton("继续游戏");
    QPushButton *saveButton = new QPushButton("保存游戏");
    QPushButton *exitButton = new QPushButton("退出游戏");
    QLabel * pauseLabel = new QLabel("游戏暂停");
    pauseLabel->setStyleSheet("font-size: 30px;");
    pauseLabel->setAlignment(Qt::AlignCenter);
    QVector<QPushButton*>buttons = {resumeButton,saveButton,exitButton};
    //设置按钮样式
    for(auto i:buttons){
        i->setStyleSheet("font-size: 20px;"
                         "background-color: rgb(255, 255, 255);"
                         "border: 2px solid black;"
                         "border-radius: 6px;"
                         "padding: 3px;");
    }
    //连接按钮的槽函数
    connect(resumeButton,&QPushButton::clicked,this,&LinkGame::setGamePauseNoValue);
    connect(saveButton,&QPushButton::clicked,[this](){
        SaveSystem::saveGame(this);
    });
    connect(exitButton,&QPushButton::clicked,[this](){
        this->close();
    });
    //添加标签和按钮到layout中
    layout->addWidget(pauseLabel,0,0,1,8);
    layout->addWidget(resumeButton,2,2,1,4);
    layout->addWidget(saveButton,3,2,1,4);
    layout->addWidget(exitButton,4,2,1,4);
    layout->addWidget(new QLabel,5,0,1,8);
    this->setLayout(layout);
}

void LinkGame::initBoxType()
{
    //初始化箱子的种类
    this->remainBoxNumber = boxCol * boxRow; //箱子总数
    QVector<int> boxTypeList(remainBoxNumber); //箱子种类列表
    int cur = 0;
    //随机生成箱子种类
    for(int i = 0;i < remainBoxNumber;i ++)
    {
        boxTypeList[i] = -1;
    }
    for(int i = 0;i < remainBoxNumber;i ++)
    {
        //如果箱子种类已经被初始化，就跳过
        if(boxTypeList[i] != -1)continue;
        //随机生成箱子种类，每个箱子种类有对应的概率
        int randomNum = QRandomGenerator::global()->bounded(1,1001);
        int randomType = 0;
        //箱子种类对应的概率
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
        //另一个箱子的位置随机生成
        boxTypeList[i] = randomType;
        int randomLoc = QRandomGenerator::global()->bounded(i+1,remainBoxNumber);
        while(boxTypeList[randomLoc] != -1)
        {
            randomLoc = QRandomGenerator::global()->bounded(i+1,remainBoxNumber);
        }
        boxTypeList[randomLoc] = randomType;
    }
    //将箱子种类列表转化为箱子种类矩阵
    for(int i = 0; i < boxRow; i ++){
        for(int j = 0; j < boxCol; j ++){
            boxType[i][j] = boxTypeList[cur++];
        }
    }
}

void LinkGame::initMap()
{
    //初始化箱子
    QSize size = this->size();
    //设置窗口最小大小
    setMinimumSize(QSize(400,300));
    this->xScaleRatio = size.width()/800.0;
    this->yScaleRatio = size.height()/600.0;
    double displayWidth = qMin((800*(this->xScaleRatio))/(boxCol + 2), (600 * (this->yScaleRatio)) / (boxRow + 2));
    //确定箱子大小和边框宽度，最后确定全部箱子的位置
    this->boxWidth = displayWidth/xScaleRatio;
    this->boxHeight = displayWidth/yScaleRatio;
    this->passageWidth = (800- boxWidth * boxCol) / 2;
    this->passageHeight = (600- boxHeight * boxRow) / 2;
    initBoxType(); //初始化箱子种类
    for(int row = 0; row < boxRow; row ++){
        for(int col = 0; col < boxCol; col ++){
            //箱子的左上角坐标
            int LeftTopX = passageWidth + col*boxWidth;
            int LeftTopY = passageHeight + row*boxHeight;
            //箱子的颜色和边框颜色
            QColor boxColor = typeToColor[boxType[row][col]];
            QColor boarderColor = QColor(255,255,255);
            //初始化箱子
            BoxOfGame * box = new BoxOfGame(boxWidth,boxHeight,LeftTopX,LeftTopY,boxColor,boarderColor,boxType[row][col]);
            boxMap[row][col] = box;
        }
    }
}

void LinkGame::initPlayer(int gameTypeInput)
{
    //初始化玩家
    int playerWidth = 20; //玩家宽度
    int playerHeight = (1*playerWidth*this->xScaleRatio)/this->yScaleRatio; //玩家高度
    int initialSpeed = 15; //玩家初始速度
    //初始化玩家1
    player1 = new Player(playerWidth,300-playerHeight/2,playerWidth,playerHeight,initialSpeed,this,1);
    if(gameTypeInput){
        //如果为双人游戏，初始化玩家2
        player2 = new Player(800-playerWidth-20,300-playerHeight/2,playerWidth,playerHeight,initialSpeed,this,2);
    } else {
        player2 = nullptr;
    }
}

void LinkGame::keyPressEvent(QKeyEvent *event)
{
    //键盘事件
    switch (event->key()) {
        //玩家1的移动
        case Qt::Key_W:
            player1->playerMove(0);
            break;
        case Qt::Key_S:
            player1->playerMove(1);
            break;
        case Qt::Key_A:
            player1->playerMove(2);
            break;
        case Qt::Key_D:
            player1->playerMove(3);
            break;
        //玩家2的移动
        case Qt::Key_Up:
            player2->playerMove(0);
            break;
        case Qt::Key_Down:
            player2->playerMove(1);
            break;
        case Qt::Key_Left:
            player2->playerMove(2);
            break;
        case Qt::Key_Right:
            player2->playerMove(3);
            break;
        //暂停游戏
        case Qt::Key_Space:
            setGamePause(!gamePause);
            break;
        case Qt::Key_Escape:
            setGamePause(!gamePause);
            break;
        default:
            break;
    }
}



void LinkGame::checkGameEnd()
{
    //检查游戏是否结束
    if(remainBoxNumber <= 0)
    {
        //如果没有箱子，游戏结束
        this->gameEnd = true;
        this->gamePause = false;
        this->summaryText = "";
    }
}

void LinkGame::updateRemainTime()
{
    if(gamePause){
        //如果游戏暂停，就不更新时间
        return;
    }
    //更新剩余时间
    remainTime--;
    this->leftTimeText = QString::number(remainTime) + "s";
    if(remainTime == 0){
        //剩余时间为0，游戏结束
        gameEnd = true;
        gamePause = false;
        this->summaryText = "时间到，游戏结束!";
    }
    if(gameEnd){
        //游戏结束，停止计时器
        this->remainTimeTimer->stop();
    }
}

void LinkGame::generateGadget()
{
    if (gadgets.size() >= maxGadgetNumber) {
        //如果道具数量已经达到最大值，就不生成道具
        return;
    }
    double randomPossibility = QRandomGenerator::global()->bounded(100000000)/100000000.0; //生成一个随机数
    if(randomPossibility < gadgetSummonPossibility) {
        //如果随机数小于生成道具的概率，就生成道具
        auto *garget = new Gadget(this, this->gameType);
        gadgets.push_back(garget);
    }
}

void LinkGame::shuffleMap()
{
    //打乱箱子
    clearHintBox(); //清除提示
    QVector<QPair<int,int>> boxesOfMap;
    //找出所有没有被删除的箱子
    for(int row = 0;row < boxRow;row ++){
        for(int col = 0;col < boxCol;col ++){
            if(!boxMap[row][col]->getBoxState().boxRemoved){
                boxesOfMap.push_back(QPair<int,int>(row,col));
            }
        }
    }
    while(!boxesOfMap.empty()){
        //随机交换箱子
        //获取第一个箱子
        int randomIndex = QRandomGenerator::global()->bounded(0,boxesOfMap.size());
        int firstRow = boxesOfMap[randomIndex].first;
        int firstCol = boxesOfMap[randomIndex].second;
        boxesOfMap.removeAt(randomIndex); //移除箱子
        //获取第二个箱子
        int secondRandonIndex = QRandomGenerator::global()->bounded(0,boxesOfMap.size());
        int secondRow = boxesOfMap[secondRandonIndex].first;
        int secondCol = boxesOfMap[secondRandonIndex].second;
        boxesOfMap.removeAt(secondRandonIndex);
        BoxOfGame::swapBox(boxMap[firstRow][firstCol],boxMap[secondRow][secondCol]); //交换箱子
    }
}

void LinkGame::hintBox()
{
    //提示箱子
    if(!hintTimerOn){
        //如果没有开启提示，就不提示
        if(!hintedBoxes.empty()){
            //如果hintedBox不为空，清除
            clearHintBox();
        }
        return;
    }
    if(!hintedBoxes.empty()){
        //如果hintedBox不为空
        if(boxMap[hintedBoxes[0].first][hintedBoxes[0].second]->getBoxState().boxRemoved ||
           boxMap[hintedBoxes[1].first][hintedBoxes[1].second]->getBoxState().boxRemoved){
            //如果有提示的箱子被清除了，就清除提示
            hintedBoxes.clear();
        }
        else{
            return;
        }
    }
    //搜索可以消除的箱子
    auto pairs = SelectChecker::searchPair(this);
    this->boxMap[pairs.first.first][pairs.first.second]->setBoxHinted(true);//标记箱子
    this->boxMap[pairs.second.first][pairs.second.second]->setBoxHinted(true);
    hintedBoxes.push_back(pairs.first);
    hintedBoxes.push_back(pairs.second);
}

void LinkGame::clearHintBox()
{
    //清除提示
    if(!hintedBoxes.empty()){
        //如果hintedBox不为空,清除箱子现在被提示的状态
        boxMap[hintedBoxes[0].first][hintedBoxes[0].second]->setBoxHinted(false);
        boxMap[hintedBoxes[1].first][hintedBoxes[1].second]->setBoxHinted(false);
        if(!boxMap[hintedBoxes[0].first][hintedBoxes[0].second]->getBoxState().boxRemoved &&
           !boxMap[hintedBoxes[1].first][hintedBoxes[1].second]->getBoxState().boxRemoved){
            boxMap[hintedBoxes[0].first][hintedBoxes[0].second]->setBoxHinted(false);
            boxMap[hintedBoxes[1].first][hintedBoxes[1].second]->setBoxHinted(false);
        }
        hintedBoxes.clear();
    }
}
void LinkGame::updateHintTime()
{
    //更新提示时间
    if(gamePause){
        //如果游戏暂停，就不更新时间
        return;
    }
    if(!hintTimerOn){
        //如果没有开启提示，就不更新时间
        return;
    }
    hintTime--; //更新hintTime
    if(hintTime <= 0){
        //如果提示时间小于等于0，就关闭提示
        hintTimerOn = false;
    }
}

void LinkGame::updateFlashTime()
{
    //更新flash时间
    if(gamePause){
        //如果游戏暂停，就不更新时间
        return;
    }
    if(!flashTimerOn){
        //如果没有开启flashTimer，就不更新时间
        return;
    }
    flashTime--;
    if(flashTime <= 0){
        //如果flash时间小于等于0，就关闭flash
        flashTimerOn = false;
    }
}

void LinkGame::updateDizzyTime() {
    //更新dizzy时间
    if (gamePause) {
        //如果游戏暂停，就不更新时间
        return;
    }
    if (!dizzyTimerOn) {
        //如果没有开启dizzyTimer，就不更新时间
        return;
    }
    if(player1->dizzyTime > 0){
        //如果玩家1的dizzy时间大于0，就减少时间
        player1->dizzyTime--;
    }
    if(player2->dizzyTime > 0){
        //如果玩家2的dizzy时间大于0，就减少时间
        player2->dizzyTime--;
    }
    if(player1->dizzyTime == 0 && player2->dizzyTime == 0){
        //如果玩家1和玩家2的dizzy时间都为0，就关闭dizzyTimer
        dizzyTimerOn = false;
    }
}

void LinkGame::updateFreezeTime() {
    //更新freeze时间
    if (gamePause) {
        //如果游戏暂停，就不更新时间
        return;
    }
    if (!freezeTimerOn) {
        //如果没有开启freezeTimer，就不更新时间
        return;
    }
    if(player1->freezeTime > 0){
        //如果玩家1的freeze时间大于0，就减少时间
        player1->freezeTime--;
    }
    if(player2->freezeTime > 0){
        //如果玩家2的freeze时间大于0，就减少时间
        player2->freezeTime--;
    }
    if(player1->freezeTime == 0 && player2->freezeTime == 0){
        //如果玩家1和玩家2的freeze时间都为0，就关闭freezeTimer
        freezeTimerOn = false;
    }
}


void LinkGame::mousePressEvent(QMouseEvent *event)
{
    //鼠标事件
    if(gameType == 1){
        //如果是双人游戏，就不响应鼠标事件
        return;
    }
    if(gamePause){
        //如果游戏暂停，就不响应鼠标事件
        return;
    }
    if(!flashTimerOn){
        //如果没有开启flashTimer，就不响应鼠标事件
        return;
    }
    //获取鼠标点击的箱子的位置
    int x = event->x()/xScaleRatio;
    int y = event->y()/yScaleRatio;
    //玩家1的鼠标点击事件
    player1->playerMove(-5,x,y);
}

void LinkGame::setGamePauseNoValue() {
    //设置游戏暂停
    if(!gamePause){
        //如果游戏被重新开启，就清除暂停页面
        QLayout *layout = this->layout();
        if (layout)
        {
            QLayoutItem *item;
            while ((item = layout->takeAt(0)) != nullptr)
            {
                delete item->widget();
                delete item;
            }
            delete layout;
        }
        gamePause = true;
    }
    else{
        gamePause = false;
    }
}

void LinkGame::startNewGame()
{
    //开始新游戏
    MainPage *mainPage = new MainPage();
    mainPage->show();
    this->close();
}

void LinkGame::loadGame()
{
    //加载游戏
    LinkGame *game = SaveSystem::loadGame();
    game->show();
    this->close();
}

void LinkGame::gameUpdate()
{
    //游戏更新
    //检查玩家的临近箱子情况
    clearClose();
    player1->checkClose();
    if(player2 != nullptr){
        player2->checkClose();
    }
    if(gameEnd || gamePause)return;
    generateGadget(); //生成道具
    hintBox(); //提示箱子
}

void LinkGame::clearClose()
{
    //清除玩家原先临近方块的临近情况
    for(int i = 0;i < boxRow; i ++)
    {
        for(int j = 0;j < boxCol; j ++)
        {
            boxMap[i][j]->setBoxClose(false);
        }
    }
}
