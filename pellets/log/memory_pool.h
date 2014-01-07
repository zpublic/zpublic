#ifndef _MEMORY_POOL_H
#define _MEMORY_POOL_H

#include <stdexcept>
#include <stdlib.h>
#include <memory.h>
#include <iostream>
typedef unsigned int uint32_t;

class MemoryPool
{
	union obj
	{
		union obj * free_list_link;
		char  buffer[1024];
	};
	union wobj
	{
		union wobj * free_list_link;
		wchar_t  buffer[1024];
	};

	static const uint32_t kDefaultSize = 0x00000080;  //128  
	static const uint32_t kIncreaseSize = 0x00000400; //1024

	public:
	explicit MemoryPool( uint32_t chunk_size = kDefaultSize, uint32_t increase_size = kIncreaseSize )
		:_chunk_size( chunk_size ),
		_increase_size( increase_size ),
		_free_obj_list(NULL),
		_wc_free_obj_list(NULL)
	{
		if( _chunk_size < 0 )
		{
			throw std::invalid_argument("invalid chunk size");
		}

		allocate( _chunk_size );
		wcallocate( _chunk_size );
	}

	~MemoryPool()
	{
		obj *temp;
		while( _free_obj_list != NULL )
		{
			temp = _free_obj_list;
			_free_obj_list = _free_obj_list->free_list_link;

			free( temp );
		}

		wobj *wtemp;
		while( _wc_free_obj_list != NULL )
		{
			wtemp = _wc_free_obj_list;
			_wc_free_obj_list = _wc_free_obj_list->free_list_link;

   			free( wtemp );
		}
	}

	public:
	char * acquire()
	{
		char * result = static_cast<char *>( memset( get_free(), 0, sizeof(obj) ) );
		return result;
	}

	wchar_t * wcacquire()
	{
		wchar_t * result = wmemset( wc_get_free(), 0, sizeof(wobj)/sizeof(wchar_t) );
		return result;
	}

	void release( void *p )
	{
		deallocate( p );
	}

	void wcrelease( wchar_t *p )
	{
		wcdeallocate( p );
	}

	private:
	char * get_free()
	{
		if( _free_obj_list == NULL )
		{
			allocate( _increase_size );
		}

		obj * result = _free_obj_list;
		_free_obj_list = result->free_list_link;

		return result->buffer;
	}

	wchar_t * wc_get_free()
	{
		if( _wc_free_obj_list == NULL )
		{
			wcallocate( _increase_size );
		}

		wobj * result = _wc_free_obj_list;
		_wc_free_obj_list = result->free_list_link;

		return result->buffer;
	}

	void deallocate( void *p )
	{
		obj *q = static_cast<obj *>( p );

		q->free_list_link = _free_obj_list;
		_free_obj_list = q;
	} 

	void wcdeallocate( wchar_t *p )
	{
		wobj *q = reinterpret_cast<wobj *>( p );

		q->free_list_link = _wc_free_obj_list;
		_wc_free_obj_list = q;
	}

	void allocate( size_t size )
	{
		obj  *next_obj = NULL;
		for( size_t i =0; i < size; ++i )
		{
			obj *current_obj = static_cast<obj *>( malloc( sizeof(obj) ) );
			if( current_obj == NULL )
			{
				std::cout<<"malloc failed"<<std::endl;
				continue;
			}

			if( _free_obj_list == NULL )
			{
				_free_obj_list = next_obj = current_obj;
			}
			else
			{
				next_obj->free_list_link = current_obj;
				next_obj = next_obj->free_list_link;
			}
		}
		next_obj->free_list_link = NULL;
	}

	void wcallocate( size_t size )
	{
		wobj  *next_obj = NULL;
		for( size_t i =0; i < size; ++i )
		{
			wobj *current_obj = static_cast<wobj *>( malloc( sizeof(wobj) ) );
			if( current_obj == NULL )
			{
				std::cout<<"malloc failed"<<std::endl;
				continue;
			}

			if( _wc_free_obj_list == NULL )
			{
				_wc_free_obj_list = next_obj = current_obj;
			}
			else
			{
				next_obj->free_list_link = current_obj;
				next_obj = next_obj->free_list_link;
			}
		}
		next_obj->free_list_link = NULL;
	}

	private:
	uint32_t  _chunk_size;
	uint32_t  _increase_size;
	obj *     _free_obj_list;  
	wobj*     _wc_free_obj_list;
};
#endif
