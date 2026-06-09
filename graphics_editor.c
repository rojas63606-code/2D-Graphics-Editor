#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_ROWS 20
#define MAX_COLS 60
#define MAX_OBJECTS 100

typedef enum { TYPE_RECTANGLE, TYPE_LINE, TYPE_CIRCLE, TYPE_TRIANGLE } ShapeType;

typedef struct {
    ShapeType type;
    int x1, y1, x2, y2;
    int x3, y3;
    int r;
} Shape;

char canvas[MAX_ROWS][MAX_COLS];
Shape objects[MAX_OBJECTS];
int object_count = 0;

void init_canvas(void) {
    for (int i = 0; i < MAX_ROWS; i++)
        for (int j = 0; j < MAX_COLS; j++)
            canvas[i][j] = '_';
}

void display_canvas(void) {
    for (int i = 0; i < MAX_ROWS; i++) {
        for (int j = 0; j < MAX_COLS; j++)
            putchar(canvas[i][j]);
        putchar('\n');
    }
}

void plot(int x, int y) {
    if (x >= 0 && x < MAX_COLS && y >= 0 && y < MAX_ROWS)
        canvas[y][x] = '*';
}

void draw_line(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = x1 < x2 ? 1 : -1;
    int sy = y1 < y2 ? 1 : -1;
    int err = dx - dy;

    while (1) {
        plot(x1, y1);
        if (x1 == x2 && y1 == y2) break;
        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
    }
}

void draw_rectangle(int x1, int y1, int x2, int y2) {
    int left   = x1 < x2 ? x1 : x2;
    int right  = x1 < x2 ? x2 : x1;
    int top    = y1 < y2 ? y1 : y2;
    int bottom = y1 < y2 ? y2 : y1;

    for (int x = left; x <= right; x++) {
        plot(x, top);
        plot(x, bottom);
    }
    for (int y = top; y <= bottom; y++) {
        plot(left,  y);
        plot(right, y);
    }
}

void draw_circle(int cx, int cy, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    while (y >= x) {
        plot(cx + x, cy + y); plot(cx - x, cy + y);
        plot(cx + x, cy - y); plot(cx - x, cy - y);
        plot(cx + y, cy + x); plot(cx - y, cy + x);
        plot(cx + y, cy - x); plot(cx - y, cy - x);
        if (d < 0) d = d + 4 * x + 6;
        else { d = d + 4 * (x - y) + 10; y--; }
        x++;
    }
}

/* FIX: now uses all three points correctly */
void draw_triangle(int x1, int y1, int x2, int y2, int x3, int y3) {
    draw_line(x1, y1, x2, y2);
    draw_line(x2, y2, x3, y3);
    draw_line(x3, y3, x1, y1);
}

void redraw_canvas(void) {
    init_canvas();
    for (int i = 0; i < object_count; i++) {
        Shape *s = &objects[i];
        switch (s->type) {
            case TYPE_RECTANGLE:
                draw_rectangle(s->x1, s->y1, s->x2, s->y2);
                break;
            case TYPE_LINE:
                draw_line(s->x1, s->y1, s->x2, s->y2);
                break;
            case TYPE_CIRCLE:
                draw_circle(s->x1, s->y1, s->r);
                break;
            case TYPE_TRIANGLE:
                /* FIX: pass s->x3, s->y3 instead of s->x2, s->y1 */
                draw_triangle(s->x1, s->y1, s->x2, s->y2, s->x3, s->y3);
                break;
        }
    }
}

void add_rectangle(void) {
    if (object_count >= MAX_OBJECTS) { printf("Max objects reached.\n"); return; }
    Shape s = {0};
    s.type = TYPE_RECTANGLE;
    printf("Enter x1 y1 x2 y2: ");
    scanf("%d %d %d %d", &s.x1, &s.y1, &s.x2, &s.y2);
    objects[object_count++] = s;
    redraw_canvas();
}

void add_line(void) {
    if (object_count >= MAX_OBJECTS) { printf("Max objects reached.\n"); return; }
    Shape s = {0};
    s.type = TYPE_LINE;
    printf("Enter x1 y1 x2 y2: ");
    scanf("%d %d %d %d", &s.x1, &s.y1, &s.x2, &s.y2);
    objects[object_count++] = s;
    redraw_canvas();
}

void add_circle(void) {
    if (object_count >= MAX_OBJECTS) { printf("Max objects reached.\n"); return; }
    Shape s = {0};
    s.type = TYPE_CIRCLE;
    printf("Enter center x y and radius: ");
    scanf("%d %d %d", &s.x1, &s.y1, &s.r);
    objects[object_count++] = s;
    redraw_canvas();
}

void add_triangle(void) {
    if (object_count >= MAX_OBJECTS) { printf("Max objects reached.\n"); return; }
    Shape s = {0};
    s.type = TYPE_TRIANGLE;
    printf("Enter x1 y1 x2 y2 x3 y3: ");
    /* FIX: read into x3, y3 instead of r twice */
    scanf("%d %d %d %d %d %d", &s.x1, &s.y1, &s.x2, &s.y2, &s.x3, &s.y3);
    objects[object_count++] = s;
    redraw_canvas();
}

void delete_object(void) {
    if (object_count == 0) { printf("No objects to delete.\n"); return; }

    printf("Objects:\n");
    for (int i = 0; i < object_count; i++) {
        printf("%d: ", i + 1);
        switch (objects[i].type) {
            case TYPE_RECTANGLE:
                printf("Rectangle (%d,%d)-(%d,%d)\n",
                       objects[i].x1, objects[i].y1,
                       objects[i].x2, objects[i].y2);
                break;
            case TYPE_LINE:
                printf("Line (%d,%d)-(%d,%d)\n",
                       objects[i].x1, objects[i].y1,
                       objects[i].x2, objects[i].y2);
                break;
            case TYPE_CIRCLE:
                printf("Circle center (%d,%d) r=%d\n",
                       objects[i].x1, objects[i].y1, objects[i].r);
                break;
            case TYPE_TRIANGLE:
                printf("Triangle (%d,%d), (%d,%d), (%d,%d)\n",
                       objects[i].x1, objects[i].y1,
                       objects[i].x2, objects[i].y2,
                       objects[i].x3, objects[i].y3);
                break;
        }
    }

    int index;
    printf("Enter object number to delete: ");
    scanf("%d", &index);
    if (index < 1 || index > object_count) { printf("Invalid selection.\n"); return; }

    for (int i = index - 1; i < object_count - 1; i++)
        objects[i] = objects[i + 1];
    object_count--;
    redraw_canvas();
}

int main(void) {
    int choice;
    init_canvas();

    do {
        printf("\n2D Graphics Editor\n");
        printf("1. Add rectangle\n");
        printf("2. Add line\n");
        printf("3. Add circle\n");
        printf("4. Add triangle\n");
        printf("5. Delete object\n");
        printf("6. Display picture\n");
        printf("0. Exit\n");
        printf("Choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: add_rectangle(); break;
            case 2: add_line();      break;
            case 3: add_circle();    break;
            case 4: add_triangle();  break;
            case 5: delete_object(); break;
            case 6: display_canvas(); break;
            case 0: break;
            default: printf("Invalid choice.\n"); break;
        }
    } while (choice != 0);

    return 0;
}
