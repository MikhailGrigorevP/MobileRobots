#pragma once
#ifndef _VECTOR_H_
#define _VECTOR_H_
#include <initializer_list>
#include <stdexcept>
#include <memory>

//! Vector namespace
/*! My realization of <vector> from stl 
*/
namespace my_std {


	/*!Iterators */

	template<class T>
	class const_Iterator : public std::iterator<std::random_access_iterator_tag, T>
	{
	protected:
		typename const_Iterator::pointer p;
	public:
		const_Iterator(typename const_Iterator::pointer p) : p(p) {}
		const_Iterator() : p(nullptr) {}
		const_Iterator(const const_Iterator& other) : p(other.p) {}
		const_Iterator(const_Iterator&& other) noexcept : p(other.p) { other.p = nullptr; }

		const_Iterator& operator=(const const_Iterator& other) { this->p = other.p; return *this; }

		bool operator != (const const_Iterator& other) const { return !(p == other.p); }
		bool operator == (const const_Iterator& other) const { return p == other.p; }	

		const typename const_Iterator::reference operator*  () const { if (p) return *p; throw std::exception("pointer is unitializated"); }

		const_Iterator& operator++ () { ++p; return *this; }
	};

	template<class T>
	class Iterator : public const_Iterator<T>
	{
	public:
		Iterator(typename Iterator::pointer p) : const_Iterator<T>(p) {}
		Iterator() : const_Iterator<T>() {}
		Iterator(const Iterator& other) : const_Iterator<T>(other) {}
		Iterator(Iterator&& other) noexcept : const_Iterator<T>(other) {}

		Iterator& operator=(typename Iterator::pointer p) { this->p = p; return *this; }
		Iterator& operator=(const Iterator& other) { this->p = other.p; return *this; }
		Iterator& operator=(Iterator&& other) noexcept { this->p = other.p; other.p = nullptr; return *this; }

		ptrdiff_t operator-(const Iterator& other) const { return this->p - other.p; }

		bool operator != (const Iterator& other) const { return !(this->p == other.p); }
		bool operator == (const Iterator& other) const { return this->p == other.p; }

		typename Iterator::reference operator*  () { if (this->p) return *this->p; throw std::exception("pointer is unitializated"); }

		Iterator& operator++ () { ++this->p; return *this; }
		Iterator  operator++ (int) { Iterator it(this->p); ++this->p; return it; }
	};

	/*!Class */
	template<class T, class Allocator = std::allocator<T>>
	class vector
	{
	private:

		const size_t _additional_size = 4096 / sizeof(T);
		size_t _capacity = _additional_size;
		size_t _size = 0;
		T* _array = nullptr;
		Allocator _allocator;
		typedef std::allocator_traits<Allocator> allocator_t;

		size_t capacity_of_size(size_t size) { return (size / _additional_size + 1) * _additional_size; }
	
		void destroy_array(T*& _array, size_t& _constructed_count) {
			for (size_t i = 0; i < _constructed_count; ++i) {
				allocator_t::destroy(_allocator, _array + i);
			}
			_constructed_count = 0;
		}
	
		void destroy_current_array() { destroy_array(_array, _size); }
		
		void deallocate_array(T*& _array, const size_t& _capacity) {
			if (_array) {
				allocator_t::deallocate(_allocator, _array, _capacity);
				_array = nullptr;
			}
		}
	
		void deallocate_current_array() {
			deallocate_array(_array, _capacity);
			_capacity = _additional_size;
		}
		
		void delete_current_array() {
			destroy_current_array();
			deallocate_current_array();
		}
	
	public:

		// Constructors
		vector() {
			try {
				_array = allocator_t::allocate(_allocator, _capacity);
			}
			catch (const std::bad_alloc & ex) {
				throw std::bad_alloc(ex);
			}
		}
		vector(const std::initializer_list<T>& list) {
			size_t list_size = list.size();

			if (list_size >= _capacity) {
				_capacity = capacity_of_size(list_size);
			}

			try {
				_array = allocator_t::allocate(_allocator, _capacity);
				for (auto&& t : list) {
					allocator_t::construct(_allocator, _array + _size, t);
					++_size;
				}
			}
			catch (const std::exception & _ex) {
				throw std::exception(_ex);
			}
		}

		vector(const vector& other) :
			_capacity(other._capacity),
			_size(other._size)
		{
			try {
				_array = allocator_t::allocate(_allocator, _capacity);
				for (size_t i = 0; i < _size; ++i) {
					allocator_t::construct(_allocator, _array + i, other[i]);
				}
			}
			catch (const std::exception & _ex) {
				throw std::exception(_ex);
			}
		}

		vector(vector&& other) noexcept :
			_size(other._size),
			_capacity(other._capacity),
			_array(other._array)
		{
			other._array = nullptr;
		}

		// Destructor
		~vector() {
			delete_current_array();
		}

		// operator =
		vector& operator=(const vector& other) {
			if (&other == this) {
				return *this;
			}

			T* new_array = nullptr;
			size_t new_size = other._size;
			size_t new_capacity = capacity_of_size(new_size);
			size_t constructed_count = 0;
			try {
				new_array = allocator_t::allocate(_allocator, new_capacity);
				for (size_t i = 0; i < new_size; ++i) {
					allocator_t::construct(_allocator, new_array + i, other[i]);
					++constructed_count;
				}
				delete_current_array();
				_capacity = new_capacity;
				_array = new_array;
				_size = new_size;
			}
			catch (const std::exception & _ex) {
				if (new_array) {
					destroy_array(new_array, constructed_count);
					deallocate_array(new_array, new_capacity);
				}
				throw std::exception(_ex);
			}
			return *this;
		}
		vector& operator=(vector&& other) noexcept {
			if (&other == this) {
				return *this;
			}

			delete_current_array();

			_size = other._size;
			_capacity = other._capacity;
			_array = other._array;
			other._array = nullptr;
			return *this;
		}

		// Elements access
		T& at(size_t index) {
			if (!(index < _size)) throw std::out_of_range("Array index is out of range");
			return *(_array + index);
		}
		const T& at(size_t index) const {
			if (!(index < _size)) throw std::out_of_range("Array index is out of range");
			return *(_array + index);
		}
		T& operator[](size_t index) {
			return *(_array + index);
		}
		const T& operator[](size_t index) const {
			return *(_array + index);
		}


		using const_iterator = const_Iterator<T>;
		using iterator = Iterator<T>;

		// Iterators
		iterator                begin()        noexcept { return iterator(_array); }
		const_iterator          begin()  const noexcept { return const_iterator(_array); }

		iterator                end()        noexcept { return iterator(_array + _size); }
		const_iterator          end()  const noexcept { return const_iterator(_array + _size); }

		size_t size()  const noexcept { return _size; }

		// Modificators
		void clear() noexcept { 
			destroy_current_array();
			_size = 0;
		}

		void erase(const size_t& index) {
			if (!(index < _size)) throw std::out_of_range("Array index is out of range");
			for (size_t i = index; i < _size - 1; ++i) {
				_array[i] = std::move(_array[i + 1]);
			}
			--_size;
			allocator_t::destroy(_allocator, _array + _size);
		}

		void push_back(const T& value) {
			emplace_back(value);
		}
		void push_back(T&& value) {
			emplace_back(std::move(value));
		}

		template<class... Args>
		void emplace_back(Args&&... args) {
			try {
				if (_size + 1 > _capacity) {
					size_t new_capacity;

					if(_size<100)
						new_capacity = _capacity + _additional_size;
					else
						new_capacity = _capacity * 2;

					T* new_array = allocator_t::allocate(_allocator, new_capacity);

					for (size_t i = 0; i < _size; ++i) {
						allocator_t::construct(_allocator, new_array + i, _array[i]);
						allocator_t::destroy(_allocator, _array + i);
					}
					allocator_t::deallocate(_allocator, _array, _capacity);
					_capacity = new_capacity;
					_array = new_array;
				}
				allocator_t::construct(_allocator, _array + _size, args...);
				++_size;
			}
			catch (const std::exception & ex)
			{
				throw std::exception(ex);
			}
		}

		template<class T, class Allocator = std::allocator<T>>
		friend bool operator == (const vector& _Left, const vector& _Right);

		template<class T, class Allocator = std::allocator<T>>
		friend bool operator != (const vector& _Left, const vector& _Right);
	};

	/*!Friend operators */
	template<class T, class Allocator = std::allocator<T>>
	inline bool operator==(const typename vector<T, Allocator>& _Left, const typename vector<T, Allocator>& _Right)
	{
		if (_Left.size() != _Right.size()) {
			return false;
		}

		typename vector < T, Allocator>::const_iterator left_begin = _Left.begin();
		typename vector < T, Allocator>::const_iterator left_end = _Left.end();
		typename vector < T, Allocator>::const_iterator right_begin = _Right.begin();

		while (left_begin != left_end)
		{
			if ((*left_begin) != (*right_begin)) {
				return false;
			}
			++left_begin;
			++right_begin;
		}

		return true;
	}

	template<class T, class Allocator = std::allocator<T>>
	inline bool operator!=(const typename vector<T, Allocator>& _Left, const typename vector<T, Allocator>& _Right)
	{
		if (_Left.size() == _Right.size()) {
			return false;
		}

		typename vector < T, Allocator>::const_iterator left_begin = _Left.begin();
		typename vector < T, Allocator>::const_iterator left_end = _Left.end();
		typename vector < T, Allocator>::const_iterator right_begin = _Right.begin();

		while (left_begin != left_end)
		{
			if ((*left_begin) == (*right_begin)) {
				return false;
			}
			++left_begin;
			++right_begin;
		}

		return true;
	}

}

#endif

