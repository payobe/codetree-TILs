#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

vector<int>belt_count;// 개수
vector<int>belt_start;// id
vector<int>belt_end;  // id
vector<int>prv, nxt;// id

int n, m, q;

//100
void do_cunstruct() {
    cin >> n >> m;
    // 초기화
    belt_count = vector<int>(n+1);
    belt_start = vector<int>(n+1);
    belt_end = vector<int>(n+1);
    prv = vector<int>(m + 1);
    nxt = vector<int>(m + 1);

    for (int i = 1; i <= m; i++) {
        int ta; cin >> ta;
        ta;
        if (belt_count[ta] == 0) {
            belt_start[ta] = i;
        }
        else {
            nxt[belt_end[ta]] = i;
            prv[i] = belt_end[ta];
        }
        belt_count[ta]++;
        belt_end[ta] = i;
    }
}
//200
void move_all() {
    int from, to; cin >> from >> to;
    //링크드 업데이트
    // to의 맨앞과 from맨뒤 연결
    if (belt_count[from] != 0) {
        if (belt_count[to] != 0) {
            prv[belt_start[to]] = belt_end[from];
            nxt[belt_end[from]] = belt_start[to];
            //to 밸트 start 업뎃
            belt_start[to] = belt_start[from];
            //from정보 삭제
            belt_end[from] = 0;
            belt_start[from] = 0;
            //개수 update
            belt_count[to] += belt_count[from];
            belt_count[from] = 0;
        }
        else {
            belt_count[to] = belt_count[from];
            belt_start[to] = belt_start[from];
            belt_end[to] = belt_end[from];

            belt_count[from] = 0;
            belt_start[from] = 0;
            belt_end[from] = 0;
        }
    }
    //출력 to의 물건개수 출력
    cout << belt_count[to] << '\n';
}
// 맨앞에 붙히기
void mplus(int to, int num) {
    if (num == 0)return;
    if (belt_count[to] == 0) {
        belt_count[to] = 1;
        belt_start[to] = num;
        belt_end[to] = num;

        nxt[num] = 0;
    }
    else {
        belt_count[to]++;
        nxt[num] = belt_start[to];
        prv[belt_start[to]] = num;
        belt_start[to] = num;
    }
}
void mminus(int to) {
    if (belt_count[to] == 0)return;
    if (belt_count[to] == 1) {
        belt_count[to] = 0;
        belt_end[to] = 0;
        belt_start[to] = 0;
    }
    else {
        belt_count[to]--;
        int a = belt_start[to];
        int b = nxt[a];
        belt_start[to] = b;
        prv[b] = 0;
        nxt[a] = 0;
    }
}
//두경우 공통
void ff(int first, int second) {
    int a = belt_start[first];
    int b = belt_start[second];
    //각각 때기
    mminus(first);
    mminus(second);
    //각각 더하기
    mplus(first, b);
    mplus(second, a);
}
//300
void move_front() {
    int first, second; cin >> first >> second;
    ff(first, second);

    cout << belt_count[second] << '\n';
}
//400
void move_split() {
    int from, to; cin >> from >> to;
    //개수 카운트
    int count = belt_count[from] / 2;
    if (count != 0) {
        //개수 만큼 포문돌면서  //target 찾기
        int target = belt_start[from];
        for (int i = 1; i < count; i++) {
            int next = nxt[target];
            target = next;
        }
        // from에서 끊음
        int b = nxt[target];
        int c = belt_start[from];
        prv[b] = 0;
        belt_start[from] = b;

        if (belt_count[to] != 0) {
            // target과 연결
            int a = belt_start[to];
            nxt[target] = a;
            prv[a] = target;
            // to start부분 없뎃
            belt_start[to] = c;
        }
        else {
            belt_start[to] = c;
            belt_end[to] = target;
            nxt[target] = 0;
            prv[c] = 0;
        }

        belt_count[to] += count;
        belt_count[from] -= count;
    }

    cout << belt_count[to] << '\n';
}
//500
void get_pinfo() {
    int p; cin >> p;
    int a = prv[p];
    int b = nxt[p];
    if (a == 0) a = -1;
    if (b == 0) b = -1;
    int ret = a + (2 * b);
    cout << ret << '\n';
}
//600
void get_binfo() {
    int belt; cin >> belt;
    int a = belt_start[belt];
    int b = belt_end[belt];
    int c = belt_count[belt];

    if (a == 0) a = -1;
    if (b == 0) b = -1;
    int ret = a + (2 * b) + (3 * c);
    cout << ret << '\n';
}
//
int main() {
    cin >> q;
    for (int i = 0; i < q; i++) {
        int ta; cin >> ta;
        if (ta == 100) {
            do_cunstruct();
        }
        else if (ta == 200) {
            move_all();
        }
        else if (ta == 300) {
            move_front();
        }
        else if (ta == 400) {
            move_split();
        }
        else if (ta == 500) {
            get_pinfo();
        }
        else {
            get_binfo();
        }
    }
    return 0;
}