#include <QApplication>
#include "LinkGame.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    LinkGame game(6,6);
    game.show();
//    LinkGame * game = SaveSystem::loadGame();
//    game->show();

    return QApplication::exec();
}
