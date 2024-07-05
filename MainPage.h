//
// Created by overji on 2024/7/5.
//

#pragma once
#include <QWidget>
#include <QStackedLayout>
#include "LinkGame.h"


class MainPage :public QWidget
{
    Q_OBJECT
public:
    explicit MainPage();
public slots:
    void startGameClicked();
    void gameTypeClicked();
    void startGame();
    void returnMainPage();
    void endGame();
    void loadGame();
private:
    QStackedLayout *stackMainLayout;
    QWidget *mainWidget;
    QWidget *startWidget;
    void initMainPage();
    void initStartGamePage();
//    void initSettingPage();
    void setBackGround(QString path);
};

