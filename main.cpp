#include <vector>

typedef unsigned int uint;

template<class...Ts>
struct tuple;

template<>
struct tuple<>{
};

template<class T, class...Ts>
struct tuple<T, Ts...> : tuple<Ts...>{
	T field;
	tuple<T, Ts...>(T t, Ts... args) : field(t), tuple<Ts...>(args...) {}
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

int main() {
	tuple<int, char, double, std::vector<int>> t(1, 'a', 1.0, std::vector<int>(10));
	placeholder<0, tuple<int, char, double>>::type a = 2;
	placeholder<1, tuple<int, char, double>>::type b = 'a';
	placeholder<2, tuple<int, char, double>>::type c = 2.0;

	get<0>(t) = 20;
	get<1>(t) = '3';
	get<2>(t) = 34.23;
	get<3>(t) = std::vector<int>(100, 4);
	return 0;
}