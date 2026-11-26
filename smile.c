
#include <stdio.h>
#include <conio.h>
#include <graphics.h>

int main()
{
    int gd = DETECT, gm;

    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    // Draw face
    setcolor(YELLOW);
    circle(300, 200, 100);

    // Draw eyes
    setcolor(WHITE);
    circle(265, 170, 10);
    circle(335, 170, 10);

    // Draw smile
    setcolor(WHITE);
    arc(300, 200, 200, 340, 60);

    getch();
    closegraph();

    return 0;
}
