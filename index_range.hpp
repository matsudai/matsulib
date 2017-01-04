#pragma once
namespace matsulib
{
	template <class> class IndexRange;

	template <class _Index>
	auto index_range(_Index begin, _Index end) -> IndexRange <_Index>
	{
		return IndexRange <_Index>{begin, end};
	}
	template <class _Index>
	auto index_range(_Index range) -> IndexRange <_Index>
	{
		return IndexRange <_Index>{range};
	}
}



template <class _Index>
class matsulib::IndexRange
{
protected:
	_Index _end;
	_Index _index;

public:
	// range = [begin, ..., end)
	IndexRange(_Index begin, _Index end) : _end{ end }, _index{ begin } {}
	// range = [0, ..., range)
	explicit IndexRange(_Index range) : _end{ range }, _index { 0 } {}

public:
	IndexRange() = delete;
	IndexRange(const IndexRange &) = default;
	IndexRange(IndexRange &&) = default;
	IndexRange &operator =(const IndexRange &) = default;
	IndexRange &operator =(IndexRange &&) = default;

public:
	auto operator *() -> _Index { return _index; }
	auto operator !=(IndexRange &) -> bool { return _index < _end; }
	auto operator ++() -> void { ++_index; }

	auto begin() const -> IndexRange { return *this; }
	auto end() const -> IndexRange { return *this; }
};