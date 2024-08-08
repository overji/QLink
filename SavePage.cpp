//
// Created by overji on 2024/8/8.
//

#include "MainPage.h"
#include "SavePage.h"
#include <iostream>
#include <QWidget>
#include <QPalette>


void SavePage::initSavePage()
{
    // 创建一个QPalette对象
    QPalette palette;

    // 设置QPalette的背景颜色为白色
    palette.setColor(QPalette::Window, Qt::white);

    // 使用QPalette设置SavePage的背景颜色
    this->setAutoFillBackground(true);
    this->setPalette(palette);
    this->layout = new QGridLayout;
    this->Title = new QLabel("存档");
    this->Title->setStyleSheet("font-size:50px;");
    this->Title->setAlignment(Qt::AlignCenter);
    this->scrollArea = new QScrollArea;
    this->newSave = new QPushButton("新建存档");
    this->returnButton = new QPushButton("返回");
    this->quickSave = new QPushButton("快速存档");
    this->quickLoad = new QPushButton("快速读档");
    this->verticalLayout = new QVBoxLayout;
    this->scrollArea->setLayout(this->verticalLayout);
    this->layout->addWidget(this->Title,0,0,1,16);
    this->layout->addWidget(this->scrollArea,1,0,8,12);
    this->layout->addWidget(this->returnButton,1,12,2,4);
    this->layout->addWidget(this->newSave,3,12,2,4);
    this->layout->addWidget(this->quickSave,5,12,2,4);
    this->layout->addWidget(this->quickLoad,7,12,2,4);
    initSaveScrollArea();
    connect(this->returnButton,&QPushButton::clicked,this,&SavePage::returnOriginPage);
    connect(this->newSave,&QPushButton::clicked,this,&SavePage::saveGame);
    this->setStyleSheet("font-size:15px;");
    this->setLayout(this->layout);
}

void SavePage::initSaveScrollArea() const
{
    int saveNum = 0;

    if(QFile::exists("save/saveData.rec")){
        QFile file("save/saveData.rec");
        QDataStream in(&file);
        in >> saveNum;
        for(int i = 0;i < saveNum;i++){
            SaveDataWidget tmpSDW("","");
            in >> tmpSDW.path >> tmpSDW.name;
            auto * newSDW = new SaveDataWidget(tmpSDW.path,tmpSDW.name);
            this->verticalLayout->addWidget(newSDW);
        }
        file.flush();
        file.close();
    } else {
        QFile file("save/saveData.rec");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            std::cerr << "Error: " << file.errorString().toStdString() << std::endl;
            return;
        }
        QDataStream out(&file);
        out << saveNum;
        file.flush();
        file.close();
    }
}

void SavePage::saveGame()
{
    int saveNum = 0;
    QFile file("save/saveData.rec");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        std::cerr << "Error: " << file.errorString().toStdString() << std::endl;
        return;
    }
    QFile tmpfile("save/tmp.rec");
    if (!tmpfile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        std::cerr << "Error: " << tmpfile.errorString().toStdString() << std::endl;
        return;
    }
    QDataStream in(&file);
    QDataStream out(&tmpfile);

    in >> saveNum;
    saveNum++;
    SaveSystem::saveGame(game,QString::number(saveNum));
    SaveDataWidget newSaveDataWidget("save/" + QString::number(saveNum) + ".sav",QString::number(saveNum));

    out << saveNum;
    for(int i = 0;i < (saveNum-1);i ++){
        SaveDataWidget tmpSDW("","");
        in >> tmpSDW.path >> tmpSDW.name;
        out << tmpSDW.path << tmpSDW.name;
    }
    out << newSaveDataWidget.path << newSaveDataWidget.name;
    file.close();
    tmpfile.close();
    if(!QFile::copy("save/tmp.rec","save/saveData.rec")){
        std::cerr << "Error: " << "copy file failed" << std::endl;
    }
    QFile::remove("save/tmp.rec");
}

SavePage::SavePage(LinkGame *game)
{
    this->game = game;
    initSavePage();
}

void SavePage::returnOriginPage()
{
    game->showPausePage();
}

SaveDataWidget::SaveDataWidget(QString path, QString name)
{
    this->path = path;
    this->name = name;
    this->textEdit = new QTextEdit;
    this->deleteButton = new QPushButton("D");
    this->setStyleSheet("font-size:20px;");
    this->textEdit->setText(name);
}
