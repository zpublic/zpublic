#include "buffer.h"
void Buffer::swapBuffer(Buffer& other)
{
    b.swap(other.b);
    std::swap(i, other.i);
}

Buffer::Container::Container(size_type maxCapacity) :
_buf(0),
_size(0),
_capacity(0),
_maxCapacity(maxCapacity)
{
}

Buffer::Container::~Container()
{
	if (0 != _buf)
	{
		free(_buf);
		_buf = 0;
	}
}

void Buffer::Container::swap(Container& other)
{
    std::swap(_buf, other._buf);

    std::swap(_size, other._size);
    std::swap(_capacity, other._capacity);
    std::swap(_shrinkCounter, other._shrinkCounter);
}

void Buffer::Container::clear()
{
	if (0 != _buf)
	{
		free(_buf);
		_buf = 0;
	}
    _size = 0;
    _capacity = 0;
}

void Buffer::Container::reserve(size_type n)
{
    size_type c = _capacity;
    if(n > _capacity)
    {
        _capacity = std::max<size_type>(n, min(2 * _capacity, _maxCapacity));
        _capacity = std::max<size_type>(static_cast<size_type>(240), _capacity);
    }
    else if(n < _capacity)
    {
        _capacity = n;
    }
    else
    {
        return;
    }

    pointer p = reinterpret_cast<pointer>(realloc(_buf, _capacity));
    if(!p)
    {
        _capacity = c; // Restore the previous capacity.
        //SyscallException ex(__FILE__, __LINE__);
        //ex.error = getSystemErrno();
        //throw ex;
    }
    _buf = p;
}
