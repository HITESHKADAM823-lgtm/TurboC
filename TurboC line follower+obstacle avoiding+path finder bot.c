/*====================================================================
  LINE FOLLOWER ROBOT - PATH SELECTION MINI PROJECT
  Language : C  (Turbo C++ 3.0, BGI graphics)

  Three lines start from a junction:
      PATH 1 (upper)  - OBSTACLE (red block) on the line
      PATH 2 (middle) - line ENDS (dead end)
      PATH 3 (lower)  - clear, leads to the FINISH

  The robot tries the paths one by one.  When it meets an obstacle or
  a dead end it turns back, returns to the junction and tries the next
  path.  When it touches the FINISH it signals "PATH COMPLETED".

  HOW TO RUN:
    1. Options > Linker > Libraries > tick "Graphics Library"
    2. Check the BGI folder in initgraph() (default C:\TURBOC3\BGI)
    3. Compile and run (Ctrl+F9).  Press ESC any time to quit.
====================================================================*/

#include <graphics.h>
#include <conio.h>
#include <dos.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define PI         3.14159265
#define NPATH      3
#define MAXP       6
#define MAXTRAIL   400
#define STEP       3           /* pixels moved per frame          */
#define OBS_RANGE  26          /* ultrasonic stop distance (px)   */
#define JX         160         /* junction                        */
#define JY         240
#define FX         585         /* finish                          */
#define FY         240
#define GOOD_PATH  2           /* index of path that reaches finish */

/* way-points of every path (first point = junction) */
int px[NPATH][MAXP] = {
    { 160, 240, 440,   0,   0 },       /* path 1 : upper  */
    { 160, 400,   0,   0,   0 },       /* path 2 : middle */
    { 160, 240, 440, 520, 585 }        /* path 3 : lower  */
};
int py[NPATH][MAXP] = {
    { 240, 100, 100,   0,   0 },
    { 240, 240,   0,   0,   0 },
    { 240, 380, 380, 240, 240 }
};
int np[NPATH]      = { 3, 2, 5 };
int has_obs[NPATH] = { 1, 0, 0 };
int obx = 340, oby = 100;              /* obstacle on path 1 */

double rx = 40, ry = 240;              /* robot position     */
double ang = 0;                        /* robot heading (rad)*/
int trail_x[MAXTRAIL], trail_y[MAXTRAIL], nt = 0;

/*------------------------------------------------------------------*/
double dist(double x1, double y1, double x2, double y2)
{
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

double norm(double a)
{
    while (a > PI)   a -= 2 * PI;
    while (a < -PI)  a += 2 * PI;
    return a;
}

void quit_check(void)
{
    if (kbhit()) {
        if (getch() == 27) {
            closegraph();
            exit(0);
        }
    }
}

/*------------------------------------------------------------------*/
void status(char *l1, char *l2)
{
    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 421, 639, 462);
    setcolor(LIGHTGREEN);
    outtextxy(10, 428, l1);
    setcolor(YELLOW);
    outtextxy(10, 444, l2);
}

void sensor_panel(int ir, int d)
{
    char buf[80];

    setfillstyle(SOLID_FILL, BLACK);
    bar(0, 464, 639, 479);
    setcolor(LIGHTCYAN);
    if (d >= 0)
        sprintf(buf, "IR sensors L-C-R: %s   Obstacle distance: %d px",
                ir ? "0 1 0" : "0 0 0", d);
    else
        sprintf(buf, "IR sensors L-C-R: %s   Obstacle distance: clear",
                ir ? "0 1 0" : "0 0 0");
    outtextxy(10, 466, buf);
}

/*------------------------------------------------------------------*/
void thick_line(int x1, int y1, int x2, int y2)
{
    setcolor(WHITE);
    setlinestyle(SOLID_LINE, 0, THICK_WIDTH);
    line(x1, y1, x2, y2);
    setlinestyle(SOLID_LINE, 0, NORM_WIDTH);
}

void draw_map(void)
{
    int p, i;

    cleardevice();

    thick_line(40, 240, JX, JY);                 /* main line */
    for (p = 0; p < NPATH; p++)
        for (i = 0; i < np[p] - 1; i++)
            thick_line(px[p][i], py[p][i], px[p][i + 1], py[p][i + 1]);

    /* obstacle on path 1 */
    setfillstyle(SOLID_FILL, RED);
    bar(obx - 10, oby - 10, obx + 10, oby + 10);

    /* finish box */
    setfillstyle(SOLID_FILL, GREEN);
    bar(FX, FY - 14, FX + 30, FY + 14);

    setcolor(LIGHTCYAN);
    outtextxy(150, 8, "LINE FOLLOWER ROBOT - PATH SELECTION");
    setcolor(YELLOW);
    outtextxy(10, 252, "START");
    outtextxy(130, 252, "JUNCTION");
    outtextxy(275, 76, "PATH 1 (upper)");
    outtextxy(300, 116, "OBSTACLE");
    outtextxy(250, 254, "PATH 2 (middle)");
    outtextxy(375, 252, "DEAD END");
    outtextxy(275, 392, "PATH 3 (lower)");
    outtextxy(568, 260, "FINISH");

    setcolor(DARKGRAY);
    line(0, 418, 639, 418);
}

/*------------------------------------------------------------------*/
/* robot is drawn with XOR : drawing it twice erases it */
void draw_robot(void)
{
    int x = (int)rx, y = (int)ry;

    setwritemode(XOR_PUT);
    setcolor(LIGHTGREEN);
    circle(x, y, 7);
    line(x, y, (int)(rx + 14 * cos(ang)), (int)(ry + 14 * sin(ang)));
    setwritemode(COPY_PUT);
}

void place_robot(double x, double y)
{
    draw_robot();                  /* erase old */
    rx = x;
    ry = y;
    draw_robot();                  /* draw new  */
    delay(15);
    quit_check();
}

void turn_to(double target)
{
    double diff;

    for (;;) {
        diff = norm(target - ang);
        if (fabs(diff) < 0.11) break;
        draw_robot();
        ang += (diff > 0) ? 0.10 : -0.10;
        draw_robot();
        delay(20);
        quit_check();
    }
    draw_robot();
    ang = target;
    draw_robot();
}

/*------------------------------------------------------------------*/
/* walk straight from (x0,y0) to (x1,y1) following the line */
void walk(int x0, int y0, int x1, int y1)
{
    double dx = x1 - x0, dy = y1 - y0, len;
    int s, steps;

    len = sqrt(dx * dx + dy * dy);
    steps = (int)(len / STEP);
    if (steps < 1) steps = 1;

    turn_to(atan2(dy, dx));
    for (s = 1; s <= steps; s++)
        place_robot(x0 + dx * s / steps, y0 + dy * s / steps);
}

/* Run along path p.  Returns 0 = finish, 1 = obstacle, 2 = dead end */
int run_path(int p)
{
    int i, s, steps, x0, y0, x1, y1, ir_count = 0;
    double dx, dy, len, nx, ny, d;

    nt = 0;
    for (i = 0; i < np[p] - 1; i++) {
        x0 = px[p][i];      y0 = py[p][i];
        x1 = px[p][i + 1];  y1 = py[p][i + 1];
        dx = x1 - x0;       dy = y1 - y0;
        len = sqrt(dx * dx + dy * dy);
        steps = (int)(len / STEP);
        if (steps < 1) steps = 1;

        turn_to(atan2(dy, dx));

        for (s = 1; s <= steps; s++) {
            nx = x0 + dx * s / steps;
            ny = y0 + dy * s / steps;

            if (has_obs[p]) {
                d = dist(nx, ny, obx, oby);
                if (d <= OBS_RANGE) {
                    sensor_panel(1, (int)d);
                    return 1;                    /* obstacle */
                }
                if (ir_count % 5 == 0) sensor_panel(1, (int)d);
            } else {
                if (ir_count % 5 == 0) sensor_panel(1, -1);
            }
            ir_count++;

            place_robot(nx, ny);
            if (nt < MAXTRAIL) {
                trail_x[nt] = (int)nx;
                trail_y[nt] = (int)ny;
                nt++;
            }
        }
    }

    if (px[p][np[p] - 1] == FX && py[p][np[p] - 1] == FY) return 0;
    sensor_panel(0, -1);                         /* line lost */
    return 2;                                    /* dead end  */
}

/* go back to the junction along the recorded trail */
void go_back(void)
{
    int i;

    turn_to(norm(ang + PI));
    for (i = nt - 1; i >= 0; i--) {
        if (i % 3 == 0 || i == 0) {
            ang = atan2(trail_y[i] - ry, trail_x[i] - rx);
        }
        place_robot(trail_x[i], trail_y[i]);
    }
    place_robot(JX, JY);
}

/*------------------------------------------------------------------*/
void finish_signal(void)
{
    int i;

    for (i = 0; i < 6; i++) {
        setfillstyle(SOLID_FILL, (i % 2) ? GREEN : YELLOW);
        bar(FX, FY - 14, FX + 30, FY + 14);
        sound(1000);
        delay(150);
        nosound();
        delay(100);
    }
}

/*------------------------------------------------------------------*/
int main(void)
{
    int gd = DETECT, gm, err, p, result, found = 0;
    char b1[80], b2[80];

    initgraph(&gd, &gm, "C:\\TURBOC3\\BGI");     /* change if needed */
    err = graphresult();
    if (err != grOk) {
        printf("Graphics error: %s\n", grapherrormsg(err));
        printf("Check the BGI path in initgraph().\n");
        getch();
        return 1;
    }

    draw_map();
    draw_robot();
    status("Robot ready at START.", "Press any key to begin (ESC = quit)");
    sensor_panel(1, -1);
    getch();

    status("Following the main line to the junction ...", "");
    walk(40, 240, JX, JY);

    for (p = 0; p < NPATH && !found; p++) {
        sprintf(b1, "Junction reached. Trying PATH %d ...", p + 1);
        sprintf(b2, "Paths rejected so far: %d", p);
        status(b1, b2);
        delay(600);

        turn_to(atan2((double)(py[p][1] - JY), (double)(px[p][1] - JX)));
        result = run_path(p);

        if (result == 0) {
            sprintf(b1, "PATH COMPLETED!  PATH %d is the correct path.",
                    p + 1);
            status(b1, "Robot touched the FINISH.  Press any key.");
            finish_signal();
            found = 1;
        } else {
            if (result == 1)
                sprintf(b1, "OBSTACLE on PATH %d! Turning back.", p + 1);
            else
                sprintf(b1, "PATH %d line ends (dead end)! Turning back.",
                        p + 1);
            sprintf(b2, "Paths rejected so far: %d", p + 1);
            status(b1, b2);
            delay(800);
            go_back();
        }
    }

    if (!found)
        status("No valid path found!", "Press any key.");

    getch();
    closegraph();
    return 0;
}
