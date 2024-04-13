#include <iostream>
#include<vector>

#define MAX 20
using namespace std;

int dy[4] = { 1,0,-1,0 };
int dx[4] = { 0,1,0,-1 };

int n, k;
vector<int>d;
vector<vector<int>>arr;
vector<vector<int>>arr2;
vector<int>diff;
int count = 0;
int from;
vector<int>cut;

bool is_range(int x, int y) {
    if (x < 0 || MAX <= x)return false;
    if (y < 0 || MAX <= y)return false;
    if (arr[x][y] == -1)return false;
    return true;
}
bool is_range2(int y, int x) {
    if (x < 0 || 25 <= x)return false;
    if (y < 0 || 4 <= y)return false;
    if (arr2[y][x] == -1)return false;
    return true;
}
//피자만들기 메인
void do_pizza() {
    // 가장 작은애들한테 밀가루 넣어주기
    int mmin = 3000;
    for (int i = 0; i < n ; i++) {
        mmin = min(mmin, d[i]);
    }
    for (int i = 0; i < n; i++) {
        if (d[i] == mmin)d[i]++;
    }
    // 도우 말아주기   
    // 맨처음에 전처리함

    // 도우 눌러주기
    // 눌러주기전 전처리
    for (int i = 0; i < n; i++) diff[i] = 0;
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j <MAX; j++) {
            if (arr[i][j] == -1)continue;
            int x = j;
            int y = i;
            // 4방향과 인터렉션
            for (int k = 0; k < 4; k++) {
                int ty = i + dy[k];
                int tx = j + dx[k];
                if (!is_range(ty, tx))continue;
                int cur = arr[y][x];
                int nxt = arr[ty][tx];
                
                int s =abs( d[cur] - d[nxt]);
                s /= 5;
                if (d[cur] < d[nxt]) {
                    diff[cur] += s;
                    diff[nxt] -= s;
                }
            }
        }
    }
    for (int i = 0; i < n; i++) {
        d[i] += diff[i];
    }
    // 도우 펴기
    int ti = 0;
    vector<int>cd(n);
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++) {
            if (arr[j][i] == -1)break;
            cd[ti] = d[arr[j][i]];
            ti++;
        }
    }
    for (int i = 0; i < n; i++) {
        d[i] = cd[i];
    }
    // 도우 두번 반
    for (int i = 0; i < n; i++)diff[i] = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < n/4; j++) {
            int x = j;
            int y = i;
            for (int k = 0; k < 4; k++) {
                int tx = x + dx[k];
                int ty = y + dy[k];
                if (!is_range2(ty, tx))continue;
                int cur = arr2[y][x];
                int nxt = arr2[ty][tx];
                int s = abs(d[cur] - d[nxt]);
                s /= 5;
                if (d[cur] < d[nxt]) {
                    diff[cur] += s;
                    diff[nxt] -= s;
                }

            }
        }
    }
    for (int i = 0; i < n; i++) {
        d[i] += diff[i];
    }
    ti = 0;
    for (int i = 24; i >=0; i--) {
        for (int j = 0; j < 4; j++) {
            if (arr2[j][i] == -1)break;
            cd[ti] = d[arr2[j][i]];
            ti++;
        }
    }
    for (int i = 0; i < n; i++) {
        d[i] = cd[i];
    }
}
// 끝내도 되는지 check
bool check_break() {
    int mmin = 3000;
    int mmax = 0;
    for (int i = 0; i < n; i++) {
        mmin = min(mmin, d[i]);
        mmax = max(mmax, d[i]);
    }
    if (mmax - mmin <= k)return true;
    else return false;
}
//dfasdfasdf////asdfasdfasdf///asdfasdfasdf///asdfasdfasdf
int main() {
    // 여기에 코드를 작성해주세요.
    cin >> n >> k;
    arr = vector<vector<int>>(MAX, vector<int>(MAX));
    arr2 = vector<vector<int>>(4, vector<int>(25));
    diff = vector<int>(n);
    d = vector<int>(n);
    for (int i = 0; i < n; i++)cin >> d[i];

    vector<int>tv;
    tv.push_back(1);
    for (int i = 0; i <= 10; i++) {
        tv.push_back(i + 1);
        tv.push_back(i + 1);
    }
    int sum = 0;
    from = 1;
    while (1) {
        sum += tv[from];
        if (n - sum < tv[from + 1]) {
            from--;
            break;
        }
        from++;
    }
    sum = 0;
    for (int i = 1; i <= from; i++) {
        cut.push_back(tv[i]);
        sum += tv[i];
    }
    cut.push_back(n - sum);
    // 도우말기 전처리
    for (int i = 0; i < MAX; i++) {
        for (int j = 0; j < MAX; j++)arr[i][j] = -1;
    }
    int l = cut[cut.size() - 1];
    int ti = n - 1;
    for (int i = l-1; i >=0; i--) {
        arr[0][i] = ti;
        ti--;
    }
    int x = 0;
    int y = 0;
    for (int i = 0; i < from; i++) {
        for (int j = 0; j < cut[from - i -1 ]; j++) {
            y += dy[i % 4];
            x += dx[i % 4];
            arr[y][x] = ti;
            ti--;
        }
    }
    // 도우말기 끝
    // 도우 반 접기 처리
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 25; j++) {
            arr2[i][j] = -1;
        }
    }
    int r = n / 4;
    ti = n - 1;
    for (int i = 0; i < r; i++) {
        arr2[0][i] = ti;
        ti--;
    }
    ti = 0;
    for (int i = 0; i < r; i++) {
        arr2[1][i] = ti;
        ti++;
    }
    ti = n / 2 - 1;
    for (int i = 0; i < r; i++) {
        arr2[2][i] = ti;
        ti--;
    }
    ti = n / 2;
    for (int i = 0; i < r; i++) {
        arr2[3][i] = ti;
        ti++;
    }

    
    int count = 0;
    while (!check_break()) {
        count++;
        do_pizza();
    }

    cout << count;
    return 0;
}