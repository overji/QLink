//
// Created by overji on 2024/6/20.
//

#pragma once

#include <QWidget>
#include <QTime>
#include <QPair>
#include <QVector>
#include <QQueue>
#include "BoxOfGame.h"
#include "Player.h"
#include "Gadget.h"
#include "SaveSystem.h"
#include "SelectChecker.h"
#include "SpecialAlgorithm.h"
#include "MainPage.h"

class Player;
class BoxOfGame;
class Gadget;
class SaveSystem;

class LinkGame : public QWidget
{
    Q_OBJECT
    friend class Player;
    friend class SelectChecker;
    friend class SimpleTest;
    friend class Gadget;
    friend class SaveSystem;
public:
    explicit LinkGame(const int& M = 10, const int& N = 10,int gameTypeInput = 0,const int &remainTimeInput = 0 ,QWidget * parent = nullptr);
    ~LinkGame();
    void shuffleMap();
    void setGamePause();
public slots:
    void removeBox();
    void updateRemainTime();
    void updateHintTime();
    void updateFlashTime();
    void updateFreezeTime();
    void updateDizzyTime();
    void startNewGame();
    void loadGame();
private:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent * event) override;
    void mousePressEvent(QMouseEvent * event) override;
    void drawMap(QPainter & painter);
    void drawEndPage(QPainter & painter);
    void drawPausePage(QPainter & painter);
    void initGlobalBox(const int& M,const int& N);
    void initMap();
    void initTimers(const int &remainTimeInput);
    void initBoxType();
    void initPlayer(int gameTypeInput);
    void gameUpdate();
    void drawLine(QPainter & painter);
    void checkGameEnd();
    void generateGadget();
    void hintBox();
    void clearHintBox();


    int gameFps;
    int remainBoxNumber;
    int boxCol;
    int boxRow;
    int boxWidth;
    int boxHeight;
    int passageWidth;
    int passageHeight;
    double xScaleRatio;
    double yScaleRatio;
    bool gameEnd;
    bool gamePause;
    bool noSolution;
    int remainTime;
    int gameType;//0是单人游戏，1是双人游戏

    QVector<QColor> typeToColor;
    QVector<QVector<int>> boxType;
    QVector<QVector<BoxOfGame *>> boxMap;
    QString removeText;
    QString leftTimeText;
    QString summaryText;
    int types;

    QVector<QPoint> linePath;
    QVector<QPair<int,int>> toBeRemovedBox;
    bool removeTimerOn;

    QTimer * removeBoxTimer;
    QTimer * remainTimeTimer;
    QTimer * flashTimeTimer;
    QTimer * hintTimeTimer;
    QTimer * freezeTimeTimer;
    QTimer * dizzyTimeTimer;

    QVector<Gadget *> gadgets;
    int maxGadgetNumber;
    double gadgetSummonPossibility;

    int hintTime;
    bool hintTimerOn;
    QVector<QPair<int,int>>hintedBoxes;

    int flashTime;
    bool flashTimerOn;

    bool dizzyTimerOn;
    bool freezeTimerOn;

    Player * player1;
    Player * player2;
};

