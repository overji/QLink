//
// Created by overji on 2024/7/5.
//

#include "MainPage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QWidget>
#include <QPushButton>
#include <QRadioButton>
#include <QLabel>
#include <QButtonGroup>
#include "SaveSystem.h"


MainPage::MainPage()
{
    initMainPage();
    initStartGamePage();
    stackMainLayout = new QStackedLayout;
    stackMainLayout->addWidget(this->mainWidget);
    stackMainLayout->addWidget(this->startWidget);
    stackMainLayout->setCurrentIndex(0);
    this->setLayout(stackMainLayout);
    resize(800,600);
    this->setBackGround("images/__sys_tm_bg01a.png");
}

void MainPage::initMainPage()
{
    this->mainWidget = new QWidget;
    QGridLayout * mainLayout = new QGridLayout(this);

    QPushButton *startGame = new QPushButton("开始游戏");
    connect(startGame,&QPushButton::clicked,this,&MainPage::startGameClicked);
    QPushButton *exitGame = new QPushButton("退出游戏");
    connect(exitGame,&QPushButton::clicked,this,&MainPage::endGame);
    QPushButton * loadGame = new QPushButton("加载游戏");
    connect(loadGame,&QPushButton::clicked,this,&MainPage::loadGame);
    QVector<QPushButton *> buttons = {startGame,exitGame,loadGame};
    for(auto button:buttons){
        button->setStyleSheet("background-color:rgba(255,255,255,0);"
                              "font-size:20px;"
                              "border:1px solid white;"
                              "border-radius: 5px;");
    }
    QLabel *gameTitle = new QLabel("扣踢连连看");
    gameTitle->setStyleSheet("font-size:50px;"
                             "color: white;"
                             "text-decoration: dotted azure 2px;");
    gameTitle->setAlignment(Qt::AlignCenter);

    mainLayout->addWidget(new QLabel,0,0,2,16);
    mainLayout->addWidget(new QLabel,2,0,4,4);
    mainLayout->addWidget(gameTitle,2,4,4,8);
    mainLayout->addWidget(new QLabel,2,12,4,4);
    mainLayout->addWidget(new QLabel,6,0,4,16);
    mainLayout->addWidget(startGame,10,1,2,4);
    mainLayout->addWidget(loadGame,10,6,2,4);
    mainLayout->addWidget(exitGame,10,11,2,4);
    this->mainWidget->setLayout(mainLayout);
}

void MainPage::setBackGround(QString path)
{
    QPalette palette;
    QPixmap pixmap(QPixmap(path).scaled(this->size(),Qt::IgnoreAspectRatio,Qt::SmoothTransformation));
    palette.setBrush(QPalette::Window,QBrush(pixmap));
    this->setPalette(palette);
    setAutoFillBackground(true);
}

void MainPage::initStartGamePage()
{
    this->startWidget = new QWidget;
    QGridLayout * startLayout = new QGridLayout;
    QLabel * MHint = new QLabel("几行方块(请输入2~16之间的整数)?");
    QLabel * NHint = new QLabel("几列方块(请输入2~16之间的整数)?");
    QLabel * oddHint = new QLabel("请确保行数和列数至少有一个为整数!");
    QLabel * gameTypeHint = new QLabel("游戏模式: ");
    QLabel * title = new QLabel("游戏启动设置");
    QLabel * gameHint = new QLabel("W向上移动,S向下移动,A向左移动,D向右移动,Esc暂停游戏");
    QPushButton * startGameButton = new QPushButton("开始游戏");
    connect(startGameButton,&QPushButton::clicked,this,&MainPage::startGame);
    QPushButton * cancelButton = new QPushButton("取消");
    connect(cancelButton,&QPushButton::clicked,this,&MainPage::returnMainPage);
    QRadioButton * singleGame = new QRadioButton("单人游戏");
    singleGame->setChecked(true);
    QRadioButton * multiGame = new QRadioButton("多人游戏");
    connect(singleGame,&QRadioButton::clicked,this,&MainPage::gameTypeClicked);
    connect(multiGame,&QRadioButton::clicked,this,&MainPage::gameTypeClicked);
    QSpinBox * MInput = new QSpinBox();
    MInput->setRange(2,16);
    MInput->setValue(6);
    QSpinBox * NInput = new QSpinBox();
    NInput->setRange(2,16);
    NInput->setValue(6);
    QVector<QLabel *> hints = {MHint,NHint,oddHint,gameTypeHint,title,gameHint};
    QVector<QSpinBox *> inputs = {MInput,NInput};
    QVector<QRadioButton *> gameTypes = {singleGame,multiGame};
    QVector<QPushButton *> buttons = {startGameButton,cancelButton};
    for(auto hint:hints){
        hint->setStyleSheet("font-size:20px;");
        hint->setAlignment(Qt::AlignCenter);
    }
    for(auto input:inputs){
        input->setStyleSheet("font-size:20px;");
        input->setAlignment(Qt::AlignCenter);
    }
    for(auto gameType:gameTypes){
        gameType->setStyleSheet("font-size:20px;");
    }
    for(auto button:buttons){
        button->setStyleSheet("background-color:rgba(255,255,255,0);"
                              "font-size:20px;"
                              "border:1px solid white;"
                              "border-radius: 5px;");
    }
    title->setStyleSheet("font-size:20px;");

    startLayout->addWidget(title,0,0,1,8);
    startLayout->addWidget(MHint,1,0,1,4);
    startLayout->addWidget(MInput,1,4,1,4);
    startLayout->addWidget(NHint,2,0,1,4);
    startLayout->addWidget(NInput,2,4,1,4);
    startLayout->addWidget(oddHint,3,0,1,8);
    startLayout->addWidget(gameTypeHint,4,0,1,4);
    startLayout->addWidget(singleGame,4,4,1,2);
    startLayout->addWidget(multiGame,4,6,1,2);
    startLayout->addWidget(gameHint,5,0,1,8);
    startLayout->addWidget(startGameButton,6,1,1,2);
    startLayout->addWidget(cancelButton,6,5,1,2);
    this->startWidget->setLayout(startLayout);
}

void MainPage::gameTypeClicked()
{
    QRadioButton * singleGame = qobject_cast<QRadioButton *>(qobject_cast<QGridLayout*>(startWidget->layout())->itemAtPosition(4, 4)->widget());
    QLabel * gameHint = qobject_cast<QLabel *>(qobject_cast<QGridLayout*>(startWidget->layout())->itemAtPosition(5,0)->widget());
    if(singleGame->isChecked()){
        gameHint->setText("W向上移动,S向下移动,A向左移动,D向右移动,Esc、空格暂停");
    } else {
        gameHint->setText("↑向上移动,↓向下移动,←向左移动,→向右移动,Esc、空格暂停");
    }
}

void MainPage::startGameClicked()
{
    this->stackMainLayout->setCurrentIndex(1);
}

void MainPage::startGame()
{
    QSpinBox * MInput = qobject_cast<QSpinBox *>(qobject_cast<QGridLayout*>(startWidget->layout())->itemAtPosition(1,4)->widget());
    QSpinBox * NInput = qobject_cast<QSpinBox *>(qobject_cast<QGridLayout*>(startWidget->layout())->itemAtPosition(2,4)->widget());
    QRadioButton * singleGame = qobject_cast<QRadioButton *>(qobject_cast<QGridLayout*>(startWidget->layout())->itemAtPosition(4, 4)->widget());
    int M = MInput->value();
    int N = NInput->value();
    int gameType = singleGame->isChecked()?0:1;
    LinkGame * game = new LinkGame(M,N,gameType);
    game->show();
    this->close();
}

void MainPage::returnMainPage()
{
    this->stackMainLayout->setCurrentIndex(0);
}

void MainPage::endGame()
{
    std::exit(0);
}

void MainPage::loadGame()
{
    SaveSystem::loadGame()->show();
    this->close();
}