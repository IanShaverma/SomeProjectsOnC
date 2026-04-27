#include <stdio.h>

#define WIDTH 80
#define HEIGHT 25
#define ROCKET_SIZE 3

int playSession(int lcount, int rcount) { return (lcount == 21 || rcount == 21); }

void clearScreen() { printf("\033[H\033[J"); }

void plot(int leftRocket, int rightRocket, int xBall, int yBall, int lcount, int rcount) {
    clearScreen();
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (y == 0 || y == HEIGHT - 1) {
                // plot top and bottom borders
                printf("-");
            } else if (x == xBall && y == yBall) {
                // plot Ball
                printf("*");
            } else if (x == 0 || x == WIDTH - 1 || x == WIDTH / 2) {
                // plot middle line and vertical borders
                printf("|");
            } else if (x == 1 && (leftRocket <= y && y <= leftRocket + ROCKET_SIZE - 1)) {
                // plot left rocket
                printf("[");
            } else if (x == WIDTH - 2 && (rightRocket <= y && y <= rightRocket + ROCKET_SIZE - 1)) {
                // plot right rocket
                printf("]");
            } else if (x == WIDTH / 2 - 4 && y == HEIGHT / 2) {
                // plot left counter
                printf("%d", lcount);
            } else if (x == WIDTH / 2 + 4 && y == HEIGHT / 2) {
                // plot right counter
                printf("%d", rcount);
            } else {
                // plot other space
                printf(" ");
            }
        };
        printf("\n");
    };
    return;
}

int leftTurn(int leftRocket) {
    char c = getchar();
    while (c != 'a' && c != 'A' && c != 'z' && c != 'Z' && c != ' ') {
        scanf("%c", &c);
    };
    if ((c == 'a' || c == 'A') && leftRocket > 1) {
        leftRocket = leftRocket - 1;
    } else if ((c == 'z' || c == 'Z') && leftRocket + 2 < 23) {
        leftRocket = leftRocket + 1;
    }
    return (leftRocket);
}

int rightTurn(int rightRocket) {
    char c = getchar();
    while (c != 'k' && c != 'K' && c != 'm' && c != 'M' && c != ' ') {
        scanf("%c", &c);
    };
    if ((c == 'k' || c == 'K') && rightRocket > 1) {
        rightRocket = rightRocket - 1;
    } else if ((c == 'm' || c == 'M') && rightRocket + 2 < 23) {
        rightRocket = rightRocket + 1;
    }
    return (rightRocket);
}

int moveXBall(int velocityX, int xBall) { return (xBall + velocityX); }
int moveYBall(int velocityY, int yBall) { return (yBall + velocityY); }
int checkBallStatus(int xBall, int yBall, int leftRocket, int rightRocket) {
    if (xBall == 0) {
        return (1);  // touched left zone
    } else if (xBall == WIDTH) {
        return (2);  // touched right zone
    } else if ((xBall == 2 && (leftRocket <= yBall && yBall <= leftRocket + ROCKET_SIZE - 1)) ||
               (xBall == WIDTH - 3 && (rightRocket <= yBall && yBall <= rightRocket + ROCKET_SIZE - 1))) {
        return (3);  // touched rocket
    } else if (yBall == 1 || yBall == HEIGHT - 2) {
        return (4);  // touched top or bottom borders
    } else {
        return (5);  // ball in game (flying)
    };
}

int initialRound(int lcount, int rcount) {
    int leftRocket = HEIGHT / 2 - 1;
    int rightRocket = HEIGHT / 2 - 1;

    int ballStatus = 5;  // 1- touch left zone, 2- touch right zone, 3- touch rocket, 4- touch top or bottom,
                         // 5- ball in game (flying)
    int xBall = WIDTH / 2;
    int yBall = HEIGHT / 2;
    int velocityX = 1;
    int velocityY = 1;

    while (ballStatus != 1 && ballStatus != 2) {
        plot(leftRocket, rightRocket, xBall, yBall, lcount, rcount);
        leftRocket = leftTurn(leftRocket);
        rightRocket = rightTurn(rightRocket);
        xBall = moveXBall(velocityX, xBall);
        yBall = moveYBall(velocityY, yBall);
        ballStatus = checkBallStatus(xBall, yBall, leftRocket, rightRocket);

        if (ballStatus == 1) {
            return (1);  // rcount +1
        } else if (ballStatus == 2) {
            return (2);  // lcount +1
        } else if (ballStatus == 3) {
            velocityX = -velocityX;
            velocityY = -velocityY;
        } else if (ballStatus == 4) {
            velocityY = -velocityY;
        }
    };
}

int main() {
    int lcount = 0;
    int rcount = 0;

    while (playSession(lcount, rcount) != 1) {
        int goal = initialRound(lcount, rcount);
        if (goal == 1) {
            rcount = rcount + 1;
        } else if (goal == 2) {
            lcount = lcount + 1;
        };
    };

    clearScreen();
    int leftRocket = HEIGHT / 2 - 1;
    int rightRocket = HEIGHT / 2 - 1;
    int xBall = WIDTH / 2;
    int yBall = HEIGHT / 2;
    plot(leftRocket, rightRocket, xBall, yBall, lcount, rcount);

    printf("Game finished! \n");
    printf("Left side score: %d\n", lcount);
    printf("Right side score: %d\n", rcount);

    return 0;
}
