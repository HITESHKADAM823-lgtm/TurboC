
#include <graphics.h>
#include <conio.h>

void main()
{
    int gd = DETECT, gm;

    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    line(200, 150, 350, 150);
    line(350, 150, 400, 300);
    line(400, 300, 250, 300);
    line(250, 300, 200, 150);

    getch();
    closegraph();
}
