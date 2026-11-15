
#include <graphics.h>
#include <conio.h>

void main()
{
    int gd = DETECT, gm;

    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    rectangle(220, 140, 420, 340);

    getch();
    closegraph();
}
