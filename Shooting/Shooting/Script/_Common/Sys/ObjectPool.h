#pragma once

#include <algorithm>
#include <cstddef>
#include <functional>
#include <limits>
#include <memory>
#include <unordered_set>
#include <utility>
#include <vector>

template <typename T>
class ObjectPool
{
public:
	using Factory = std::function<std::unique_ptr<T>()>;

	ObjectPool(Factory factory, std::size_t initialSize = 5, std::size_t growthSize = 5,
		std::size_t maxSize = (std::numeric_limits<std::size_t>::max)())
		: _factory(std::move(factory)), _growthSize(growthSize ? growthSize : 1), _maxSize(maxSize)
	{
		Grow((std::min)(initialSize, _maxSize));
	}
	ObjectPool(const ObjectPool&) = delete;
	ObjectPool& operator=(const ObjectPool&) = delete;
	ObjectPool(ObjectPool&&) noexcept = default;
	ObjectPool& operator=(ObjectPool&&) noexcept = default;

	T* Get()
	{
		if (_available.empty() && _owned.size() < _maxSize)
			Grow((std::min)(_growthSize, _maxSize - _owned.size()));
		if (_available.empty()) return nullptr;

		T* object = _available.back();
		_available.pop_back();
		_inUse.insert(object);
		return object;
	}

	void Return(T* object)
	{
		if (object && _inUse.erase(object)) _available.push_back(object);
	}

	std::vector<T*>& GetAll() { return _all; }
	const std::vector<T*>& GetAll() const { return _all; }

private:
	Factory _factory;
	std::size_t _growthSize;
	std::size_t _maxSize;
	std::vector<std::unique_ptr<T>> _owned;
	std::vector<T*> _all;
	std::vector<T*> _available;
	std::unordered_set<T*> _inUse;

	void Grow(std::size_t count)
	{
		count = (std::min)(count, _maxSize - _owned.size());
		for (std::size_t i = 0; i < count; ++i)
		{
			auto object = _factory();
			if (!object) continue;
			T* raw = object.get();
			_owned.push_back(std::move(object));
			_all.push_back(raw);
			_available.push_back(raw);
		}
	}
};
