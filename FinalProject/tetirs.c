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
BOOL TimeStop = FALSE;
BOOL TimeStop2 = FALSE;//시간 정지 상태
void TestFull();
void next_brick(BOOL Show);//다음 블록   
void pre();//블록 미리보기
void AddItem();
void LevelUp();
void StopItem();
void UseStopItem();
void AddItem2();
void LevelUp2();
void StopItem2();
void UseStopItem2();
//void RemoveItem();
BOOL showPreview = TRUE;
BOOL MoveDownOnBoard2();
void TestFullOnBoard2();
int gameMode;
void RemoveTopLine();
void DrawBomb();
void BombItem();
void RemoveTopLine2();
void DrawBomb2();
void BombItem2();
void preHide();
void preboard2();
void next_brick_2(BOOL Show);

void resizeConsole(int width, int height) {
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

    // 콘솔 스크린 버퍼 크기 설정
    COORD newSize;
    newSize.X = width;
    newSize.Y = height;
    SetConsoleScreenBufferSize(hStdout, newSize);

    // 콘솔 창 정보를 가져오기
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hStdout, &csbi);

    // 콘솔 창 크기 설정
    SMALL_RECT rect;
    rect.Top = 0;
    rect.Left = 0;
    rect.Bottom = height - 1; // 창의 높이
    rect.Right = width - 1; // 창의 너비

    SetConsoleWindowInfo(hStdout, TRUE, &rect);
}

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

typedef struct ItemNode2 {
    char item2;
    int count2;
    struct ItemNode* next2;
} ItemNode2;

ItemNode2* ItemHead2 = NULL;

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
int score2 = 0;
int level2 = 1;

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
//    PlaySound(TEXT(R"(C:\Users\Arthur\Desktop\sound_Asset\BGM.wav)"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
//}


int main() {

    resizeConsole(170, 30);// 듀오 프레임 크기 조절
    int nFrame, nStay, nStay2;
    int x, y;
    /*if (AskUserForMusic()) {
        PlayBackgroundMusic();
    }*/

    showcursor(FALSE);
    randomize();
    clrscr();

    gotoxy(30, 10); printf("게임모드를 설정해주세요 1 OR 2\n");
    putsxy(30, 11, "1. 싱글플레이");
    putsxy(30, 12, "2. 듀오플레이");
    int gameMode = 0;
    while (TRUE)
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

        if (GetAroundForPL2(nx2, ny2, brickForPL2, rotForPL2) != EMPTY) break;

        // 벽돌 하나가 바닥에 닿을 때까지의 루프
        nFrame = 20;
        nStay = nStay2 = nFrame;
        for (; 2;) {
            if (--nStay == 0) {
                nStay = nFrame;
                MoveDown();
                
            }
            if (gameMode == 2 && --nStay2 == 0) {
                nStay2 = nFrame;
                MoveDownOnBoard2();
            }
            if (ProcessKey())
            {
                if (gameMode == 1) {
                    MoveDown();
             
                }
            }
            if (GetAround(nx, ny, brick, rot) != EMPTY) break; // 빈칸이 없으면 끝

            if (gameMode == 2 && ProcessKeyForPL2()) {
                    MoveDownOnBoard2();
            }
            delay(1000 / 20); // 벽돌이 조금씩 내려가도록 하기 위해
        }
    }
    clrscr();
    putsxy(30, 12, "G A M E  O V E R");
    //PlaySound(TEXT(R"(C:\Users\Arthur\Desktop\sound_Asset\negative_beeps.wav)"), NULL, SND_FILENAME); //종료 사운드 부분 추가
    //PlaySound(NULL, NULL, 0);
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
    putsxy(130, 5, "a,d:이동, w:회전, s:내림");
    putsxy(130, 6, "공백:전부 내림");
    gotoxy(30, 12);
    printf("+(시간정지): %d", ItemCnt('+'));
    gotoxy(30, 13);
    printf("-(한줄 지우기): %d", ItemCnt('-'));
    gotoxy(30, 14);
    printf("/(블록 가리기): %d", ItemCnt('/'));
    gotoxy(30, 15);
    printf("*(폭탄): %d", ItemCnt('*'));
    putxyfn(50, 8, "Score: %d", score);
    if (gameMode == 2) {
        gotoxy(100, 12);
        printf("1(시간정지): %d", ItemCnt2('1'));
        gotoxy(100, 13);
        printf("2(한줄 지우기): %d", ItemCnt2('2'));
        gotoxy(100, 14);
        printf("3(블록 가리기): %d", ItemCnt2('3'));
        gotoxy(100, 15);
        printf("4(폭탄): %d", ItemCnt2('4'));
    }

    if (gameMode == 1||2) {
        putsxy(50, 2, "Player 1");
        putxyfn(50, 8, "Score: %d", score);
    }
    else if (board == board2) {
        putsxy(130, 2, "Player 2");
        putxyfn(130, 8, "Score: %d", score2);
        putxyfn(130, 9, "Lv: %d", level2);
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
                StopItem();
                break;
            case '/':
                pre();
                break;
            case '-':
                RemoveTopLine();
                break;
            case '*':
                BombItem();
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
    if (kbhit()) {
        int ch2 = getch();
        if (ch2 == 0xE0 || ch2 == 0) {
            ch2 = getch();
            switch (ch2) {
            case 100:
                // 오른쪽으로 이동
                if (GetAround(nx2 + 1, ny2, brickForPL2, rotForPL2) == EMPTY) {
                    PrintBrickOnBoard2(FALSE);
                    nx2++;
                    PrintBrickOnBoard2(TRUE);
                }
                break;
            case 119:
                // 회전
                if (GetAround(nx2, ny2, brickForPL2, (rotForPL2 + 1) % 4) == EMPTY) {
                    PrintBrickOnBoard2(FALSE);
                    rotForPL2 = (rotForPL2 + 1) % 4;
                    PrintBrickOnBoard2(TRUE);
                }
                break;
            case 115:
                // 아래로 이동
                if (MoveDownOnBoard2()) {
                    return TRUE;
                }
                break;
            case 97:
                // 왼쪽으로 이동
                if (GetAround(nx2 - 1, ny2, brickForPL2, rotForPL2) == EMPTY) {
                    nx2--;
                    PrintBrickOnBoard2(TRUE);
                }
                break;
            }
        }
        else {
            switch (ch2) {
            case ' ':
                while (MoveDownOnBoard2() == FALSE) { ; }
                return TRUE;
                break;
            case ESC:
                exit(0);
            }
        }
    }
    return FALSE;
}

BOOL MoveDown()
{
    if (GetAround(nx, ny + 1, brick, rot) != EMPTY) {
        // 첫 번째 보드에서 충돌 발생 시
        TestFull();
        //PlaySound(TEXT(R"(C:\Users\Arthur\Desktop\sound_Asset\ping.wav)"), NULL, SND_FILENAME | SND_ASYNC);
        return TRUE;
    }

    if (gameMode == 2 && GetAround(nx2, ny2 + 1, brickForPL2, rotForPL2) != EMPTY) {
        // 두 번째 보드에서 충돌 발생 시
        TestFullOnBoard2();
        return TRUE;
    }

    PrintBrick(FALSE);
    PrintBrickOnBoard2(FALSE);

    ny++;

    PrintBrick(TRUE);
    PrintBrickOnBoard2(TRUE);

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

    // 두 번째 보드의 상태를 올바르게 출력하기 위해 ny2 좌표를 업데이트한 후에
    // DrawScreen 함수를 호출합니다.
    DrawScreen(board2, 40, 0);

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
    for (int i = 0; i < 4; i++) {
        gotoxy(BX + (Shape[brickForPL2][rotForPL2][i].x + nx2) * 2 + 80, BY + Shape[brickForPL2][rotForPL2][i].y + ny2);
        puts(arTile[Show ? BRICK : EMPTY]);
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
            score2 += 100;
            if (score2 % 100 == 0) {
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

   
    for (i = 0; i < 4; i++) {
        if (gameMode == 2) {
            gotoxy(107 + (Shape[n_brick][n_rot][i].x + nx2) * 2, 3 + Shape[n_brick][n_rot][i].y + ny2);
            puts(arTile[Show ? BRICK : CLEAN]);
        }
    }
  
}


void pre() {
    
    gotoxy(35, 3); puts("다음 블록    \n");
    gotoxy(35, 4); puts("              \n");
    gotoxy(35, 5); puts("              \n");
    gotoxy(35, 6); puts("              \n");
    gotoxy(35, 7); puts("              \n");
    gotoxy(35, 8); puts("              \n");
    gotoxy(35, 9); puts("              \n");
    gotoxy(35, 10); puts("              \n");
    gotoxy(35, 11); puts("              \n");
    
}


void preboard2() {

    if (gameMode == 2) {
        gotoxy(100, 3); puts("다음 블록    \n");
        gotoxy(100, 4); puts("              \n");
        gotoxy(100, 5); puts("              \n");
        gotoxy(100, 6); puts("              \n");
        gotoxy(100, 7); puts("              \n");
        gotoxy(100, 8); puts("              \n");
        gotoxy(100, 9); puts("              \n");
        gotoxy(100, 10); puts("              \n");
        gotoxy(100, 11); puts("              \n");
    }

}

void AddItem(char item, int count) { // 새로운 아이템을 아이템 리스트에 추가하는 함수
    // 음수 또는 0 이하의 아이템 개수는 처리하지 않음
    if (count <= 0) {
        fprintf(stderr, "유효하지 않은 아이템 개수입니다.\n");
        return;
    }

    // 현재 아이템 리스트의 시작 노드를 가리키는 포인터
    ItemNode* current = ItemHead;

    // 아이템 리스트를 순회하면서 이미 추가된 아이템인지 확인
    while (current != NULL) {
        // 이미 추가된 아이템이면 개수만 증가시키고 함수 종료
        if (current->item == item) {
            current->count += count;
            return;
        }
        current = current->next; // 다음 노드로 이동
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

    char randomItem;
    int RandomItemIndex = rand() % 4;

    switch (RandomItemIndex) {
    case 0:
        randomItem = '+';
        break;
    case 1:
        randomItem = '/';
        break;
    case 2:
        randomItem = '-';
        break;
    case 3:
        randomItem = '*';
        break;
    }

    // 선택된 아이템을 추가
    AddItem(randomItem, 1);
}

int ItemCnt(char targetItem) { // 현재 보유한 아이템의 총 개수를 반환하는 함수
    int itemcnt = 0;

    // 현재 아이템 리스트의 시작 노드를 가리키는 포인터
    ItemNode* current = ItemHead;
    while (current != NULL) {
        // 현재 노드가 NULL이 아닐 때까지 반복
        if (current->item == targetItem || targetItem == '\0') {
            // 현재 노드의 아이템 개수를 전체 아이템 개수에 더함
            itemcnt += current->count;
        }
        // 다음 노드로 이동
        current = current->next;
    }
    // 계산된 전체 아이템 개수 반환
    return itemcnt;
}

void RemoveUsedItem(char item) { // 사용된 아이템을 아이템 리스트에서 제거하는 함수 
    // 현재 아이템 리스트의 시작 노드를 가리키는 포인터
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

void BombItem() {

    int BombItemCnt = ItemCnt('*');

    if (BombItemCnt > 0) {
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
        RemoveUsedItem('*');
        DrawScreen(board, 0, 0);

        nx = BW / 2;
        ny = 3;
        rot = 0;
        next_brick(TRUE);
    }

}

void AddItem2(char item2, int count2) { // 새로운 아이템을 아이템 리스트에 추가하는 함수
    // 음수 또는 0 이하의 아이템 개수는 처리하지 않음
    if (count2 <= 0) {
        fprintf(stderr, "유효하지 않은 아이템 개수입니다.\n");
        return;
    }

    // 현재 아이템 리스트의 시작 노드를 가리키는 포인터
    ItemNode2* current2 = ItemHead;

    // 아이템 리스트를 순회하면서 이미 추가된 아이템인지 확인
    while (current2 != NULL) {
        // 이미 추가된 아이템이면 개수만 증가시키고 함수 종료
        if (current2->item2 == item2) {
            current2->count2 += count2;
            return;
        }
        current2 = current2->next2; // 다음 노드로 이동
    }

    // 새로운 아이템을 저장할 노드 동적 할당
    ItemNode2* newItem2 = (ItemNode2*)malloc(sizeof(ItemNode2));
    if (newItem2 == NULL) {
        // 메모리 할당 실패 시 에러 메시지 출력 후 함수 종료
        fprintf(stderr, "메모리 할당에 실패했습니다.\n");
        return;
    }

    // 새로운 아이템 정보 설정
    newItem2->item2 = item2;
    newItem2->count2 = count2;
    newItem2->next2 = NULL;

    // 연결 리스트에 노드 추가
    if (ItemHead2 == NULL) {
        // 리스트가 비어있으면 새로운 노드를 리스트의 첫 번째 노드로 설정
        ItemHead2 = newItem2;
    }
    else {
        // 리스트가 비어있지 않으면 끝까지 순회하여 마지막 노드 뒤에 새로운 노드 추가
        current2 = ItemHead2;
        while (current2->next2 != NULL) {
            current2 = current2->next2;
        }
        current2->next2 = newItem2;
    }
}

void LevelUp2() {

    level2++;
    DrawScreen(board, 0, 0);
    delay(100);

    char randomItem2;
    int RandomItemIndex2 = rand() % 4;

    switch (RandomItemIndex2) {
    case 0:
        randomItem2 = '1';
        break;
    case 1:
        randomItem2 = '2';
        break;
    case 2:
        randomItem2 = '3';
        break;
    case 3:
        randomItem2 = '4';
        break;
    }

    // 선택된 아이템을 추가
    AddItem(randomItem2, 1);
}

int ItemCnt2(char targetItem2) { // 현재 보유한 아이템의 총 개수를 반환하는 함수

    int itemcnt2 = 0;

    // 현재 아이템 리스트의 시작 노드를 가리키는 포인터
    ItemNode2* current2 = ItemHead2;
    while (current2 != NULL) {
        // 현재 노드가 NULL이 아닐 때까지 반복
        if (current2->item2 == targetItem2 || targetItem2 == '\0') {
            // 현재 노드의 아이템 개수를 전체 아이템 개수에 더함
            itemcnt2 += current2->count2;
        }
        // 다음 노드로 이동
        current2 = current2->next2;
    }
    // 계산된 전체 아이템 개수 반환
    return itemcnt2;
}

void RemoveUsedItem2(char item2) { // 사용된 아이템을 아이템 리스트에서 제거하는 함수 
    // 현재 아이템 리스트의 시작 노드를 가리키는 포인터
    ItemNode2* current2 = ItemHead2;
    // 현재 노드의 이전 노드를 가리키는 포인터
    ItemNode2* prev2 = NULL;

    // 아이템 리스트를 순회하면서 사용한 아이템 찾기
    while (current2 != NULL) {
        // 찾았을 경우 해당 아이템 노드의 카운트를 감소
        if (current2->item2 == item2) {
            current2->count2--;

            // 만약 해당 아이템의 카운트가 0이면 노드를 삭제하고 메모리 해제
            if (current2->count2 == 0) {
                if (prev2 != NULL) {
                    // 이전 노드가 존재하면 해당 노드의 다음 노드를 현재 노드의 다음 노드로 설정
                    prev2->next2 = current2->next2;
                }
                else {
                    // 이전 노드가 없으면 현재 노드가 리스트의 첫 번째 노드이므로 리스트의 시작을 현재 노드의 다음 노드로 설정
                    ItemHead2 = current2->next2;
                }
                free(current2); // 현재 노드 메모리 해제
            }

            break;
        }

        // 다음 노드로 이동
        prev2 = current2;
        current2 = current2->next2;
    }
}

void StopItem2() {

    int plusItemCnt2 = ItemCnt('1');

    if (plusItemCnt2 > 0) {
        TimeStop2 = TRUE;
        RemoveUsedItem2('1');
        DrawScreen(board, 0, 0);
        delay(5000);
        TimeStop2 = FALSE;
    }
}

void preHide2() {

    int HideItemCnt2 = ItemCnt('3');

    if (HideItemCnt2 > 0) {
        RemoveUsedItem('3');
        pre2(FALSE);
        Sleep(500);
        next_brick2(TRUE);
    }
}

void RemoveTopLine2() {

    int RemoveItemCount2 = ItemCnt('2');

    // 만약 현재 아이템이 '-'가 아니거나, 아이템이 아직 증정되지 않았다면 함수 실행을 중단합니다.
    if (RemoveItemCount2 > 0) {

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
        RemoveUsedItem2('2');
        DrawScreen(board, 0, 0);
    }
}

void BombItem2() {

    int BombItemCnt2 = ItemCnt2('*');

    if (BombItemCnt2 > 0) {
        int bombRange2 = 1;

        // 폭발 범위 내의 블록을 제거
        for (int i = -bombRange2; i <= bombRange2; i++) {
            for (int j = -bombRange2; j <= bombRange2; j++) {
                int targetX2 = nx + i;
                int targetY2 = ny + j;

                // 범위를 벗어나지 않도록 체크
                if (targetX2 >= 1 && targetX2 <= BW && targetY2 >= 1 && targetY2 <= BH) {

                    // 폭탄 모양 출력
                    DrawBomb(targetX2, targetY2);
                    // 일정 시간 동안 딜레이
                    delay(100);
                    // 폭탄이 터진 위치를 다시 보드로 설정
                    board[targetX2][targetY2] = EMPTY;

                }
            }
        }
        RemoveUsedItem2('4');
        DrawScreen(board, 0, 0);

        nx = BW / 2;
        ny = 3;
        rot = 0;
        next_brick2(TRUE);
    }

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