#include "mainwindow.h"
#include <QApplication>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "sculptor.h"
//#include "putbox.h"
//#include "cutbox.h"
//#include "putsphere.h"
#include "cutsphere.h"
//#include "putellipsoid.h"
#include "cutellipsoid.h"
//#include "putvoxel.h"
#include "cutvoxel.h"
#include "figurageometrica.h"
//biblitoteca que permite utilizar o setprecision
//#include <iomanip>
/*
 * float x = 2.4514;
 * cout<<"x="<<setprecison(3)<<x<<endl;
 *
 * SAIDA: x = 2.45
 * UTIL PARA UTILIZAR O SETCOLOR MASHLAB
 * ESTE PROGRAMA ACEITA AS CORES (RGB-A) 1.0 0.0 0.0 1.0
 */
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
