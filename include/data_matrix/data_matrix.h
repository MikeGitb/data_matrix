#pragma once

#include <cassert>
#include <tuple>
#include <type_traits>
#include <vector>

namespace mba {

inline const char* get_hello()
{
	return "data_matrix says hello!";
}

template<class T>
struct column {
	T* _begin;
	T* _end;

	template<class C, class = std::enable_if_t<!std::is_const_v<T>, C>>
	explicit column( C& c )
		: _begin( c.data() )
		, _end( c.data() + c.size() )
	{
	}

	template<class C, class = std::enable_if_t<std::is_const_v<T>, C>>
	explicit column( const C& c )
		: _begin( c.data() )
		, _end( c.data() + c.size() )
	{
	}

	T*             begin() const { return _begin; }
	T*             end() const { return _end; }
	std::ptrdiff_t size() const { return _end - _begin; }
	T&             operator[]( int idx ) const { return *( _begin + idx ); }
};

template<class... RowTypesT>
class data_matrix {
	using self_t = data_matrix<RowTypesT...>;
	template<int idx>
	using element_type = std::tuple_element_t<idx, std::tuple<RowTypesT...>>;

	static constexpr std::index_sequence_for<RowTypesT...> indexes{};

public:
	using idx_t = std::ptrdiff_t;

	using row_t       = std::tuple<RowTypesT&...>;
	using const_row_t = std::tuple<const RowTypesT&...>;

	template<idx_t i>
	using column_t = column<element_type<i>>;

	template<idx_t i>
	using const_column_t = column<const element_type<i>>;

	template<bool IsConst>
	using row_type = std::conditional_t<IsConst, const_row_t, row_t>;

private:
	template<size_t... I>
	void push_back_impl( std::index_sequence<I...>, const RowTypesT&... e )
	{
		auto t = std::make_tuple( e... );
		( std::get<I>( rows ).push_back( std::get<I>( t ) ), ... );
	}

	template<size_t... I>
	row_t get_row( std::index_sequence<I...>, idx_t i )
	{
		return {std::get<I>( rows )[i]...};
	}

	template<size_t... I>
	const_row_t get_row( std::index_sequence<I...>, idx_t i ) const
	{
		return {std::get<I>( rows )[i]...};
	}

	template<size_t... I>
	void resize_impl( std::index_sequence<I...>, idx_t i )
	{
		( std::get<I>( rows ).resize( i ), ... );
	}

	template<class C>
	void erase_impl_single( C& c, idx_t start, idx_t end )
	{
		c.erase( c.begin() + start, c.begin() + end );
	}

	template<size_t... I>
	void erase_impl( std::index_sequence<I...>, idx_t start, idx_t end )
	{
		( erase_impl_single( std::get<I>( rows ), start, end ), ... );
	}

	using storarge_t = std::tuple<std::vector<RowTypesT>...>;
	storarge_t rows;

public:
	template<bool IsConst>
	struct iterator_t {
		std::conditional_t<IsConst, const self_t*, self_t*> matrix;

		std::ptrdiff_t idx;

		row_type<IsConst> operator*() { return ( *matrix )[idx]; }

		operator iterator_t<true>() const { return {matrix, idx}; }

		// clang-format off
		iterator_t& operator++()		{ idx++; return *this; }
		iterator_t& operator++( int )	{ idx++; return {matrix, idx - 1}; }

		iterator_t& operator+=( idx_t i ) {	idx += i; return *this;	}
		iterator_t& operator-=( idx_t i ) { idx -= i; return *this;	}

		friend idx_t operator-( iterator_t l, iterator_t r ) { return l.idx - r.idx; }
		friend iterator_t operator+( iterator_t it, idx_t idx ) { return {it.matrix,it.idx + idx}; }
		friend iterator_t operator+( idx_t idx, iterator_t it ) { return {it.matrix, it.idx + idx}; }

		friend bool operator==( iterator_t left, iterator_t right )	{ assert( left.matrix == right.matrix ); return left.idx == right.idx;	}
		friend bool operator!=( iterator_t left, iterator_t right ) { assert( left.matrix == right.matrix ); return left.idx != right.idx;	}
		friend bool operator< ( iterator_t left, iterator_t right )	{ assert( left.matrix == right.matrix ); return left.idx <  right.idx;	}
		friend bool operator> ( iterator_t left, iterator_t right )	{ assert( left.matrix == right.matrix ); return left.idx >  right.idx;	}
		friend bool operator<=( iterator_t left, iterator_t right )	{ assert( left.matrix == right.matrix ); return left.idx <= right.idx;	}
		friend bool operator>=( iterator_t left, iterator_t right )	{ assert( left.matrix == right.matrix ); return left.idx >= right.idx;	}
		// clang-format on
	};

	using iterator       = iterator_t<false>;
	using const_iterator = iterator_t<true>;

	row_t       operator[]( idx_t i ) { return get_row( indexes, i ); };
	const_row_t operator[]( idx_t i ) const { return get_row( indexes, i ); };

	template<int idx>
	auto get()
	{
		return column_t<idx>{std::get<idx>( rows )};
	}

	template<int idx>
	auto get() const
	{
		return const_column_t<idx>{std::get<idx>( rows )};
	}

	idx_t size() const { return this->template get<0>().size(); }
	void  resize( idx_t i ) {}

	iterator       begin() { return {this, 0}; }
	iterator       end() { return {this, size()}; }
	const_iterator begin() const { return {this, 0}; }
	const_iterator end() const { return {this, size()}; }

	iterator erase( const_iterator start, const_iterator end )
	{
		erase_impl( indexes, start.idx, end.idx );
		return {this, start.idx};
	}

	void push_back( const RowTypesT&... e ) { push_back_impl( indexes, e... ); }
};

} // namespace mba
