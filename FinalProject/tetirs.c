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
#define gotoxy(x,y) { COORD Cur = {x, y}; \
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur);}
#define showcursor(bShow) { CONSOLE_CURSOR_INFO CurInfo = {20, bShow}; \
   SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo); }
#define putxyfn(x,y,format, value) {gotoxy(x,y);printf(format,value);}//추가

enum { ESC = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };
enum { w = 119, a = 97, s = 115, d = 100 };
#define putsxy(x, y, s) {gotoxy(x, y);puts(s);}
#define putxyfn(x,y,format, value) {gotoxy(x,y);printf(format,value);}//추가
#define BX 5
#define BY 1
#define BW 10
#define BH 20

void DrawScreen(int board[BW + 2][BH + 2], int x, int y);
void DrawScreenForPL2(int board2[BW + 2][BH + 2], int x, int y);
//void DrawScreen();
BOOL ProcessKey();
BOOL ProcessKeyForPL2();
void PrintBrick(BOOL Show);
void PrintBrickOnBoard2(BOOL Show);
int GetAround(int x, int y, int b, int r);
int GetAroundForPL2(int x2, int y2, int b2, int r2);
BOOL MoveDown();
BOOL TimeStop = FALSE; //시간 정지 상태
void TestFull();
void next_brick(BOOL Show);//다음 블록   
void pre();//블록 미리보기
void AddItem();
void LevelUp();
void StopItem();
void UseStopItem();
//void RemoveItem();
BOOL showPreview = TRUE;
BOOL MoveDownOnBoard2();
void TestFullOnBoard2();
int gameMode=0;

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
int nx2, ny2;
int brick, brickForPL2, rot, rotForPL2;
int n_brick, n_rot;
int score = 0;
int level = 1;
int board1[BW + 2][BH + 2];
int board2[BW + 2][BH + 2];


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

void PlayBackgroundMusic()
{
    PlaySound(NULL, NULL, 0);
    PlaySound(TEXT(R"(C:\Users\Arthur\Desktop\sound_Asset\BGM.wav)"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
}

int main()
{
    int nFrame, nStay;
    int nFrame2, nStay2;
    int x, y;
    if (AskUserForMusic()) {
        PlayBackgroundMusic();
    }

    showcursor(FALSE);
    randomize();
    clrscr();

    gotoxy(30, 10); printf("게임모드를 설정해주세요 1 OR 2\n");
    putsxy(30, 11, "1. 싱글플레이");
    putsxy(30, 12, "2. 듀오플레이");
    
    while (true)
    {
        if (_kbhit())
        {
            int ch = _getch();
            if (ch == '1')
            {
                gameMode = 1;
                clrscr();
                break;
            }
            else if (ch == '2')
            {
                gameMode = 2;
                clrscr();
                break;
            }
        }
        delay(100);
    }
    if (gameMode == 1) {
        DrawScreen(board, 0, 0);
    }
    else {
        DrawScreen(board1, 0, 0);
        DrawScreenForPL2(board2, 40, 0);
    }

    //이 부분 수정 의심 중
    for (x = 0; x < BW + 2; x++) {
        for (y = 0; y < BH + 2; y++) {
            if (y == 0 || y == BH + 1 || x == 0 || x == BW + 1) {
                board[x][y] = WALL;
                board2[x][y] = WALL;  // board2에도 벽 초기화
            }
            else {
                board[x][y] = EMPTY;
                board2[x][y] = EMPTY;  // board2에도 빈 공간 초기화
            }
        }
    }

    DrawScreen(board, 0, 0);
    if (gameMode == 2) {
        DrawScreenForPL2(board2, 40, 0);
    }
    nFrame = 20;
    nFrame2 = 20;

    // 전체 게임 루프
    //수정필요!!
    if (gameMode == 1) {
        for (; ;) {
            gotoxy(50, 9); printf("LV : %d", level);
            brick = random(sizeof(Shape) / sizeof(Shape[0])); //벽돌 하나 출력
            //brickForPL2 = random(sizeof(Shape) / sizeof(Shape[0]));
            nx = BW / 2;
            //nx2 = BW / 2;
            ny = 3;
            //ny2 = 3;
            rot = 0;
            //rotForPL2 = 0;
            n_rot = 0;
            brick = n_brick;
            n_brick = rand() % (sizeof(Shape) / sizeof(Shape[0]));
            PrintBrick(TRUE);
            pre();
            next_brick(TRUE);
            if (GetAround(nx, ny + 1, brick, rot) != EMPTY) {
                // 첫 번째 보드에서 충돌 발생 시
                TestFull();
                PlaySound(TEXT(R"(C:\Users\Arthur\Desktop\sound_Asset\ping.wav)"), NULL, SND_FILENAME | SND_ASYNC);
                return TRUE;
            }
            if (GetAround(nx, ny, brick, rot) != EMPTY) break; // 빈칸이 없으면 끝

            // 벽돌 하나가 바닥에 닿을 때까지의 루프
            nStay = nFrame;//이 부분 추가 설정 필요!
            //nStay2 = nFrame2;
            for (; 2;) {
                if (--nStay == 0) {
                    nStay = nFrame;
                    if (MoveDown()) break;
                }
                /*if (--nStay2 == 0) {
                    nStay2 = nFrame2;
                    if (MoveDownOnBoard2())break;
                }*/
                if (ProcessKey()) {
                    MoveDown();
                }
                /*if (ProcessKeyForPL2()) {
                    if (gameMode == 2) {
                        //MoveDown();
                        MoveDownOnBoard2();
                    }
                }*/
                delay(1000 / 20); // 벽돌이 조금씩 내려가도록 하기 위해
            }
        }
    }
    if (gameMode == 2) {
        for (; ;) {
            gotoxy(50, 9); printf("LV : %d", level);
            brick = random(sizeof(Shape) / sizeof(Shape[0])); //벽돌 하나 출력
            brickForPL2 = random(sizeof(Shape) / sizeof(Shape[0]));
            nx = BW / 2;
            nx2 = BW / 2;
            ny = 3;
            ny2 = 3;
            rot = 0;
            rotForPL2 = 0;
            n_rot = 0;
            brick = n_brick;
            n_brick = rand() % (sizeof(Shape) / sizeof(Shape[0]));
            PrintBrick(TRUE);
            PrintBrickOnBoard2(TRUE);
            pre();
            next_brick(TRUE);
            if (GetAround(nx, ny + 1, brick, rot) != EMPTY) {
                // 첫 번째 보드에서 충돌 발생 시
                TestFull();
                PlaySound(TEXT(R"(C:\Users\Arthur\Desktop\sound_Asset\ping.wav)"), NULL, SND_FILENAME | SND_ASYNC);
                return TRUE;
            }
            if (GetAround(nx, ny, brick, rot) != EMPTY) break; // 빈칸이 없으면 끝

            // 벽돌 하나가 바닥에 닿을 때까지의 루프
            nStay = nFrame;//이 부분 추가 설정 필요!
            nStay2 = nFrame2;
            for (; 2;) {
                if (--nStay == 0) {
                    nStay = nFrame;
                    if (MoveDown()) break;
                }
                if (--nStay2 == 0) {
                    nStay2 = nFrame2;
                    if (MoveDownOnBoard2())break;
                }
                if (ProcessKey()) {
                    MoveDown();
                }
                if (ProcessKeyForPL2()) {
                    if (gameMode == 2) {
                        //MoveDown();
                        MoveDownOnBoard2();
                    }
                }
                delay(1000 / 20); // 벽돌이 조금씩 내려가도록 하기 위해
            }
        }
    }
    clrscr();
    putsxy(30, 12, "G A M E  O V E R");
    PlaySound(TEXT(R"(C:\Users\Arthur\Desktop\sound_Asset\negative_beeps.wav)"), NULL, SND_FILENAME); //종료 사운드 부분 추가
    PlaySound(NULL, NULL, 0);
    putxyfn(30, 15, "Best score: %d \n", score);
    showcursor(TRUE);

}



/*void ShowAndDropItem(int x, int y, int item)
{
    for (int i = 0; i < 4; i++) {
        gotoxy(BX + (x + Shape[item][0][i].x) * 2, BY + y + Shape[item][0][i].y);
        puts(arTile[BRICK]);
        delay(100);  // 블록이 떨어지는 속도를 조절하기 위한 딜레이
    }
}*/

void DrawScreen(int board[BW + 2][BH + 2], int x, int y) {
    for (int i = 0; i < BW + 2; i++) {
        for (int j = 0; j < BH + 2; j++) {
            putsxy(BX + x * 2 + i * 2, BY + y + j, arTile[board[i][j]]);
            putsxy(50, 12, "Drawscreen is working!");
        }
    }

    //putsxy(50, 3, "Tetris Ver 1.0");
    putsxy(50, 5, "좌우:이동, 위:회전, 아래:내림");
    putsxy(50, 6, "공백:전부 내림");
    gotoxy(35, 12);
    putxyfn(50, 8, "Score: %d", score);

    if (gameMode == 1||2) {
        putsxy(50, 2, "Player 1");
        putxyfn(50, 8, "Score: %d", score);
    }
    if (gameMode==2) {
        putsxy(110, 2, "Player 2");
        putxyfn(110, 8, "Score: %d", score);
        putxyfn(110, 10,"%d", gameMode);
    }
}

/*void DrawScreenForPL2(int board2[BW + 2][BH + 2], int x, int y) {
    for (int i = 0; i < BW + 2; i++) {
        for (int j = 0; j < BH + 2; j++) {
            putsxy(BX + x * 2 + i * 2, BY + y + j, arTile[board2[i][j]]);
            putsxy(110, 12, "Drawscreen is working!");//작동을 하지 않음!!! 디버깅 결과
        }
    }
}*/
void DrawScreenForPL2(int board2[BW + 2][BH + 2], int x, int y) {
    for (int i = 0; i < BW + 2; i++) {
        for (int j = 0; j < BH + 2; j++) {
            if (gameMode == 2) {
                putsxy(BX + i * 2 + 80, BY + y + j, arTile[board2[i][j]]);
            }
            //putsxy(BX + i * 2 + 80, BY + y + j, arTile[board2[i][j]]);
            putsxy(110, 12, "Drawscreen is working!");
        }
    }
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
                    putxyfn(50, 10, "input: %d", ch);
                }
                break;
            case RIGHT:
                if (GetAround(nx + 1, ny, brick, rot) == EMPTY) {
                    PrintBrick(FALSE);
                    nx++;
                    PrintBrick(TRUE);
                    putxyfn(50, 10, "input: %d", ch);
                }
                break;
            case UP:
                if (GetAround(nx, ny, brick, (rot + 1) % 4) == EMPTY) {
                    PrintBrick(FALSE);
                    rot = (rot + 1) % 4;
                    PrintBrick(TRUE);
                    putxyfn(50, 10, "input: %d", ch);
                }
                break;
            case DOWN:
                if (MoveDown()) {
                    return TRUE;
                    putxyfn(50, 10, "input: %d", ch);
                }
                break;

            }

        }
        else {
            switch (ch) {
            case ' ':
                while (MoveDown() == FALSE) { ; }
                return TRUE;
            case '+':
                UseStopItem();
                break;

            case ESC:
                exit(0);
            }
        }
    }
    return FALSE;
}

BOOL ProcessKeyForPL2()
{
    if (GetAsyncKeyState('D') & 0x8000) {
        // 'D' 키가 눌렸을 때 오른쪽으로 이동
        if (GetAroundForPL2(nx2 + 1, ny2, brickForPL2, rotForPL2) == EMPTY) {
            PrintBrickOnBoard2(FALSE);
            delay(50);
            nx2++;
            PrintBrickOnBoard2(TRUE);
        }
    }
    if (GetAsyncKeyState('W') & 0x8000) {
        // 'W' 키가 눌렸을 때 회전
        if (GetAroundForPL2(nx2, ny2, brickForPL2, (rotForPL2 + 1) % 4) == EMPTY) {
            PrintBrickOnBoard2(FALSE);
            delay(50);
            rotForPL2 = (rotForPL2 + 1) % 4;
            PrintBrickOnBoard2(TRUE);
        }
    }
    if (GetAsyncKeyState('S') & 0x8000) {
        // 'S' 키가 눌렸을 때 아래로 이동
        if (MoveDownOnBoard2()) {
            delay(50);
            // 'S' 키 처리 후 바로 빠져나가지 않도록 수정
        }
    }
    if (GetAsyncKeyState(VK_TAB) & 0x8000) {
        // 'Tab' 키가 눌렸을 때 빠르게 아래로 이동
        while (MoveDownOnBoard2() == FALSE) { ; }
        delay(60);
        return TRUE;
    }
    if (GetAsyncKeyState(VK_ADD) & 0x8000) {
        // '+' 키가 눌렸을 때 아이템 사용
        UseStopItem();
    }
    if (GetAsyncKeyState('A') & 0x8000) {
        // 'A' 키가 눌렸을 때 왼쪽으로 이동
        if (GetAroundForPL2(nx2 - 1, ny2, brickForPL2, rotForPL2) == EMPTY) {
            PrintBrickOnBoard2(FALSE);
            delay(50);
            nx2--;
            PrintBrickOnBoard2(TRUE);
        }
    }
    if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
        // ESC 키가 눌렸을 때 종료
        exit(0);
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
void PrintBrickOnBoard2(BOOL Show)
{
    if (gameMode == 2) {
        for (int i = 0; i < 4; i++) {
            int x = BX + (Shape[brickForPL2][rotForPL2][i].x + nx2) * 2 + 80;
            int y = BY + Shape[brickForPL2][rotForPL2][i].y + ny2;
            gotoxy(x, y);
            puts(arTile[Show ? BRICK : EMPTY]);
        }
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

int GetAroundForPL2(int x, int y, int b, int r)
{
    int k2 = EMPTY;

    // 벽돌이 차지한 타일 모양 중 가장 큰 값을 찾는다.
    for (int i = 0; i < 4; i++) {
        k2 = max(k2, board2[x + Shape[b][r][i].x][y + Shape[b][r][i].y]);
    }
    return k2;
}

BOOL MoveDown()
{
    if (GetAround(nx, ny + 1, brick, rot) != EMPTY) {
        // 첫 번째 보드에서 충돌 발생 시
        TestFull();
        PlaySound(TEXT(R"(C:\Users\Arthur\Desktop\sound_Asset\ping.wav)"), NULL, SND_FILENAME | SND_ASYNC);
        return TRUE;
    }

    /*if (gameMode == 2 && GetAround(nx2, ny2 + 1, brickForPL2, rotForPL2) != EMPTY) {
        TestFull();
        // 두 번째 보드에서 충돌 발생 시
        //TestFullOnBoard2();
        return TRUE;
    }*/

    PrintBrick(FALSE);
    //PrintBrickOnBoard2(FALSE);
    ny++;
    PrintBrick(TRUE);
    //PrintBrick(TRUE);
    //PrintBrickOnBoard2(TRUE);
    putxyfn(50, 17, "y: %d", ny);
    return FALSE;
}

BOOL MoveDownOnBoard2()
{
    if (GetAroundForPL2(nx2, ny2 + 1, brickForPL2, rotForPL2) != EMPTY) {
        TestFullOnBoard2();
        return TRUE;
    }

    PrintBrickOnBoard2(FALSE);
    ny2++;
    PrintBrickOnBoard2(TRUE);
    putxyfn(110, 16, "y: %d", ny2);
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
                putsxy(50, 15, "hit!");
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
            putsxy(50, 16, "remove!");
            score += 100;
            if (score % 100 == 0) {
                LevelUp();
            }
            DrawScreen(board, 0, 0);
            delay(200);
        }


    }
}
void TestFullOnBoard2()
{
    // 바닥에 내려앉은 벽돌 기록
    for (int j = 0; j < 4; j++) {
        board2[nx2 + Shape[brickForPL2][rotForPL2][j].x][ny2 + Shape[brickForPL2][rotForPL2][j].y] = BRICK;
    }

    // 수평으로 가득찬 벽돌 제거
    for (int y = 1; y < BH + 1; y++) {
        BOOL bFull = TRUE;
        for (int x = 1; x < BW + 1; x++) {
            if (board2[x][y] == EMPTY) {
                bFull = FALSE;
                putsxy(110, 13, "hit!");
                break;
            }
        }
        // 한줄이 가득 찼으면 이 줄을 제거한다.
        if (bFull) {
            for (int ty = y; ty > 1; ty--) {
                for (int x = 1; x < BW + 1; x++) {
                    board2[x][ty] = board2[x][ty - 1];
                }
            }
            score += 100;
            if (score % 100 == 0) {
                LevelUp();
            }
            DrawScreen(board2, 40, 0);
            delay(200);
        }
    }
}

void next_brick(BOOL Show) {

    int i;
    for (i = 0; i < 4; i++) {
        gotoxy(28 + (Shape[n_brick][n_rot][i].x + nx) * 2, 3 + Shape[n_brick][n_rot][i].y + ny);
        puts(arTile[Show ? BRICK : CLEAN]);
    }

}

void pre() {
    if (showPreview) {
        gotoxy(35, 3); puts(" 다음 블록\n");
        gotoxy(35, 4); puts("              \n");
        gotoxy(35, 5); puts("              \n");
        gotoxy(35, 6); puts("              \n");
        gotoxy(35, 7); puts("              \n");
        gotoxy(35, 8); puts("              \n");
        gotoxy(35, 9); puts("              \n");
        gotoxy(35, 10); puts("              \n");
        gotoxy(35, 11); puts("              \n");
    }
}

void AddItem(char item, int count) {
    // 메모리 할당 시도
    ItemNode* newItem = (ItemNode*)malloc(sizeof(ItemNode));
    if (newItem == NULL) {
        // 실패 시 처리
        fprintf(stderr, "메모리 할당에 실패했습니다.\n");

    }

    // 메모리 초기화
    newItem->item = item;
    newItem->count = count;
    newItem->next = NULL;  // 새 노드의 다음 노드는 초기에는 NULL로 설정합니다.

    // 연결 리스트에 노드 추가
    newItem->next = ItemHead;
    ItemHead = newItem;
}


void LevelUp() {

    level++;
    DrawScreen(board, 0, 0);
    delay(100);

    // 기존의 아이템 노드 모두 삭제
    while (ItemHead != NULL) {
        ItemNode* temp = ItemHead;
        ItemHead = ItemHead->next;
        free(temp);
    }

    AddItem('+', 1);
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

    //ItemCnt();
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

void UseStopItem() {
    // 아이템이 아직 증정되지 않았으면 사용할 수 없음
    if (ItemHead == NULL || ItemHead->item != '+') {
        return;
    }
    TimeStop = TRUE;
    DrawScreen(board, 0, 0);
    delay(5000);
    TimeStop = FALSE;
    RemoveUsedItem('+');
    DrawScreen(board, 0, 0);
    ItemCnt();
}
