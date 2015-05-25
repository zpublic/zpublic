#pragma once

#include <mutex>
#include <condition_variable>

#include <windows.h>

namespace Util
{
	class Readwrite_Lock
	{
	public:
		Readwrite_Lock()
			: stat(0)
		{
		}

		void ReadLock()
		{
			while (stat < 0)
			{
				std::unique_lock<std::mutex> lock(mtx);			// 写操作存在时阻塞
				cond.wait(lock);
			}
			InterlockedIncrement(reinterpret_cast<ULONG*>(&stat));
		}

		void ReadUnlock()
		{
			InterlockedDecrement(reinterpret_cast<ULONG*>(&stat));
			if (stat == 0)
				cond.notify_one();								// 叫醒一个等待的写操作
		}

		void WriteLock()
		{
			while (stat != 0)									// 读或写操作存在时都阻塞
			{
				std::unique_lock<std::mutex> lock(mtx);
				cond.wait(lock);
			}
			InterlockedExchange(reinterpret_cast<ULONG*>(&stat), (ULONG)-1);
		}

		void WriteUnlock()
		{
			InterlockedExchange(reinterpret_cast<ULONG*>(&stat), (0));
			cond.notify_all(); // 叫醒所有等待的读和写操作
		}

	private:
		std::mutex mtx;
		std::condition_variable cond;
		int stat; // == 0 无锁；> 0 已加读锁个数；< 0 已加写锁
	};
}
