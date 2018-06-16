#include <cstdio>

#include <algorithm>
#include <functional>
#include <queue>

using namespace std;

const int N;

template <class T, class Compare = less<T> >
struct FibHeap {
	struct Node {
		T k;
		int deg;
		bool mark;
		Node *prv, *nxt, *prt, *ch;

		static Node* create(const T& x) {
			static Node pool[N];
			static Node* p = pool;
			p->k = x;
			return p++;
		}
	};

	int n;
	Compare comp;
	Node *min;

	FibHeap(const Compare& comp = Compare()) : n(), comp(comp), min() {}

	int size() const { return n; }
	T top() const { return min->k; }

	Node* insert(const T& x) {
		++n;
		Node* p = Node::create(x);
		if (!min) {
			min = p;
			p->nxt = p->prv = p;
		} else {
			p->prv = min;
			p->nxt = min->nxt;
			p->prv->nxt = p->nxt->prv = p;
			if (comp(p->k, min->k))
				min = p;
		}
		return p;
	}

	void merge(FibHeap& h) {
		n += h.n;
		if (!min) {
			min = h.min;
			h.min = NULL;
			return;
		}
		if (!h.min)
			return;
		h.min->prv = min;
		h.min->nxt = min->nxt;
		h.min->prv->nxt = h.min->nxt->prv = h.min;
		if (comp(h.min->k, min->k))
			min = h.min;
		h.min = NULL;
	}

	/* TODO consolidate, decrease-key */
	void link(Node* y, Node* x) {
		
	}

	void consolidate() {

	}

	T pop() {
		--n;
		Node* z = min;
		if (z->ch) {
			Node* p = z->ch->nxt;
			while (p != z->ch) {
				p->prv->nxt = p->nxt;
				p->nxt->prv = p->prv;
				p->prt = NULL;
				p->nxt = min->nxt;
				p->prv = min->prv;
				p->nxt->prv = p->prv->nxt = p;
				p = z->ch->nxt;
			}
			p->prt = NULL;
			p->nxt = min->nxt;
			p->prv = min->prv;
			p->nxt->prv = p->prv->nxt = p;
			z->ch = NULL;
		}
		z->prv->nxt = z->nxt;
		z->nxt->prv = z->prv;
		if (z->nxt == z)
			min = NULL;
		else {
			min = z->nxt;
			consolidate();
		}
		return z->k;
	}
};