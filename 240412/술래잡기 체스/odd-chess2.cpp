#include<iostream>
#include<vector>

#define HOSRE_MAX 16
using namespace std;		

int dr[8] = { -1,-1,0,1,1,1,0,-1 };
int dc[8] = { 0,-1,-1,-1,0,1,1,1 };

int result = 0;
int sum = 0;

struct Horse {
	int r, c;
	int direction;
	int live = 1;
};
vector<Horse>h_vec(HOSRE_MAX);

Horse attacker;

// 술래말 위치 도둑말 죽이기, 방향가지기
bool kill_horse() {
	for (int i = 0; i < HOSRE_MAX; i++) {
		if (h_vec[i].live == 0)continue;
		if (h_vec[i].r == attacker.r && h_vec[i].c == attacker.c) {
			h_vec[i].live = 0;
			attacker.direction = h_vec[i].direction;
			sum += i + 1;
			return true;
		}
	}
	return false;
}
// 이자리로 움직일 수 있는지
bool can_range(int r, int c) {
	if (r < 0 || 4 <= r)return false;
	if (c < 0 || 4 <= c)return false;
	if (r == attacker.r && c == attacker.c)return false;

	return true;
}
//이동해서 그위치에 말이 존재하면 바꾸기
void swap_horse(int r, int c, int dir, int num) {
	for (int i = 0; i < HOSRE_MAX; i++) {
		if (h_vec[i].live == 0)continue;
		if (i == num)continue;

		if (r == h_vec[i].r && c == h_vec[i].c) {
			dir = (dir + 4) % 8;// 반대방향으로 바꾸기
			h_vec[i].r += dr[dir];
			h_vec[i].c += dc[dir];
			return;
		}
	}
}
//남아있는 말들 움직이기
void move_horse() {
	for (int i = 0; i < HOSRE_MAX; i++) {
		if (h_vec[i].live == 0)continue;

		int r = h_vec[i].r;
		int c = h_vec[i].c;
		int dir = h_vec[i].direction;

		bool ifdo = 0;
		for (int j = 0; j < 8; j++) {
			int tr = r + dr[dir];
			int tc = c + dc[dir];
			if (can_range(tr, tc)) {
				// 지금말 업데이트
				h_vec[i].direction = dir;
				h_vec[i].r = tr;
				h_vec[i].c = tc;
				// 움직인 위치에 있는 말 업데이트
				swap_horse(tr, tc, dir,i);
				ifdo = 1;
			}
			if (ifdo)break;

			dir++;
			if (dir > 7)dir = 0;
		}
	}
}
//
void dfs() {
	if (kill_horse()==false) {
		return;
	}
	result = max(result, sum);
	move_horse();

	Horse copy_attacker = attacker;
	vector<Horse>copy_h_vec = h_vec;
	int copy_sum = sum;

	int r = attacker.r;
	int c = attacker.c;
	for (int i = 0; i < 4; i++) {
		attacker = copy_attacker;
		h_vec = copy_h_vec;
		sum = copy_sum;

		// 이동후
		r += dr[attacker.direction];
		c += dc[attacker.direction];
		if (can_range(r, c)) {
			attacker.r = r;
			attacker.c = c;
			dfs();
		}
		else break;
	}

}

int main(void) {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	// 격자 인풋 4x4
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			int ta, tb; cin >> ta >> tb;
			h_vec[ta - 1].direction = tb - 1;
			h_vec[ta - 1].r = i;
			h_vec[ta - 1].c = j;
		}
	}

	attacker.r = 0;
	attacker.c = 0;
	dfs();

	cout << result;
	return 0;
}