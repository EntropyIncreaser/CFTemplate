/* Convex Hull Maintaining by Splay */
#include <cmath>
#include <cstdio>

#include <algorithm>

#define LOG(FMT...) fprintf(stderr, FMT)

using namespace std;

typedef double Num;
typedef double BNum;

struct Vec {
	Num x, y;

	Vec() : x(), y() {}

	Vec(Num x, Num y) : x(x), y(y) {}

	bool operator<(const Vec &rhs) const {
		return x < rhs.x;
	}

	Vec operator+(const Vec& rhs) const { return Vec(x + rhs.x, y + rhs.y); }
	Vec operator-(const Vec& rhs) const { return Vec(x - rhs.x, y - rhs.y); }

	BNum operator*(const Vec& rhs) const { return x * (BNum) rhs.y - y * (BNum) rhs.x; }
	BNum operator^(const Vec& rhs) const { return x * (BNum) rhs.x + y * (BNum) rhs.y; }
};

struct Node {
	Vec v, prv, nxt;
	union {
		struct {
			Node *ls, *rs;
		};
		Node* ch[2];
	};
	Node* prt;

	inline bool rel() const {
		return this == prt->rs;
	}

	inline void rotate() {
		Node* p = prt;
		bool f = rel();
		prt = p->prt;
		if (prt)
			prt->ch[p->rel()] = this;
		p->ch[f] = ch[!f];
		if (ch[!f])
			ch[!f]->prt = p;
		ch[!f] = p;
		p->prt = this;
	}

	inline void splay(Node* goal) {
		while (prt != goal) {
			if (prt->prt == goal)
				return rotate();
			if (rel() == prt->rel()) {
				prt->rotate();
				rotate();
			} else {
				rotate();
				rotate();
			}
		}
	}
};

struct Convex {
	Node* t;

	void ins(const Vec& v);

	BNum bound(const Vec& v) {
		Vec x = Vec(v.y, -v.x);
		while (true) {
			if (t->prv * x > 0)
				t = t->ls;
			else if (t->nxt * x < 0)
				t = t->rs;
			else
				break;
		}
		t->splay(NULL);
		return t->v ^ v;
	}
};

const int N;

Node* create() {
	static Node pool[N];
	static Node* p = pool;
	return p++;
}

void Convex::ins(const Vec &v) {
	if (t == NULL) {
		t = create();
		t->v = v;
		return;
	}
	Node *p = t, *tmp = t;
	t = NULL;
	while (p) {
		if (p->v.x < v.x) {
			t = p;
			p = p->rs;
		} else
			p = p->ls;
	}
	if (t) {
		t->splay(NULL);
		if (!t->rs) {
			p = create();
			p->v = v;
			p->ls = t;
			t->prt = p;
			t = p;
		} else {
			p = t->rs;
			while (p->ls)
				p = p->ls;
			p->splay(t);
			if ((v - t->v) * (p->v - v) >= 0)
				return;
			p = create();
			p->v = v;
			p->prt = t;
			p->rs = t->rs;
			t->rs = p;
			p->rs->prt = p;
			t = p;
			t->splay(NULL);
		}
	} else {
		t = tmp;
		p = create();
		p->v = v;
		p->rs = t;
		t->prt = p;
		t = p;
	}
	Node* q;
	if (t->ls) {
		p = t->ls;
		while (p->ls)
			p = p->ls;
		p->splay(t);
		q = p->rs;
		while (q) {
			if (q->prv * (v - q->v) < 0) {
				p = q;
				q = q->rs;
			} else
				q = q->ls;
		}
		p->splay(t);
		p->rs = NULL;
		p->nxt = t->prv = v - p->v;
	}
	if (t->rs) {
		p = t->rs;
		while (p->rs)
			p = p->rs;
		p->splay(t);
		q = p->ls;
		while (q) {
			if ((q->v - v) * q->nxt < 0) {
				p = q;
				q = q->ls;
			} else
				q = q->rs;
		}
		p->splay(t);
		p->ls = NULL;
		t->nxt = p->prv = p->v - v;
	}
}
