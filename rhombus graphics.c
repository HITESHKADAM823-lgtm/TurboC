
#include <graphics.h>
#include <conio.h>

void main()
{
    int gd = DETECT, gm;

    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    line(300, 100, 400, 200);
    line(400, 200, 300, 300);
    line(300, 300, 200, 200);
    line(200, 200, 300, 100);

    getch();
    closegraph();
}
