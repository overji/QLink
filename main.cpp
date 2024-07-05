#include <QApplication>
#include "LinkGame.h"
#include "MainPage.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    MainPage mainPage;
    mainPage.show();
//    LinkGame game(6,6,0);
//    game.show();

    return QApplication::exec();
}
