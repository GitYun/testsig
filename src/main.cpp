#include <iostream>
#include "window_test.h"

using namespace std;

int main(int argc, char** argv)
{
    int w = 400, h = 300;
    Win_Test wt((Fl::w() - w) / 2, (Fl::y() - h) / 2, w, h, "Window Test");

    Fl::lock();
    wt.show(argc, argv);

    Fl::run();
    return 0;
}
