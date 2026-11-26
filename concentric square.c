
#include <stdio.h>
#include <conio.h>
#include <graphics.h>

int main()
{
    int gd = DETECT, gm;
    int x, y, s;

    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    x = getmaxx() / 2;
    y = getmaxy() / 2;

    for (s = 30; s <= 150; s += 30)
    {
        rectangle(x - s, y - s, x + s, y + s);
    }

    getch();
    closegraph();

    return 0;
}
