#define _CRT_NONSTDC_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
#include <mmsystem.h>//음원관련 추가사항

#pragma comment(lib, "winmm.lib") //사운드 부분 추가사항
#define randomize() srand((unsigned)time(NULL))
#define random(n) (rand() % (n))
#define delay(n) Sleep(n)
#define clrscr() system("cls")
#define BOMB '*'
#define gotoxy(x,y) { COORD Cur = {x, y}; \
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur);}
#define showcursor(bShow) { CONSOLE_CURSOR_INFO CurInfo = {20, bShow}; \
   SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo); }
#define putxyfn(x,y,format, value) {gotoxy(x,y);printf(format,value);}//추가

enum { ESC = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };
#define putsxy(x, y, s) {gotoxy(x, y);puts(s);}
#define putxyfn(x,y,format, value) {gotoxy(x,y);printf(format,value);}//추가
#define BX 5
#define BY 1
#define BW 10
#define BH 20


void DrawScreen();
BOOL ProcessKey();
void PrintBrick(BOOL Show);
int GetAround(int x, int y, int b, int r);
BOOL MoveDown();
BOOL TimeStop = FALSE; //시간 정지 상태
void TestFull();
void next_brick(BOOL Show);//다음 블록   
void pre();//블록 미리보기
void AddItem();
void LevelUp();
void StopItem();
void NonItem();
void RemoveTopLine();
void BombItem();
void DrawBomb();

struct Point {
    int x, y;
};

struct Point Shape[][4][4] = {
   { { 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 },{ 0,0,1,0,2,0,-1,0 },{ 0,0,0,1,0,-1,0,-2 } },
   { { 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 },{ 0,0,1,0,0,1,1,1 } },
   { { 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 },{ 0,0,-1,0,0,-1,1,-1 },{ 0,0,0,1,-1,0,-1,-1 } },
   { { 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 },{ 0,0,-1,-1,0,-1,1,0 },{ 0,0,-1,0,-1,1,0,-1 } },
   { { 0,0,-1,0,1,0,-1,-1 },{ 0,0,0,-1,0,1,-1,1 },{ 0,0,-1,0,1,0,1,1 },{ 0,0,0,-1,0,1,1,-1 } },
   { { 0,0,1,0,-1,0,1,-1 },{ 0,0,0,1,0,-1,-1,-1 },{ 0,0,1,0,-1,0,-1,1 },{ 0,0,0,-1,0,1,1,1 } },
   { { 0,0,-1,0,1,0,0,1 },{ 0,0,0,-1,0,1,1,0 },{ 0,0,-1,0,1,0,0,-1 },{ 0,0,-1,0,0,-1,0,1 } },
};

typedef struct ItemNode {
    char item;
    int count;
    struct ItemNode* next;
} ItemNode;

ItemNode* ItemHead = NULL;

enum { EMPTY, BRICK, WALL, CLEAN };
char arTile[3][4] = { ". ","■","□" };
int board[BW + 2][BH + 2];
int nx, ny;
int brick, rot;
int n_brick, n_rot;
int score = 0;
int level = 1;

BOOL AskUserForMusic()
{
    putsxy(30, 10, "배경음악을 재생할까요? (Y/N)");
    while (TRUE) {
        if (_kbhit()) {
            int ch = _getch();
            if (ch == 'Y' || ch == 'y') {
                return TRUE;

            }
            else if (ch == 'N' || ch == 'n') {
                return FALSE;
            }
        }
        delay(100);
    }
}

//void PlayBackgroundMusic()
//{
//    PlaySound(NULL, NULL, 0);
//    PlaySound(TEXT(R"(C:\Users\geniu\Desktop\sound_Asset\BGM.wav)"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
//}

int main()
{

    int nFrame, nStay;
    int x, y;
    /*if (AskUserForMusic()) {
        PlayBackgroundMusic();
    }*/

    showcursor(FALSE);
    randomize();
    clrscr();

    // 가장자리는 벽, 나머지는 빈 공간으로 초기화한다.
    for (x = 0; x < BW + 2; x++) {
        for (y = 0; y < BH + 2; y++) {
            if (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) {
                board[x][y] = WALL;
            }
            else {
                board[x][y] = EMPTY;
            }
        }
    }
    DrawScreen();
    nFrame = 20;

    // 전체 게임 루프
    for (; 1;) {
        gotoxy(50, 9); printf("LV : %d", level);
        brick = random(sizeof(Shape) / sizeof(Shape[0])); //벽돌 하나 출력
        nx = BW / 2;
        ny = 3;
        rot = 0;
        n_rot = 0;
        brick = n_brick;
        n_brick = rand() % (sizeof(Shape) / sizeof(Shape[0]));
        PrintBrick(TRUE);
        pre();
        next_brick(TRUE);

        if (GetAround(nx, ny, brick, rot) != EMPTY) break; // 빈칸이 없으면 끝

        // 벽돌 하나가 바닥에 닿을 때까지의 루프
        nStay = nFrame;
        for (; 2;) {
            if (--nStay == 0) {
                nStay = nFrame;
                if (MoveDown()) break;
            }
            if (ProcessKey()) break;
            delay(1000 / 20); // 벽돌이 조금씩 내려가도록 하기 위해
        }

    }
    clrscr();
    putsxy(30, 12, "G A M E  O V E R");
    //PlaySound(TEXT(R"(C:\Users\geniu\Desktop\sound_Asset\negative_beeps.wav)"), NULL, SND_FILENAME); //종료 사운드 부분 추가
    //PlaySound(NULL, NULL, 0);
    putxyfn(30, 15, "Best score: %d \n", score);
    showcursor(TRUE);
}

void ShowAndDropItem(int x, int y, int item)
{
    for (int i = 0; i < 4; i++) {
        gotoxy(BX + (x + Shape[item][0][i].x) * 2, BY + y + Shape[item][0][i].y);
        puts(arTile[BRICK]);
        delay(100);  // 블록이 떨어지는 속도를 조절하기 위한 딜레이
    }
}

void DrawScreen()
{
    for (int x = 0; x < BW + 2; x++) {
        for (int y = 0; y < BH + 2; y++) {
            putsxy(BX + x * 2, BY + y, arTile[board[x][y]]);
        }
    }

    putsxy(50, 3, "Tetris Ver 1.0");
    putsxy(50, 5, "좌우:이동, 위:회전, 아래:내림");
    putsxy(50, 6, "공백:전부 내림");
    gotoxy(30, 12);
    printf("+(시간정지): %d", ItemCnt());
    gotoxy(30, 13);
    printf("-(한줄 지우기): %d", ItemCnt());
    gotoxy(30, 14);
    printf("/(블록 가리기): %d", ItemCnt());
    gotoxy(30, 15);
    printf("*(폭탄): %d", ItemCnt());
    putxyfn(50, 8, "Score: %d", score);

}

BOOL ProcessKey()
{
    if (kbhit()) {
        int ch = getch();
        if (ch == 0xE0 || ch == 0) {
            ch = getch();
            switch (ch) {
            case LEFT:
                if (GetAround(nx - 1, ny, brick, rot) == EMPTY) {
                    PrintBrick(FALSE);
                    nx--;
                    PrintBrick(TRUE);
                }
                break;
            case RIGHT:
                if (GetAround(nx + 1, ny, brick, rot) == EMPTY) {
                    PrintBrick(FALSE);
                    nx++;
                    PrintBrick(TRUE);
                }
                break;
            case UP:
                if (GetAround(nx, ny, brick, (rot + 1) % 4) == EMPTY) {
                    PrintBrick(FALSE);
                    rot = (rot + 1) % 4;
                    PrintBrick(TRUE);
                }
                break;
            case DOWN:
                if (MoveDown()) {
                    return TRUE;
                }
                break;

            }

        }
        else {
            switch (ch) {
            case '/':
                NonItem();
                break;
            case ' ':
                while (MoveDown() == FALSE) { ; }
                return TRUE;
            case '+':
                NonItem();
                break;
            case '-':
                RemoveTopLine();
                break;
                /*case '*':
                    BombItem();
                    break;*/
            case ESC:
                exit(0);
            }
        }
    }
    return FALSE;
}

void PrintBrick(BOOL Show)
{
    for (int i = 0; i < 4; i++) {
        gotoxy(BX + (Shape[brick][rot][i].x + nx) * 2, BY + Shape[brick][rot][i].y + ny);
        puts(arTile[Show ? BRICK : EMPTY]);
    }
}

void next_brick(BOOL Show) {
    int i;

    for (i = 0; i < 4; i++) {
        gotoxy(28 + (Shape[n_brick][n_rot][i].x + nx) * 2, 3 + Shape[n_brick][n_rot][i].y + ny);
        puts(arTile[Show ? BRICK : CLEAN]);
    }

}

int GetAround(int x, int y, int b, int r)
{
    int k = EMPTY;

    // 벽돌이 차지한 타일 모양 중 가장 큰 값을 찾는다.
    for (int i = 0; i < 4; i++) {
        k = max(k, board[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
    }
    return k;
}

BOOL MoveDown()
{
    // 바닥에 닿았으면 가득찼는지 점검하고 TRUE를 리턴한다.
    if (GetAround(nx, ny + 1, brick, rot) != EMPTY) {
        TestFull();
        //PlaySound(TEXT(R"(C:\Users\geniu\Desktop\sound_Asset\ping.wav)"), NULL, SND_FILENAME | SND_ASYNC); //충돌 사운드 관련 추가
        return TRUE;
    }

    // 아직 공중에 떠 있으면 한칸 아래로 내린다.
    PrintBrick(FALSE);
    ny++;
    PrintBrick(TRUE);
    return FALSE;
}

void TestFull()
{
    // 바닥에 내려앉은 벽돌 기록
    for (int i = 0; i < 4; i++) {
        board[nx + Shape[brick][rot][i].x][ny + Shape[brick][rot][i].y] = BRICK;
    }

    // 수평으로 가득찬 벽돌 제거
    for (int y = 1; y < BH + 1; y++) {
        BOOL bFull = TRUE;
        for (int x = 1; x < BW + 1; x++) {
            if (board[x][y] == EMPTY) {
                bFull = FALSE;
                break;
            }
        }
        // 한줄이 가득 찼으면 이 줄을 제거한다.
        if (bFull) {
            for (int ty = y; ty > 1; ty--) {
                for (int x = 1; x < BW + 1; x++) {
                    board[x][ty] = board[x][ty - 1];
                }
            }
            score += 100;
            if (score % 100 == 0) {
                LevelUp();
            }
            DrawScreen();
            delay(200);
        }


    }
}

void pre() {

    gotoxy(35, 3); puts("다음블록\n");
    gotoxy(35, 4); puts("              \n");
    gotoxy(35, 5); puts("              \n");
    gotoxy(35, 6); puts("              \n");
    gotoxy(35, 7); puts("              \n");
    gotoxy(35, 8); puts("              \n");
    gotoxy(35, 9); puts("              \n");
    gotoxy(35, 10); puts("              \n");
    gotoxy(35, 11); puts("              \n");

}


void AddItem(char item, int count) {
    // 기존 아이템 확인
    ItemNode* current = ItemHead;
    while (current != NULL) {
        if (current->item == item) {
            // 이미 추가된 아이템이면 count만 증가시키고 종료
            current->count += count;
            return;
        }
        current = current->next;
    }

    // 새로운 아이템 추가
    ItemNode* newItem = (ItemNode*)malloc(sizeof(ItemNode));
    if (newItem == NULL) {
        fprintf(stderr, "메모리 할당에 실패했습니다.\n");
        return;
    }

    newItem->item = item;
    newItem->count = count;
    newItem->next = NULL;  // 새로운 노드의 다음 노드는 초기에는 NULL로 설정합니다.

    // 연결 리스트에 노드 추가
    if (ItemHead == NULL) {
        ItemHead = newItem;
    }
    else {
        current = ItemHead;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = newItem;
    }
}

void LevelUp() {

    level++;
    DrawScreen();
    delay(100);

    int RandomItem = rand() % 4;

    switch (RandomItem) {
    case 0:
        AddItem('+', 1);
        break;
    case 1:
        AddItem('/', 1);
        break;
    case 2:
        AddItem('-', 1);
        break;
    case 3:
        AddItem('*', 1);
        break;
    }
}

void RemoveUsedItem(char item) {

    ItemNode* current = ItemHead;
    ItemNode* prev = NULL;

    // 아이템 리스트를 순회하며 사용한 아이템 찾기
    while (current != NULL) {
        if (current->item == item) {
            // 찾았을 경우 해당 아이템 노드를 삭제하고 메모리 해제
            if (prev != NULL) {
                prev->next = current->next;
            }
            else {
                ItemHead = current->next;
            }
            free(current);
            break;
        }

        // 다음 노드로 이동
        prev = current;
        current = current->next;
    }
}

void StopItem() {

    if (ItemHead != NULL && ItemHead->item == '+') {
        TimeStop = TRUE;
        ItemNode* temp = ItemHead;
        ItemHead = ItemHead->next;
        free(temp);
    }

    ItemCnt();
}

int ItemCnt() {

    int itemcnt = 0;
    ItemNode* current = ItemHead;

    while (current != NULL) {
        itemcnt += current->count;
        current = current->next;
    }

    return itemcnt;
}

void NonItem() {
    // 아이템이 아직 증정되지 않았으면 사용할 수 없음
    if (ItemHead == NULL || (ItemHead->item != '+' && ItemHead->item != '/')) {
        return;
    }
    if (ItemHead->item == '+') {
        TimeStop = TRUE;
        DrawScreen();
        delay(5000);
        TimeStop = FALSE;
        RemoveUsedItem('+');
    }
    else if (ItemHead->item == '/') {
        pre(FALSE);
        delay(5000);
        pre(TRUE);
        RemoveUsedItem('/');
    }
    DrawScreen();
    ItemCnt();
}

void RemoveTopLine() {
    //// 만약 현재 아이템이 '-'가 아니거나, 아이템이 아직 증정되지 않았다면 함수 실행을 중단합니다.
    //if (ItemHead == NULL || ItemHead->item != '-') {
    //    return;
    //}

    // 보드에서 가장 아래에 있는 줄을 삭제합니다.
    for (int y = BH; y > 0; y--) {
        for (int x = 1; x < BW + 1; x++) {
            // 각 칸을 한 칸씩 아래로 이동시킵니다.
            board[x][y] = board[x][y - 1];
        }
    }

    // 보드의 맨 위에 빈 줄을 추가합니다.
    for (int x = 1; x < BW + 1; x++) {
        board[x][1] = EMPTY;
    }

    // 사용된 아이템을 제거합니다.
    RemoveUsedItem('-');

    // 화면을 다시 그립니다.
    DrawScreen();
}

void BombItem() {
    int bombRange = 1;

    // 폭발 범위 내의 블록을 제거
    for (int i = -bombRange; i <= bombRange; i++) {
        for (int j = -bombRange; j <= bombRange; j++) {
            int targetX = nx + i;
            int targetY = ny + j;

            // 범위를 벗어나지 않도록 체크
            if (targetX >= 1 && targetX <= BW && targetY >= 1 && targetY <= BH) {
                // 폭탄 모양 출력
                DrawBomb(targetX, targetY);
                // 일정 시간 동안 딜레이
                delay(100);
                // 폭탄이 터진 위치를 다시 보드로 설정
                board[targetX][targetY] = EMPTY;
            }
        }
    }

    // 사용된 아이템을 제거
    RemoveUsedItem('*');

    // 화면을 다시 그림
    DrawScreen();

    // 새로운 블록을 놓음
    nx = BW / 2;
    ny = 3;
    rot = 0;
    next_brick(TRUE);

    // 이어서 게임 루프를 진행하거나 다른 처리를 수행
}

void DrawBomb(int x, int y) {
    // 폭탄 모양 출력
    putsxy(BX + x * 2, BY + y, "╔═╗");
    putsxy(BX + x * 2, BY + y + 1, "║*║");
    putsxy(BX + x * 2, BY + y + 2, "╚═╝");

    // 게임 보드에서 해당 위치를 EMPTY로 설정
    board[x][y] = EMPTY;
    board[x][y + 1] = EMPTY;
    board[x][y + 2] = EMPTY;
}
