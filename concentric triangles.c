
#include <stdio.h>
#include <conio.h>
#include <graphics.h>

int main()
{
    int gd = DETECT, gm;
    int i;

    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");

    for(i = 0; i < 5; i++)
    {
        line(300-i*20, 100+i*20,
             200+i*20, 250-i*20);

        line(200+i*20, 250-i*20,
             400-i*20, 250-i*20);

        line(400-i*20, 250-i*20,
             300-i*20, 100+i*20);
    }

    getch();
    closegraph();

    return 0;
}
