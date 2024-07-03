//
// Created by overji on 2024/6/24.
//

#pragma once
#include <QtTest/QtTest>

class SimpleTest :public QObject
{
    Q_OBJECT
private slots:
    void noTwistHorizontalCorrectTest();
    void noTwistVerticalCorrectTest();
    void noTwistHorizontalWrongTest();
    void noTwistVerticalWrongTest();
    void oneTwistCorrectTest();
    void oneTwistCorrectTest2();
    void oneTwistWrongTest();
    void oneTwistWithOneLineTest();
    void oneTwistWithOneLineBlockedTest();
    void twoTwistCorrectTest();
    void twoTwistCorrectTest2();
    void threeTwistWrongTest1();
    void threeTwistWrongTest2();
    void differentTypeWrongTest();
    void popSameBoxTest();
    void removeSelectedTest();
    void reversedBoxSequenceTest();
};

