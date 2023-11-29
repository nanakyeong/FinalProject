#define _CRT_NONSTDC_NO_WARNINGS

#include <stdio.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define randomize() srand((unsigned)time(NULL))
#define random(n) (rand() % (n))
#define delay(n) Sleep(n)
#define clrscr() system("cls")
#define gotoxy(x,y) { COORD Cur = {x, y}; \
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),Cur);}
#define showcursor(bShow) { CONSOLE_CURSOR_INFO CurInfo = {20, bShow}; \
   SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&CurInfo); }

enum { ESC = 27, LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80 };
#define putsxy(x, y, s) {gotoxy(x, y);puts(s);}
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
void next_brick();//다음 블록   
void pre();//블록 미리보기
void RandomItem();//아이템 증정
void BoardDelete();// @ 아이템의 블록 표현
void ShowRemove();//블록 내려오기
void RemoveLine();//라인 제거하고 블록 떨어뜨리기
void SwapBrick();

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

enum { EMPTY, BRICK, WALL, CLEAN };
char arTile[3][4] = { ". ","■","□" };
int board[BW + 2][BH + 2];
int nx, ny;
int brick, rot;
int n_brick, n_rot;
int score = 0;
int level = 1;

int main()
{

    int nFrame, nStay;
    int x, y;

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

        if (score % 50 == 0) {
            level++;
            DrawScreen();
            delay(100);
        }

        // 1부터 5까지의 랜덤한 아이템 선택
        int randomItem = rand() % 5 + 1;
        char itemCode;

        switch (randomItem) {
        case 1: itemCode = '+'; break;
        case 2: itemCode = '@'; break;
        case 3: itemCode = '-'; break;
        case 4: itemCode = '*'; break;
        case 5: itemCode = '/'; break;
        default: itemCode = '+'; break; // 기본값으로 '+'
        }

    }
    clrscr();
    putsxy(30, 12, "G A M E  O V E R");
    gotoxy(50, 14); printf("LV : %d", level);
    showcursor(TRUE);
}

void BoardDelete(int x, int y, BOOL Show) {

    for (int i = 0; i < 4; i++) {
        gotoxy(BX + (x + Shape[0][0][i].x) * 2, BY + y + Shape[0][0][i].y);
        puts(arTile[Show ? BRICK : CLEAN]);
    }
}

void ShowAndDropItem(int x, int y, int item)
{
    for (int i = 0; i < 4; i++) {
        gotoxy(BX + (x + Shape[item][0][i].x) * 2, BY + y + Shape[item][0][i].y);
        puts(arTile[BRICK]);
        delay(100);  // 블록이 떨어지는 속도를 조절하기 위한 딜레이
    }
}

void RemoveLine() {
    // 블록을 특정 위치에서 시작하도록 설정 (여기서는 가운데에서 시작)
    int startX = BW / 2;
    int startY = 1;

    // 블록을 표시하고 내리는 동작
    ShowAndDropItem(startX, startY, n_brick);

    // 블록이 특정 위치에 도달하면 해당 위치의 라인을 지우기
    int targetLine = 5;  // 예시로 5번째 라인을 지움
    if (startY >= targetLine) {
        // 라인 지우기
        for (int x = 1; x <= BW; x++) {
            gotoxy(BX + x * 2, BY + targetLine);
            puts(arTile[CLEAN]);
        }
    }
}

void SwapBrick(int brick, int n_brick) {
    // 현재 블록과 다음 블록을 교체
    int temp = brick;
    brick = n_brick;
    n_brick = temp;
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
            case '+':
                if (GetAround(nx, ny, brick, rot) == EMPTY && !TimeStop) {
                    PrintBrick(FALSE);
                    nx--;
                    PrintBrick(TRUE);
                }
                break;
            case '@':
                RandomItem('@');
                break;
            }

        }
        else {
            switch (ch) {
            case ' ':
                while (MoveDown() == FALSE) { ; }
                return TRUE;
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
        RemoveLine();
        TestFull();
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
            score += 10; // 한 줄당 10점 증가
            if (score % 50 == 0) { //점수가 50점 당 한 번씩 레벨을 올림
                level++;
                DrawScreen();
                delay(100); // 레벨 업을 알리기 위한 딜레이
            }
            DrawScreen();
            delay(200);
        }


    }
}

void next_brick(BOOL Show) {
    int i;
    // 미리보기를 가림
    if (!Show) {
        gotoxy(28, 3); puts("              ");
        gotoxy(28, 4); puts("              ");
        gotoxy(28, 5); puts("              ");
        gotoxy(28, 6); puts("              ");
        gotoxy(28, 7); puts("              ");
        gotoxy(28, 8); puts("              ");
        gotoxy(28, 9); puts("              ");
        gotoxy(28, 10); puts("              ");
        gotoxy(28, 11); puts("              ");
    }
    else {
        // 미리보기를 출력
        for (i = 0; i < 4; i++) {
            gotoxy(28 + (Shape[n_brick][n_rot][i].x + nx) * 2, 3 + Shape[n_brick][n_rot][i].y + ny);
            puts(arTile[Show ? BRICK : CLEAN]);
        }
    }
}

void pre() {

    gotoxy(35, 3); puts(" 다음블럭\n");
    gotoxy(35, 4); puts("              \n");
    gotoxy(35, 5); puts("              \n");
    gotoxy(35, 6); puts("              \n");
    gotoxy(35, 7); puts("              \n");
    gotoxy(35, 8); puts("              \n");
    gotoxy(35, 9); puts("              \n");
    gotoxy(35, 10); puts("              \n");
    gotoxy(35, 11); puts("              \n");
}

void RandomItem(char item) {

    switch (item) {
        // 시간 정지 기능 구현
    case '+':
        TimeStop = TRUE;
        Sleep(5000); // 5초간 정지
        TimeStop = FALSE;
        break;
    case '@':
        // 3x3 블록 내려오는 동작 구현
        for (int y = ny; y < ny + 3; y++) {
            BoardDelete(nx, y - 1, FALSE);  // 이전 위치 지우기
            BoardDelete(nx, y, TRUE);       // 새로운 위치에 표시
            delay(500);                      // 0.5초 동안 대기
        }

        // 3x3 영역 블록 제거
        for (int i = 0; i < 4; i++) {
            board[nx + Shape[brick][rot][i].x - 1][ny + Shape[brick][rot][i].y - 1] = EMPTY;
        }
        break;
    case '-':
        RemoveLine();
        break;
    case '*':
        SwapBrick(brick, n_brick);
        PrintBrick(TRUE);
        break;
    case '/':
        next_brick(FALSE);
        PrintBrick(TRUE);
        break;
    default:
        printf("존재하지 않는 아이템입니다.");
        break;
    }
}