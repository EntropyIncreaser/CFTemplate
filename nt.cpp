/* --- Numeric Template --- */
#include <algorithm>

template <class Num>
struct Larger {
	typedef Num Type;
};

template<> struct Larger<char> { typedef short Type; };

template<> struct Larger<short> { typedef int Type; };

template<> struct Larger<int> { typedef long long Type; };

template<> struct Larger<long long> { typedef __int128 Type; };

template <int P>
struct FermatGroup {
	static constexpr const bool isSpecialized = false;

	inline static constexpr int root() { return 0; }
	inline static constexpr int invRoot() { return 0; }
};

template <class Num>
constexpr Num gcd(Num a, Num b) {
	return b ? gcd(b, a % b) : a;
}

template <class Num>
constexpr void exGcd(Num a, Num b, Num& x, Num& y) {
	if (!b) {
		x = 1;
		y = 0;
		return;
	}
	exGcd(b, a % b, y, x);
	y -= a / b * x;
}

template <class Num, Num P>
constexpr Num inversion(Num a) {
	Num x, y;
	exGcd(a, P, x, y);
	if (x < 0)
		x += P;
	return x;
}

template <class Num, Num P>
constexpr Num mpow(Num a, Num k) {
	Num ret = 1;
	while (k) {
		if (k & 1)
			ret = (typename Larger<Num>::Type)ret * a % P;
		a = (typename Larger<Num>::Type)a * a % P;
		k >>= 1;
	}
	return ret;
};

#define DECLARE_FERMAT_GROUP(P, R) template<> struct FermatGroup<P> { \
                                       static constexpr const bool isSpecialized = true;\
                                       inline static constexpr int root() { return R; } \
                                       inline static constexpr int invRoot() { return inversion<int, P>(R); }\
                                   };

DECLARE_FERMAT_GROUP(998244353, 3)
DECLARE_FERMAT_GROUP(1004535809, 3)

template <class Num, Num P>
void ntt(Num* a, int lgn, bool inv) {
	typedef typename Larger<Num>::Type LNum;
	int n = 1 << lgn;
	{
		int *brev = new int[n];
		brev[0] = 0;
		for (int i = 1; i < n; ++i)
			brev[i] = (brev[i >> 1] >> 1) | ((i & 1) << (lgn - 1));
		for (int i = 0; i < n; ++i)
			printf("%d: %d\n", i, brev[i]);
		for (int i = 0; i < n; ++i)
			if (i < brev[i])
				std::swap(a[i], a[brev[i]]);
		delete[] brev;
	}
	Num rt = inv ? FermatGroup<P>::invRoot() : FermatGroup<P>::root();
	for (int i = 0; i < lgn; ++i) {
		Num g = mpow<Num, P>(rt, (P - 1) >> (i + 1));
		int t = 1 << i;
		for (int j = 0; j < n; j += t << 1) {
			Num w = 1;
			for (int k = j; k < j + t; ++k) {
				Num a0 = a[k], a1 = a[k + t];
				a[k] = (a0 + (LNum)w * a1) % P;
				a[k + t] = (a0 + (LNum)(P - w) * a1) % P;
				w = (LNum)w * g % P;
			}
		}
	}
	if (inv) {
		Num rv = inversion<Num, P>(n);
		for (int i = 0; i < n; ++i)
			a[i] = (LNum)a[i] * rv % P;
	}
}
