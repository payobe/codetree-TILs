#include <iostream>
#include <vector>

#define COUNT 48

using namespace std;

int dr[4] = { -1,0,1,0 };
int dc[4] = { 0,1,0,-1 };
int fd[COUNT] = { 0,1,2,2,3,3,0,0,0,1,1,1,2,2,2,2,3,3,3,3,0,0,0,0,
2,2,2,2,1,1,1,1,0,0,0,0,3,3,3,2,2,2,1,1,0,0,3,2 };


int n, m, h, k;
int turn;
int point = 0;

struct People {
    int r, c, direction, live;
};

vector<People>runner;
vector<vector<int>>tree;
People finder;
// 술래와의 맨하탄 거리 측정
int distance(int i) {
    int r = runner[i].r;
    int c = runner[i].c;
    int a = finder.r;
    int b = finder.c;
    return abs(r - a) + abs(c - b);
}
// 격자 밖 체크
bool is_range(int r, int c) {
    if (r < 0 || n <= r)return false;
    if (c < 0 || n <= c)return false;
    return true;
}
// 러너 움직임
void runner_move(int i) {
    int r = runner[i].r;
    int c = runner[i].c;
    int dir = runner[i].direction;
    int tr = r + dr[dir];
    int tc = c + dc[dir];
    // 격자 밖이 아닐때
    if (is_range(tr, tc)) {
        if (tr != finder.r || tc != finder.c) {
            runner[i].r = tr;
            runner[i].c = tc;
        }
    }
    // 격자 밖일 때
    else {
        dir = dir + 2;
        dir %= 4;
        tr = r + dr[dir];
        tc = c + dc[dir];
        if (tr != finder.r || tc != finder.c) {
            runner[i].r = tr;
            runner[i].c = tc;
        }
    }
}
//main게임
//러너 잡기
int catch_runner(int r, int c) {
    int ret = 0;
    for (int i = 0; i < m; i++) {
        if (runner[i].live == 0)continue;
        if (runner[i].r == r && runner[i].c == c) {
            ret++;
            runner[i].live = 0;
        }
    }
    return ret;
}
void game_start() {
    // 도망자 턴
    for (int i = 0; i < m; i++) {
        if (runner[i].live == 0)continue;
        int dist = distance(i);
        if (dist <= 3) { // 3칸 이하 찾기
            runner_move(i);// 움직이기
        }
    }
    // 술래 턴
    // 술래 움직이기
    int r = finder.r;
    int c = finder.c;
    int dir = fd[turn % COUNT];
    int next_dir = fd[(turn + 1) % COUNT];
    finder.r = r + dr[dir];
    finder.c = c + dc[dir];
    // 술래가 도망자잡기
    // 내칸 포함 3칸
    dir = next_dir;
    r = finder.r;
    c = finder.c;
    int tr = r;
    int tc = c;

    int catch_count = 0;
    for (int i = 0; i < 3; i++) {
        if (!is_range(tr, tc))break;
        // 나무 있는지 
        if (tree[tr][tc] == 1) {
            tr += dr[dir];
            tc += dc[dir];
            continue;
        }
        //없으면 다 잡기
        int ret = catch_runner(tr,tc);
        catch_count += ret;
        tr += dr[dir];
        tc += dc[dir];

    }
    // 점수 얻기(나무 안잡힘)// turn +1 해주셔야합니다
    point += (turn + 1) * catch_count;
}

int main() {
    // 여기에 코드를 작성해주세요.
    cin >> n >> m >> h >> k;
    // 도망자, 술래, 나무 ,턴
    runner = vector<People>(m);
    tree = vector<vector<int>>(n, vector<int>(n)); // r,c한칸씩 줄여서 
    finder = { n / 2,n / 2, 0 , 1 };
    for (int i = 0; i < m; i++) {
        int ta, tb, tc; cin >> ta >> tb >> tc;
        runner[i] = { ta - 1 , tb - 1 , tc , 1 };
    }
    for (int i = 0; i < h; i++) {
        int ta, tb; cin >> ta >> tb;
        tree[ta - 1][tb - 1] = 1;
    }

    for (int i = 0; i < k; i++) {
        turn = i;
        game_start();
    }

    cout << point;

    return 0;
}