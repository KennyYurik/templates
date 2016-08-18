#include <vector>

typedef unsigned int uint;

template<class...Ts>
struct tuple;

template<>
struct tuple<>{
	tuple<>& operator=(tuple<>& src) {
		return *this;
	}
};

template<class T, class...Ts>
struct tuple<T, Ts...> : tuple<Ts...>{
	T field;
	tuple<T, Ts...>(T t, Ts... args) : field(t), tuple<Ts...>(args...) {}

	template<class T1, class...Ts1>
	tuple<T, Ts...>& operator=(tuple<T1, Ts1...>& src) {
		this->field = src.field;
		tuple<Ts...>& base = *this;
		tuple<Ts1...>& base_src = src;
		base = base_src;
		return *this;
	}
};

template<uint k, class Tuple>
struct placeholder;

template<uint k, class T, class...Ts>
struct placeholder<k, tuple<T, Ts...>> {
	typedef typename placeholder<k - 1, tuple<Ts...>>::type type;
};

template<class T, class...Ts>
struct placeholder<0, tuple<T, Ts...>> {
	typedef T type;
};

template<uint k, class...Ts>
struct tget;

template<uint k, class T, class... Ts>
struct tget<k, T, Ts...> {
	static typename placeholder<k, tuple<T, Ts...>>::type& get(tuple<T, Ts...>& t) {
		return tget<k - 1, Ts...>::get(static_cast<tuple<Ts...>&>(t));
	}
};

template<class T, class... Ts>
struct tget<0, T, Ts...> {
	static T& get(tuple<T, Ts...>& t) {
		return t.field;
	}
};

template<uint k, class... Ts>
typename placeholder<k, tuple<Ts...>>::type& get(tuple<Ts...>& t) {
	return tget<k, Ts...>::get(t);
}

template<class... Ts> 
typename tuple<Ts&...> tie(Ts&... ts) {
	return tuple<Ts&...>(ts...);
}

template<typename T>
T add(const T& arg1, const T& arg2) {
	return arg1 + arg2;
}

template<typename T, T _append(const T&, const T&) = add>
struct monoid {
	static const bool defined = false;
	typedef T type;
	static const T zero = T(0);

	static T append(const T& arg1, const T& arg2) {
		return _append(arg1, arg2);
	}

	static T pow(const T& arg, uint power) {
		return arg * power;
	}
};

template<typename T, T _append(const T&, const T&) = add, typename = std::enable_if<monoid<T, _append>::defined>::type>
struct use {
	typedef monoid<T, _append> m;
	// use m::zero and shit
};

int main() {
	tuple<int, double, char> c(1, 1.0, 'b');
	int a;
	double b;
	char g;
	tie(a, b, g) = c;
	//tuple<double, char> ee = c;
	int q = monoid<int>::pow(10, 3);
	int w = monoid<int>::append(10, 3);
	use<int, add, int> t;
	return 0;
}