//
// Created by overji on 2024/6/24.
//

#include "SimpleTest.h"
#include "Player.h"
#include "SelectChecker.h"
#include <QtTest/QtTest>

void SimpleTest::noTwistHorizontalCorrectTest()
{
    LinkGame * linkGame = new LinkGame(6,5);
    Player * player = new Player(0,0,0,0,0,1);
    linkGame->boxMap[0][0]->typeOfBox = 1;
    linkGame->boxMap[0][1]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(0,0));
    player->currentSelected.push_back(QPair<int,int>(0,1));
    SelectChecker::checkSelected(linkGame,player);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[0][0]->boxState.boxRemoved == true);
    QVERIFY(linkGame->boxMap[0][1]->boxState.boxRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::noTwistVerticalCorrectTest()
{
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0);
    linkGame->boxMap[0][0]->typeOfBox = 1;
    linkGame->boxMap[1][0]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(0,0));
    player->currentSelected.push_back(QPair<int,int>(1,0));
    QVERIFY(player->currentSelected.size() == 2);
    SelectChecker::checkSelected(linkGame,player);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[0][0]->boxState.boxRemoved == true);
    QVERIFY(linkGame->boxMap[1][0]->boxState.boxRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::noTwistHorizontalWrongTest()
{
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[1][3]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(1,1));
    player->currentSelected.push_back(QPair<int,int>(1,3));
    SelectChecker::checkSelected(linkGame, player);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxRemoved == false);
    QVERIFY(linkGame->boxMap[1][3]->boxState.boxRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::noTwistVerticalWrongTest()
{
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[3][1]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(1,1));
    player->currentSelected.push_back(QPair<int,int>(3,1));
    SelectChecker::checkSelected(linkGame, player);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxRemoved == false);
    QVERIFY(linkGame->boxMap[3][1]->boxState.boxRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::oneTwistCorrectTest()
{
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[2][2]->typeOfBox = 1;
    linkGame->boxMap[2][1]->boxState.boxRemoved = true;
    player->currentSelected.push_back(QPair<int,int>(1,1));
    player->currentSelected.push_back(QPair<int,int>( 2,2));
    SelectChecker::checkSelected(linkGame, player);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxRemoved == true);
    QVERIFY(linkGame->boxMap[2][2]->boxState.boxRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::oneTwistCorrectTest2()
{
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[2][2]->typeOfBox = 1;
    linkGame->boxMap[1][2]->boxState.boxRemoved = true;
    player->currentSelected.push_back(QPair<int,int>(1,1));
    player->currentSelected.push_back(QPair<int,int>( 2,2));
    SelectChecker::checkSelected(linkGame, player);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxRemoved == true);
    QVERIFY(linkGame->boxMap[2][2]->boxState.boxRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::oneTwistWrongTest()
{
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[2][2]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(1,1));
    player->currentSelected.push_back(QPair<int,int>( 2,2));
    SelectChecker::checkSelected(linkGame, player);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxRemoved == false);
    QVERIFY(linkGame->boxMap[2][2]->boxState.boxRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::oneTwistWithOneLineTest()
{
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[2][3]->typeOfBox = 1;
    linkGame->boxMap[2][1]->boxState.boxRemoved = true;
    linkGame->boxMap[2][2]->boxState.boxRemoved = true;
    player->currentSelected.push_back(QPair<int,int>(1,1));
    player->currentSelected.push_back(QPair<int,int>( 2,3));
    SelectChecker::checkSelected(linkGame, player);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxRemoved == true);
    QVERIFY(linkGame->boxMap[2][3]->boxState.boxRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::oneTwistWithOneLineBlockedTest()
{
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[2][3]->typeOfBox = 1;
    linkGame->boxMap[2][1]->boxState.boxRemoved = true;
    player->currentSelected.push_back(QPair<int,int>(1,1));
    player->currentSelected.push_back(QPair<int,int>( 2,3));
    SelectChecker::checkSelected(linkGame, player);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxRemoved == false);
    QVERIFY(linkGame->boxMap[2][3]->boxState.boxRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::twoTwistCorrectTest()
{
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0);
    linkGame->boxMap[0][0]->typeOfBox = 1;
    linkGame->boxMap[0][2]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(0,0));
    player->currentSelected.push_back(QPair<int,int>(0,2));
    SelectChecker::checkSelected(linkGame,player);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[0][0]->boxState.boxRemoved == true);
    QVERIFY(linkGame->boxMap[0][2]->boxState.boxRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::twoTwistCorrectTest2()
{
    LinkGame * linkGame = new LinkGame(6,5);
    Player * player = new Player(0,0,0,0,0);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[4][3]->typeOfBox = 1;
    linkGame->boxMap[2][1]->boxState.boxRemoved = true;
    linkGame->boxMap[2][2]->boxState.boxRemoved = true;
    linkGame->boxMap[2][3]->boxState.boxRemoved = true;
    linkGame->boxMap[3][3]->boxState.boxRemoved = true;
    player->currentSelected.push_back(QPair<int,int>(1,1));
    player->currentSelected.push_back(QPair<int,int>(4,3));
    SelectChecker::checkSelected(linkGame,player);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxRemoved == true);
    QVERIFY(linkGame->boxMap[4][3]->boxState.boxRemoved == true);
    delete linkGame;
    delete player;
}

void SimpleTest::threeTwistWrongTest1()
{
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0);
    linkGame->boxMap[1][0]->typeOfBox = 1;
    linkGame->boxMap[1][4]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(1,0));
    player->currentSelected.push_back(QPair<int,int>(1,4));
    SelectChecker::checkSelected(linkGame,player);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][0]->boxState.boxRemoved == false);
    QVERIFY(linkGame->boxMap[1][4]->boxState.boxRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::threeTwistWrongTest2()
{
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0);
    linkGame->boxMap[0][1]->typeOfBox = 1;
    linkGame->boxMap[3][2]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(0,1));
    player->currentSelected.push_back(QPair<int,int>(3,2));
    SelectChecker::checkSelected(linkGame,player);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[0][1]->boxState.boxRemoved == false);
    QVERIFY(linkGame->boxMap[3][2]->boxState.boxRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::differentTypeWrongTest()
{
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,1);
    linkGame->boxMap[0][0]->typeOfBox = 1;
    linkGame->boxMap[0][1]->typeOfBox = 2;
    player->currentSelected.push_back(QPair<int,int>(0,0));
    player->currentSelected.push_back(QPair<int,int>(0,1));
    SelectChecker::checkSelected(linkGame,player);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[0][0]->boxState.boxRemoved == false);
    QVERIFY(linkGame->boxMap[0][1]->boxState.boxRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::popSameBoxTest()
{
    LinkGame * linkGame = new LinkGame(4,5);
    Player * player = new Player(0,0,0,0,0,1);
    linkGame->boxMap[0][0]->typeOfBox = 1;
    linkGame->boxMap[0][0]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(0,0));
    player->currentSelected.push_back(QPair<int,int>(0,0));
    SelectChecker::checkSelected(linkGame,player);
    QVERIFY(player->currentSelected.size() == 1);
    QVERIFY(linkGame->boxMap[0][0]->boxState.boxRemoved == false);
    delete linkGame;
    delete player;
}

void SimpleTest::removeSelectedTest()
{
    LinkGame * linkGame = new LinkGame(6,5);
    Player * player = new Player(0,0,0,0,0,1);
    linkGame->boxMap[0][0]->typeOfBox = 1;
    linkGame->boxMap[1][1]->typeOfBox = 1;
    player->currentSelected.push_back(QPair<int,int>(0,0));
    player->currentSelected.push_back(QPair<int,int>(1,1));
    SelectChecker::checkSelected(linkGame,player);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[0][0]->boxState.boxRemoved == false);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxRemoved == false);
    QVERIFY(linkGame->boxMap[0][0]->boxState.boxSelected == false);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxSelected == false);
    delete linkGame;
    delete player;
}

void SimpleTest::reversedBoxSequenceTest()
{
    LinkGame * linkGame = new LinkGame(6,5);
    Player * player = new Player(0,0,0,0,0,1);
    linkGame->boxMap[1][1]->typeOfBox = 1;
    linkGame->boxMap[4][3]->typeOfBox = 1;
    linkGame->boxMap[2][1]->boxState.boxRemoved = true;
    linkGame->boxMap[2][2]->boxState.boxRemoved = true;
    linkGame->boxMap[2][3]->boxState.boxRemoved = true;
    linkGame->boxMap[3][3]->boxState.boxRemoved = true;
    player->currentSelected.push_back(QPair<int,int>(4,3));
    player->currentSelected.push_back(QPair<int,int>(1,1));
    SelectChecker::checkSelected(linkGame,player);
    QVERIFY(player->currentSelected.size() == 0);
    QVERIFY(linkGame->boxMap[1][1]->boxState.boxRemoved == true);
    QVERIFY(linkGame->boxMap[4][3]->boxState.boxRemoved == true);
    delete linkGame;
    delete player;
}
QTEST_MAIN(SimpleTest);
