//
// Created by overji on 2024/8/8.
//

#pragma once
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QTextEdit>
#include "MainPage.h"

class MainPage;

class SaveDataWidget:public QPushButton
{
    Q_OBJECT
public:
    explicit SaveDataWidget(QString pathInput, QString nameInput);
    QString path;
    QString name;
    QGridLayout * gridLayout;
    QLabel * nameLabel;
    QPushButton * deleteButton;
};

class SavePage :public QWidget{
    Q_OBJECT
    QGridLayout * layout;
    QLabel * Title;
    QScrollArea * scrollArea;
    QPushButton * newSave;
    QPushButton * returnButton;
    QPushButton * quickSave;
    QPushButton * quickLoad;
    LinkGame * game;
    QVBoxLayout * verticalLayout;
public:
    SavePage(LinkGame * game);
    void initSavePage();
    void initSaveScrollArea() const;
private:
    void setCSS();
public slots:
    void saveGame(); //存档
    void returnOriginPage();
};

class LoadPage :public QWidget{
Q_OBJECT
    QGridLayout * layout;
    QLabel * Title;
    QScrollArea * scrollArea;
    QPushButton * returnButton;
    QPushButton * quickLoad;
    QVBoxLayout * verticalLayout;
    MainPage * mainPage;
public:
    LoadPage(MainPage * mainPage);
    void initLoadPage();
    void initSaveScrollArea() const;
private:
    void setCSS();
public slots:
    void returnOriginPage();
};

