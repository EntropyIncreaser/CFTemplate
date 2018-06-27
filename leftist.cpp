#include <algorithm>

using namespace std;

const int N, LGN;

template <class T, class Compare = less<T> >
struct Leftist {
	struct Node {
		T k;
		int n, dist;
		Node *ls, *rs;

		static Node* create(const T& x) {
			static Node pool[N * LGN];
			static Node* p = pool;
			p->k = x;
			return p++;
		}
	};

	Compare comp;
	Node* rt;

	Node* merge(const Node* lhs, const Node* rhs) const {
		if (!lhs)
			return const_cast<Node *>(rhs);
		if (!rhs)
			return const_cast<Node *>(lhs);
		Node* ret;
		if (comp(lhs->k, rhs->k)) {
			ret = Node::create(lhs->k);
			ret->rs = lhs->rs;
			ret->ls = merge(lhs->ls, rhs);
		} else {
			ret = Node::create(rhs->k);
			ret->rs = rhs->rs;
			ret->ls = merge(rhs->ls, lhs);
		}
		if (!(ret->ls && (!ret->rs || ret->ls->dist > ret->rs->dist)))
			swap(ret->ls, ret->rs);
		ret->dist = ret->ls ? ret->ls->dist + 1 : 0;
		ret->n = lhs->n + rhs->n;
		return ret;
	}

	Leftist(const Compare& comp = Compare(), Node* rt = NULL) : comp(comp), rt(rt) {}

	Leftist(const T& k, const Compare& comp = Compare()) : rt(Node::create(k)), comp(comp) {}

	bool empty() const { return !rt; }
	int size() const { return rt ? rt->n : 0; }
	T top() const { return rt->k; }

	Leftist merge(const Leftist& rhs) const {
		return Leftist(comp, merge(rt, rhs.rt));
	}

	Leftist insert(const T& k) const {
		return Leftist(comp, merge(rt, Node::create(k)));
	}

	Leftist pop() const {
		return Leftist(comp, merge(rt->ls, rt->rs));
	}
};
