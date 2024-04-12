#include <iostream>
#include<vector>
#include<algorithm>

using namespace std;
int dr[4] = { -1,0,1,0 };
int dc[4] = { 0,1,0,-1 };

int n, m, k;
vector<vector<vector<int>>>arr;
struct People {
    int r, c;
    int direction;
    int init;
    int gun = 0;
    int point = 0;
};
vector<People>player;

// 격자밖인지 check
bool is_range(int r, int c) {
    if (r < 0 || n <= r)return false;
    if (c < 0 || n <= c)return false;
    return true;
}
// 사람이 있는지 check
bool is_anyone(int r, int c) {
    for (int i = 0; i < m; i++) {
        if (player[i].r == r && player[i].c == c)return true;
    }
    return false;
}
// 제일 높은 총 줍기
void change_gun(int i) {
    int r = player[i].r;
    int c = player[i].c;
    int as = arr[r][c].size();
    sort(arr[r][c].begin(), arr[r][c].end());
    if (as == 0)return;
    int pgun = player[i].gun;
    // 바닥에 더 큰 총 있으면
    if (pgun < arr[r][c][as - 1]) {
        player[i].gun = arr[r][c][as - 1];
        arr[r][c].pop_back(); // 총 빼기
        arr[r][c].push_back(pgun);// 총 넣기
    }
}
// 누가 이기는지 판별
int who_win(int first, int second) {
    People p1 = player[first];
    People p2 = player[second];

    if (p1.init + p1.gun > p2.init + p2.gun) return first;
    if (p1.init + p1.gun < p2.init + p2.gun) return second;

    if (p1.init > p2.init)return first;
    if (p1.init < p2.init)return second;
}
// 싸우기
void fight(int r, int c) {
    int p1 = -1;
    int p2 = -1;
    // 플레이어 선정
    for (int i = 0; i < m; i++) {
        if (player[i].r == r && player[i].c == c) {
            if (p1 == -1)p1 = i;
            else p2 = i;
        }
    }
    // 승자 패자 난누기
    int loser, winner;
    winner = who_win(p1, p2);
    if (p1 == winner)loser = p2;
    else loser = p1;
    // 승자행동
    //  점수 획득
    int point = player[winner].init + player[winner].gun;
    point -= player[loser].init + player[loser].gun;
    player[winner].point += point;

    // 패자행동
    //  총 내려놓기
    int lr = player[loser].r;
    int lc = player[loser].c;
    arr[lr][lc].push_back(player[loser].gun);
    player[loser].gun = 0;
    //  위치 찾아서 가기
    int dir = player[loser].direction;
    for (int i = 0; i < 4; i++) {
        int tr = lr + dr[dir];
        int tc = lc + dc[dir];
        if (is_range(tr, tc) && !is_anyone(tr, tc)) {
            player[loser].r = tr;
            player[loser].c = tc;
            player[loser].direction = dir;
            break;
        }
        dir++;
        if (dir >= 4)dir = 0;
    }
    //  총줍기
    change_gun(loser);
    change_gun(winner);
}
// main 게임
void game_start() {
    for (int i = 0; i < m; i++) {
        int r = player[i].r;
        int c = player[i].c;
        int dir = player[i].direction;
        // 방향 바꾸기
        if (!is_range(r + dr[dir], c + dc[dir])) {
            dir += 2;
            dir %= 4;
        }
        // 있으면 fight
        if (is_anyone(r+dr[dir], c+dc[dir])) {
            player[i].r = r + dr[dir];
            player[i].c = c + dc[dir];
            player[i].direction = dir;
            fight(r + dr[dir] , c + dc[dir]);
        }
        // 없으면 무난 총줍기
        else {
            player[i].r = r + dr[dir];
            player[i].c = c + dc[dir];
            player[i].direction = dir;
            change_gun(i);
        }
    }
}

//
int main() {
    cin >> n >> m >> k;
    arr = vector<vector<vector<int>>>(n, vector<vector<int>>(n));
    player = vector<People>(m);
    // 총 정보
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int ta; cin >> ta;
            arr[i][j].push_back(ta);
        }
    }
    // 플레이어 정보
    for (int i = 0; i < m; i++) {
        int r, c, direction, init;
        cin >> r >> c >> direction >> init;
        player[i] = { r -1 ,c- 1,direction,init,0,0 };
    }
    // k 번 게임 시작
    for (int i = 0; i < k; i++) {
        game_start();
    }

    for (int i = 0; i < m; i++) {
        cout << player[i].point << ' ';
    }
    return 0;
}