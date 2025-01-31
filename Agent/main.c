#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char board[10] = ".........";

void cin(char *input) {
    int ln = 0;
    while (1) {
        char c = getchar();
        if (c == '\n') {
            input[ln] = '\0';
            break;
        }
        if (ln < 200 - 1) {
            input[ln++] = c;
        }
    }
}

int find(char *input) {
    for (int i = 0; i < strlen(input); i++) {
        if (input[i] >= '0' && input[i] <= '8') {
            return input[i] - '0';
        }
    }
    return -1;
}

void reply(const char *response) {
    printf("\033[1;31m""AI: ""\033[0m");
    fflush(stdout);
    for (int i = 0; i < strlen(response); i++) {
        printf("%c", response[i]);
        fflush(stdout);
        usleep(50000);
    }
    printf("\n");
}

void focus(const char *app) {
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "wmctrl -a '%s'", app);
    system(cmd);
    sleep(1);
}

void dir(int n, int m, int i, char *str) {
    int dl = 2 - n / 3;
    int dr = m / 3;
    int b = 6 + n % 3;
    int r = (b == 6 || b == 7) ? 2 : 3;
    const char *xr = (board[b] == '.') ? "r" : "rr";

    snprintf(str, 20, "%.*s%s%.*s%.*s",
             dl, "dddddd", xr, r, "rrrrrr", dr, "dddddd");
}

void route(int l, int r, char *str) {
    int j = 0;

    str[j++] = l + '0';

    if (l <= r) {
        for (int i = l + 1; i < r; i++) {
            str[j++] = i + '0';
            str[j++] = i + '0';
        }
    } else {
        for (int i = l + 1; i <= 8; i++) {
            str[j++] = i + '0';
            str[j++] = i + '0';
        }
        for (int i = 0; i < r; i++) {
            str[j++] = i + '0';
            str[j++] = i + '0';
        }
    }

    str[j++] = r + '0';
    str[j] = '\0';
}

void place(int my, int ai) {
    focus("fx-991ES PLUS C Emulator");
    if (my > 5) system("xdotool key Left");
    char r[200];
    route(my, ai, r);
    for (int i = 0; i < strlen(r); i+=2) {
        char d[20];
        dir(r[i] - '0', r[i + 1] - '0', my, d);
        for (int j = 0; j < strlen(d); j++) {
            if (d[j] == 'r') {
                system("xdotool key Right");
            } else {
                system("xdotool key Down");
            }
            usleep(50);
        }
    }
    system("xdotool key KP_Multiply");
}

void processInput(char *input) {
    for (int i = 0; i < 9; i++) {
        if (i == find(input)) { 
            board[i] = 'X';

            char cmd[256];
            snprintf(cmd, sizeof(cmd), "python3 minimax.py \"%s\"", board);
            FILE *fp = popen(cmd, "r");
            if (!fp) {
                reply("Failed to execute Python script.");
                return;
            }

            char ai_move[20];
            int move;
            fgets(ai_move, sizeof(ai_move), fp);
            sscanf(ai_move, "%s %d", board, &move);
            pclose(fp);

            char response[50];
            snprintf(response, sizeof(response), "I'll place my X at %d\n", move);
            reply(response);

            place(i, move);
            board[move] = 'O';
            return;
        }
    }
    reply("I didn't understand your position!\n");
}

int main() {
    char input[200];

    printf("\033[H\033[2J");
    while (1) {
        printf("\033[1;32m""a7md: ""\033[0m");
        fflush(stdout);
        cin(input);
        processInput(input);
    }
    return 0;
}
