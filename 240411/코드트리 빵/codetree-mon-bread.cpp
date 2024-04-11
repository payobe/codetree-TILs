#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>

using namespace std;
#define ll long long
ll dr[4] = { -1,0,0,1 };
ll dc[4] = { 0,-1,1,0 };

ll n, m;
vector<vector<ll>>canPass;
ll turn;

struct Base {
	ll r, c;
	ll can = 1;
	ll dist = 0;
	ll number;
};
vector<Base>baseVec;

struct People {
	ll r, c;
	ll pr, pc;
	ll isFinish;
};
vector<People>peopleVec;

ll findPath(ll who) {
	People tp = peopleVec[who];
	vector<vector<ll>>tv = vector<vector<ll>>(n + 1, vector<ll>(n + 1));
	for (ll i = 1; i <= n; i++)
		for (ll j = 1; j <= n; j++)
			tv[i][j] = -1;
	
	queue<pair<ll, ll>>que;
	que.push({ tp.pr,tp.pc });
	tv[tp.pr][tp.pc] = 4;
	while (!que.empty()) {
		pair<ll, ll>tpll = que.front();
		que.pop();

		ll r = tpll.first;
		ll c = tpll.second;

		
		ll trg = 0;
		for (ll i = 3; i >= 0; i--) {
			ll tr = r + dr[i];
			ll tc = c + dc[i];

			if (tr <= 0 || n < tr || tc <= 0 || n < tc)continue;

			if (tr == tp.r && tc == tp.c) {
				tv[tr][tc] = i;
				trg = 1;
				break;
			}
			if (canPass[tr][tc] == 0)continue;
			if (tv[tr][tc] != -1)continue;

			tv[tr][tc] = i;
			que.push({ tr,tc });
		}
		if (trg == 1)break;
	}
	ll ret = tv[tp.r][tp.c];
	ret = 3 - ret;
	return ret;
}

void goMove() {
	for (ll i = 1; i <= m; i++) {
		if (peopleVec[i].isFinish == 1)continue;
		if (i >= turn)continue;
		ll direction = findPath(i);
		peopleVec[i].r += dr[direction];
		peopleVec[i].c += dc[direction];
	}
}

void checkArrive() {// done
	for (ll i = 1; i <= m; i++) {
		if (peopleVec[i].isFinish == 1)continue;
		if (i >= turn)continue;

		People tp = peopleVec[i];
		if (tp.r == tp.pr && tp.c == tp.pc) {
			peopleVec[i].isFinish = 1;
			canPass[tp.r][tp.c] = 0;
		}
	}
}

ll findDistance(ll who) {
	Base tbase = baseVec[who];
	People tp = peopleVec[turn];
	vector<vector<ll>>tv = vector<vector<ll>>(n + 1, vector<ll>(n + 1));
	for (ll i = 1; i <= n; i++)
		for (ll j = 1; j <= n; j++)
			tv[i][j] = -1;

	queue<pair<ll, ll>>que;
	que.push({ tp.pr,tp.pc });
	tv[tp.pr][tp.pc] = 0;
	while (!que.empty()) {
		pair<ll, ll>tpll = que.front();
		que.pop();

		ll r = tpll.first;
		ll c = tpll.second;
		ll dist = tv[r][c];

		if (r == tbase.r && c == tbase.c)break;

		for (ll i = 3; i >= 0; i--) {
			ll tr = r + dr[i];
			ll tc = c + dc[i];

			if (tr <= 0 || n < tr || tc <= 0 || n < tc)continue;
			if (canPass[tr][tc] == 0)continue;
			if (tv[tr][tc] != -1)continue;

			tv[tr][tc] = dist+1;
			que.push({ tr,tc });
		}
	}
	return tv[tbase.r][tbase.c];
}

bool cmp(Base a, Base b) {
	if (a.dist < b.dist)return true;
	if (a.dist > b.dist)return false;

	if (a.r < b.r)return true;
	if (a.r > b.r)return false;

	if (a.c < b.c)return true;
	if (a.c > b.c)return false;
}

void inBase() {
	if (turn > m)return;
	People tpeo = peopleVec[turn];
	ll sr = tpeo.pr; ll sc = tpeo.pc;
	ll bs = baseVec.size();

	vector<Base>forSort;
	for (ll i = 0; i < bs; i++) {
		Base tbase = baseVec[i];
		if (tbase.can == 0)continue;
		ll dist = findDistance(i);
		tbase.dist = dist;
		forSort.push_back(tbase);
	}
	sort(forSort.begin(), forSort.end(), cmp);

	Base result = forSort[0];
	baseVec[result.number].can = 0;
	peopleVec[turn].r = result.r;
	peopleVec[turn].c = result.c;
	canPass[result.r][result.c] = 0;
}

void start() {
	goMove();
	checkArrive();
	inBase();
}


int main(void) {
	ios_base::sync_with_stdio(0), cin.tie(0), cout.tie(0);
	cin >> n >> m;
	canPass = vector<vector<ll>>(n+1, vector<ll>(n+1));
	peopleVec = vector<People>(m + 1);
	for (ll i = 1; i <= n; i++) {
		for (ll j = 1; j <= n; j++) {
			canPass[i][j] = 1;
			ll ta; cin >> ta;
			if (ta) {
				Base tb;
				tb.r = i;
				tb.c = j;
				tb.can = 1;
				tb.number = baseVec.size();
				baseVec.push_back(tb);
			}
		}
	}
	for (ll i = 1; i <= m; i++) {
		ll ta, tb; cin >> ta >> tb;
		People tp;
		tp.r = -1;
		tp.c = -1;
		tp.pr = ta;
		tp.pc = tb;
		tp.isFinish = 0;
		peopleVec[i]= tp;
	}

	turn = 0;
	for (ll i = 1; i <= m; i++) {
		turn++;
		start();
	}

	while (1) {
		turn++;

		start();

		bool trg = 1;
		for (ll i = 1; i <= m; i++) {
			if (peopleVec[i].isFinish == 0)trg = 0;
		}
		if (trg == 0)continue;
		break;
	}
	cout << turn;
	return 0;
}