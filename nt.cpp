template <class Num>
Num gcd(Num a, Num b) {
	return b ? gcd(b, a % b) : a;
}

template <class Num>
void exGcd(Num a, Num b, Num& x, Num& y) {
	if (!b) {
		x = 1;
		y = 0;
		return;
	}
	exGcd(b, a % b, y, x);
	y -= a / b * x;
}

template <class Num, Num P>
Num inversion(Num a) {
	Num x, y;
	exGcd(a, P, x, y);
	if (x < 0)
		x += P;
	return x;
}
