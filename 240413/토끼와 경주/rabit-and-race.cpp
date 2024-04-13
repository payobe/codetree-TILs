#include <iostream>
#include<unordered_map>
#include<algorithm>
#include<vector>
#include<set>

#define ll long long

using namespace std;

int dr[4] = { -1, 0, 1, 0 };
int dc[4] = { 0,1,0,-1 };

int n, m, p;
struct Rabbit {
    int r, c;
    int dist;
    int jump_count =0;
    int id;
    int did = 0;
};
unordered_map<int, Rabbit>rabbit;
unordered_map<int, int>point;
unordered_map<int, int>is_visit;// 1이면 방문, 아닐 시 방문아님

struct rcmp {
    bool operator()(const Rabbit& a, const Rabbit& b)const {
        if (a.jump_count < b.jump_count)return true;
        if (a.jump_count > b.jump_count)return false;

        if (a.r + a.c < b.r + b.c)return true;
        if (a.r + a.c > b.r + b.c)return false;

        if (a.r < b.r)return true;
        if (a.r > b.r)return false;

        if (a.c < b.c)return true;
        if (a.c > b.c)return false;

        if (a.id < b.id)return true;
        if (a.id > b.id)return false;
        return false;
    }
};

set<Rabbit, rcmp>s;

int sum = 0;
vector<int>who;

//100
void ready() {
    cin >> n >> m >> p;
    for (int i = 0; i < p; i++) {
        int ta, tb; cin >> ta >> tb;
        who.push_back(ta - 1);
        rabbit[ta - 1].dist = tb;
        rabbit[ta - 1].id = ta - 1;
    }
    int ws = who.size();
    for (int i = 0; i < ws; i++) {
        rabbit[who[i]].r = 1;
        rabbit[who[i]].c = 1;
        rabbit[who[i]].jump_count = 0;
       
    }
}

bool cmp3(int first, int second) {
    Rabbit a = rabbit[first];
    Rabbit b = rabbit[second];
    if (a.did < b.did)return true;
    if (a.did > b.did)return false;

    if (a.r + a.c < b.r + b.c)return true;
    if (a.r + a.c > b.r + b.c)return false;

    if (a.r < b.r)return true;
    if (a.r > b.r)return false;

    if (a.c < b.c)return true;
    if (a.c > b.c)return false;

    if (a.id < b.id)return true;
    if (a.id > b.id)return false;
}
//범위 밖인지 체크
bool is_range(int r, int c) {
    if (r < 1 || n < r)return false;
    if (c < 1 || m < c)return false;
    return true;
}
// 4개방향중에 고르기
bool cmp2(pair<int, int>a, pair<int, int>b) {
    if (a.first + a.second > b.first + b.second)return true;
    if (a.first + a.second < b.first + b.second)return false;

    if (a.first > b.first)return true;
    if (a.first < b.first)return false;

    if (a.second > b.second)return true;
    if (a.second < b.second)return false;
    return false;
}
// 점수 내리고 전체점수 올리기
void give_minus(int cur, int p) {
    sum += p;
    if (is_visit[cur] != 1) {
        is_visit[cur] = 1;
        point[cur] = 0;
    }
    point[cur] -= p;
}
// 점수주기
void give_plus(int cur, int p) {
    if (is_visit[cur] != 1) {
        is_visit[cur] = 1;
        point[cur] = 0;
    }
    point[cur] += p;
}
//200 한번이라도 뽑혔는지 check?
void game_start() {
    int k, sc; cin >> k >> sc;

    // 이니시
    int ws = who.size();
    s.clear();
    for (int i = 0; i < ws; i++) {
        rabbit[who[i]].did = 0;
        s.insert(rabbit[who[i]]);
    }

    // 게임진행

    for (int i = 0; i < k; i++) {
        // 후보 선정
        int cur = (*s.begin()).id;
        // 후보 이동
        vector<pair<int, int>>vec;
        int r = rabbit[cur].r;
        int c = rabbit[cur].c;
        int dist = rabbit[cur].dist;
        for (int i = 0; i < 4; i++) {
             int tr = r + dr[i] * dist;
            int tc = c + dc[i] * dist;

            if (!is_range(tr, tc)) {
                if (dr[i] == 0) {
                    int ds = dist % (2 * m - 2);
                    if (dc[i] == 1||c==1) {
                        ds -= (m - c);
                        ds = abs(ds);
                        tc = m - ds;
                    }
                    else {
                        ds -= c - 1;
                        ds = abs(ds);
                        tc = 1 + ds;
                    }
                }
                else {
                    int ds = dist % (2 * n - 2);
                    if (dr[i] == 1 || r == 1) {
                        ds -= (n - r);
                        ds = abs(ds);
                        tr = n - ds;
                    }
                    else {
                        ds -= r - 1;
                        ds = abs(ds);
                        tr = 1 + ds;
                  
                    }
                }
            }
            vec.push_back({ tr,tc });
        }
        sort(vec.begin(), vec.end(), cmp2);
        pair<int, int>w = vec[0];
        s.erase(s.find(rabbit[cur]));
        rabbit[cur].r = w.first;
        rabbit[cur].c = w.second;
        rabbit[cur].jump_count++;
        rabbit[cur].did = 1;
        s.insert(rabbit[cur]);
        int p = w.first + w.second;

        give_minus(cur, p);
    }
    // s 주기 진행
    sort(who.begin(), who.end(), cmp3);
    give_plus(who[ws - 1], sc);
}
//300
void change_dist() {
    int cur, l; cin >> cur >> l;
    rabbit[cur-1].dist *= l;
}
// 포인트 얻기
int get_point(int cur) {
    if (is_visit[cur] != 1) {
        is_visit[cur] = 1;
        point[cur] = 0;
    }
    return point[cur];
}
//400   
void best_rabbit() {
    int ws = who.size();
    int ret = 0;
    for (int i = 0; i < ws; i++) {
        int p = get_point(who[i]);
        ret = max(ret, sum + p);
    }
    cout << ret;
}
int main() {
    // 여기에 코드를 작성해주세요.
    int q; cin >> q;
    for (int i = 0; i < q; i++) {
        int ta; cin >> ta;
        if (ta == 100) {
            ready();
        }
        else if (ta == 200) {
            game_start();
        }
        else if (ta == 300) {
            change_dist();
        }
        else {
            best_rabbit();
        }
    }
    return 0;
}