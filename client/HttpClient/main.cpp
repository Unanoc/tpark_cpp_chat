//
// Created by vadim on 12.04.18.
//
#include <client.h>
#include <QApplication>
#include <iostream>

int main(int argc, char* argv[]){
    QApplication app(argc, argv);

    Client *mw=Client::GetInstance("login","password");
    mw->Print();
    std::cout<<"asd"<<std::endl;
    return app.exec();

}