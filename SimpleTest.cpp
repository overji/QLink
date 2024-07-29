//
// Created by overji on 2024/6/24.
//

#include "SimpleTest.h"
#include "Player.h"
#include "SelectChecker.h"
#include <QtTest/QtTest>
#include <QPainter>

void SimpleTest::noTwistHorizontalCorrectTest()
{
    //测试无折线横向无阻挡，应当正确消除方块
    LinkGame * linkGame = new LinkGame(6,5);
    Player * player = new Player(0,0,0,0,0,linkGame,1);
    linkGame->boxMap[0][0]->typeOfBox = 1;
    linkGame->boxMap[0][1]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(0,0));
    player->currentSelected.push_back(QPair<int,int>(0,1));
    SelectChecker::checkSelected(linkGame,player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[0][0]->boxState.boxToBeRemoved == true);
    QVERIFY(linkGame->boxMap[0][1]->boxState.boxToBeRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::noTwistVerticalCorrectTest()
{
    //测试无折线纵向无阻挡，应当正确消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->boxMap[0][0]->typeOfBox = 1;
    linkGame->boxMap[1][0]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(0,0));
    player->currentSelected.push_back(QPair<int,int>(1,0));
    QVERIFY(player->currentSelected.size() == 2);
    SelectChecker::checkSelected(linkGame,player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[0][0]->boxState.boxToBeRemoved == true);
    QVERIFY(linkGame->boxMap[1][0]->boxState.boxToBeRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::noTwistHorizontalWrongTest()
{
    //测试无折线横向有阻挡，应当不消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[1][3]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(1,1));
    player->currentSelected.push_back(QPair<int,int>(1,3));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxToBeRemoved == false);
    QVERIFY(linkGame->boxMap[1][3]->boxState.boxToBeRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::noTwistVerticalWrongTest()
{
    //测试无折线纵向有阻挡，应当不消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[3][1]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(1,1));
    player->currentSelected.push_back(QPair<int,int>(3,1));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxToBeRemoved == false);
    QVERIFY(linkGame->boxMap[3][1]->boxState.boxToBeRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::oneTwistCorrectTest()
{
    //测试一折线正确消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[2][2]->typeOfBox = 1;
    linkGame->boxMap[2][1]->boxState.boxRemoved = true;
    player->currentSelected.push_back(QPair<int,int>(1,1));
    player->currentSelected.push_back(QPair<int,int>( 2,2));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxToBeRemoved == true);
    QVERIFY(linkGame->boxMap[2][2]->boxState.boxToBeRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::oneTwistCorrectTest2()
{
    //测试一折线正确消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[2][2]->typeOfBox = 1;
    linkGame->boxMap[1][2]->boxState.boxRemoved = true;
    player->currentSelected.push_back(QPair<int,int>(1,1));
    player->currentSelected.push_back(QPair<int,int>( 2,2));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxToBeRemoved == true);
    QVERIFY(linkGame->boxMap[2][2]->boxState.boxToBeRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::oneTwistWrongTest()
{
    //测试一折线应当不消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[2][2]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(1,1));
    player->currentSelected.push_back(QPair<int,int>( 2,2));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxToBeRemoved == false);
    QVERIFY(linkGame->boxMap[2][2]->boxState.boxToBeRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::oneTwistWithOneLineTest()
{
    //测试一折线，一条线较长，应当正确消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[2][3]->typeOfBox = 1;
    linkGame->boxMap[2][1]->boxState.boxRemoved = true;
    linkGame->boxMap[2][2]->boxState.boxRemoved = true;
    player->currentSelected.push_back(QPair<int,int>(1,1));
    player->currentSelected.push_back(QPair<int,int>( 2,3));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxToBeRemoved == true);
    QVERIFY(linkGame->boxMap[2][3]->boxState.boxToBeRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::oneTwistWithOneLineBlockedTest()
{
    //测试一折线，一条线较长且存在阻挡，应当不消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[2][3]->typeOfBox = 1;
    linkGame->boxMap[2][1]->boxState.boxRemoved = true;
    player->currentSelected.push_back(QPair<int,int>(1,1));
    player->currentSelected.push_back(QPair<int,int>( 2,3));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxToBeRemoved == false);
    QVERIFY(linkGame->boxMap[2][3]->boxState.boxToBeRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::twoTwistCorrectTest()
{
    //测试两折线正确消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->boxMap[0][0]->typeOfBox = 1;
    linkGame->boxMap[0][2]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(0,0));
    player->currentSelected.push_back(QPair<int,int>(0,2));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[0][0]->boxState.boxToBeRemoved == true);
    QVERIFY(linkGame->boxMap[0][2]->boxState.boxToBeRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::twoTwistCorrectTest2()
{
    //测试两折线正确消除方块
    LinkGame * linkGame = new LinkGame(6,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[4][3]->typeOfBox = 1;
    linkGame->boxMap[2][1]->boxState.boxRemoved = true;
    linkGame->boxMap[2][2]->boxState.boxRemoved = true;
    linkGame->boxMap[2][3]->boxState.boxRemoved = true;
    linkGame->boxMap[3][3]->boxState.boxRemoved = true;
    player->currentSelected.push_back(QPair<int,int>(1,1));
    player->currentSelected.push_back(QPair<int,int>(4,3));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxToBeRemoved == true);
    QVERIFY(linkGame->boxMap[4][3]->boxState.boxToBeRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::threeTwistWrongTest1()
{
    //测试三折线应当不消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->boxMap[1][0]->typeOfBox = 1;
    linkGame->boxMap[1][4]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(1,0));
    player->currentSelected.push_back(QPair<int,int>(1,4));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][0]->boxState.boxToBeRemoved == false);
    QVERIFY(linkGame->boxMap[1][4]->boxState.boxToBeRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::threeTwistWrongTest2()
{
    //测试三折线应当不消除方块
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame);
    linkGame->boxMap[0][1]->typeOfBox = 1;
    linkGame->boxMap[3][2]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(0,1));
    player->currentSelected.push_back(QPair<int,int>(3,2));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[0][1]->boxState.boxToBeRemoved == false);
    QVERIFY(linkGame->boxMap[3][2]->boxState.boxToBeRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::differentTypeWrongTest()
{
    //测试不同类型的方块应当不消除
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame,1);
    linkGame->boxMap[0][0]->typeOfBox = 1;
    linkGame->boxMap[0][1]->typeOfBox = 2;
    player->currentSelected.push_back(QPair<int,int>(0,0));
    player->currentSelected.push_back(QPair<int,int>(0,1));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[0][0]->boxState.boxToBeRemoved == false);
    QVERIFY(linkGame->boxMap[0][1]->boxState.boxToBeRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::popSameBoxTest()
{
    //测试相同方块只选择一次
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,linkGame,1);
    linkGame->boxMap[0][0]->typeOfBox = 1;
    linkGame->boxMap[0][0]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(0,0));
    player->currentSelected.push_back(QPair<int,int>(0,0));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 1);
    QVERIFY(linkGame->boxMap[0][0]->boxState.boxToBeRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::removeSelectedTest()
{
    //测试消除选中的方块
    LinkGame * linkGame = new LinkGame(6,5);
    Player * player = new Player(0,0,0,0,0,linkGame,1);
    linkGame->boxMap[0][0]->typeOfBox = 1;
    linkGame->boxMap[1][1]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(0,0));
    player->currentSelected.push_back(QPair<int,int>(1,1));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[0][0]->boxState.boxToBeRemoved == false);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxToBeRemoved == false);
    QVERIFY(linkGame->boxMap[0][0]->boxState.boxSelected == false);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxSelected == false);
    delete linkGame;
    delete player;
}

void SimpleTest::reversedBoxSequenceTest()
{
    //测试从另一个先后顺序消除方块
    LinkGame * linkGame = new LinkGame(6,5);
    Player * player = new Player(0,0,0,0,0,linkGame,1);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[4][3]->typeOfBox = 1;
    linkGame->boxMap[2][1]->boxState.boxRemoved = true;
    linkGame->boxMap[2][2]->boxState.boxRemoved = true;
    linkGame->boxMap[2][3]->boxState.boxRemoved = true;
    linkGame->boxMap[3][3]->boxState.boxRemoved = true;
    player->currentSelected.push_back(QPair<int,int>(4,3));
    player->currentSelected.push_back(QPair<int,int>(1,1));
    SelectChecker::checkSelected(linkGame, player);
    QPainter painter;
    player->drawLine(painter,linkGame);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxToBeRemoved == true);
    QVERIFY(linkGame->boxMap[4][3]->boxState.boxToBeRemoved == true);
    delete linkGame;
    delete player;
}

QTEST_MAIN(SimpleTest);
