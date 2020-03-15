#pragma once

#include <set>
#include <vector>
#include "RNG.h"

template <typename T>
struct element
{
	T t;
	mutable int idx;

	element(T t, int idx = 0) : t(t), idx(idx) {};

	bool operator< (const element& e) const
	{
		return t < e.t;
	}

	bool operator== (const element& e) const
	{
		return t == e.t;
	}
};

template <typename T>
class Container
{
	std::set<element<T>> s;
	std::vector<T> v;

public:

	const T& operator[](int idx) const;
	bool insert(const T& t);
	bool erase(const T& t);
	bool EraseAt(int i);
	int find(const T& t);
	bool present(const T& t);
    const T& GetRandom();
	T PopRandom();
	int size() const;
	bool empty() const;
	bool clear();
};

// ------------------------ definitions are provided in the same file to avoid some linking errors.

template <typename T>
bool Container<T>::erase(const T& t)
{
	auto it = s.find(t);

	// if the element is not found or the set is empty, return false.
	if (it == s.end())
		return false;

	auto tmp = s.find(v.back());
	tmp->idx = it->idx;
	std::swap(v[it->idx], v.back());
	
	v.pop_back();
	s.erase(it);

	return true;
}

template <typename T>
bool Container<T>::insert(const T& t)
{
	bool ElementInserted = s.insert(element<T>(t, (int)v.size())).second;

	if (ElementInserted)
		v.push_back(t);

	return ElementInserted;
}

template <typename T>
const T& Container<T>::operator[](int idx) const
{
	return v[idx];
}

template <typename T>
bool Container<T>::EraseAt(int i)
{
	return erase(v[i]);
}

template <typename T>
int Container<T>::find(const T& t)
{
	return (*s.find(t)).idx;
}

template <typename T>
bool Container<T>::present(const T& t)
{
	return s.find(t) != s.end();
}

template <typename T>
const T& Container<T>::GetRandom()
{

#if NO_RANDOMIZATION
	return (*s.begin()).t; // to disable the random behavior.
#endif

    return v[RNG::GetRandomNumber(v.size())];
}

template <typename T>
T Container<T>::PopRandom()
{
	T ret = GetRandom();
	erase(ret);
	return ret;
}

template <typename T>
int Container<T>::size() const
{
	return s.size();
}

template <typename T>
bool Container<T>::empty() const
{
	return s.empty();
}

template <typename T>
bool Container<T>::clear()
{
	s.clear();
	v.clear();
}

//--------------------------------------------------------------------


template <typename T>
class LiteContainer
{
	std::vector<T> v;

public:

    LiteContainer(std::vector<T> v) : v(v) {}
	LiteContainer(std::set<T> s)
	{
		for (auto i : s)
			v.push_back(i);

#if NO_RANDOMIZATION
		// optional.
		std::reverse(v.begin(), v.end());
#endif

	}
		
	T& operator[] (int idx) const
	{
		return v[idx];
	}

	T PopAt(int idx)
	{

#if NO_RANDOMIZATION
		T tt = v.back();
		v.pop_back();
		return tt;
#endif

		T t = v[idx];

		std::swap(v[idx], v.back());
		v.pop_back();

		return t;
	}

	T PopRandom()
	{
        return PopAt(RNG::GetRandomNumber(v.size()));
	}

	int size() const
	{
		return v.size();
	}

	bool empty() const
	{
		return v.empty();
	}
};
