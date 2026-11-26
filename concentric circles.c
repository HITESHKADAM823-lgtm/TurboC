
#include <stdio.h>
#include <conio.h>
#include <graphics.h>

int main()
{
    int gd = DETECT, gm;
    int x, y, r;

    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    x = getmaxx() / 2;
    y = getmaxy() / 2;

    for (r = 30; r <= 150; r += 30)
    {
        circle(x, y, r);
    }

    getch();
    closegraph();

    return 0;
}
