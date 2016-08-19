#include <vector>
#include <algorithm>
#include <limits>

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

// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------

template<typename T>
struct monoid_add {
	typedef T type;
	static const T zero = T(0);

	static T append(const T& a, const T& b) {
		return a + b;
	}

	static T pow(const T& arg, uint power) {
		return arg * power;
	}
};

template<typename T>
using monoid = monoid_add<T>;

template<typename T>
struct monoid_min {
	typedef T type;
	static const T zero;

	static T append(const T& a, const T& b) {
		return std::min(a, b);
	}

	static T pow(const T& arg, uint power) {
		return (power == 0) ? zero : arg;
	}
};

template<typename T>
const T monoid_min<T>::zero = std::numeric_limits<T>::max();

int main() {
	int q = monoid<int>::pow(10, 3);
	double d = monoid_min<double>::zero;
}