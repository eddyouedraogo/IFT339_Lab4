//
//  main.cpp
//  WBT
//
//  Created by Goulet on 14-11-05.
//  Copyright (c) 2014 UdeS. All rights reserved.
//

#include <iostream>
#include "WBT.h"
using namespace std;

int main(int argc, const char * argv[])
{
    WBT<double> WD;
    WD.afficher();

    WD.insert(1);
    WD.insert(2);
    WD.insert(3);
    WD.insert(4);
    WD.insert(5);
    WD.insert(6);

//    WD.insert(23);
//    WD.insert(12);
//    WD.insert(35);
//    WD.insert(12);
//    WD.afficher();
//    for(double d=5;d<85;d+=7) {
//            WD.insert(d);
//            WD.afficher();
//    }

    WD.afficher();
    WBT<double> W2(WD);
    WBT<double> W3;
    WD=W3;
    WD.afficher();
    W3=W2;
    W3.afficher();
    std::cout << "Fin du programme!\n";

    return 0;
}

