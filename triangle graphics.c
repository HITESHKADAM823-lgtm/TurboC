
#include <graphics.h>
#include <conio.h>

void main()
{
    int gd = DETECT, gm;

    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    line(200, 300, 300, 100);
    line(300, 100, 400, 300);
    line(400, 300, 200, 300);

    getch();
    closegraph();
}
