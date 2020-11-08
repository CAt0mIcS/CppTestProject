#ifndef RL_QUEUE_H
#define RL_QUEUE_H

#include "Reyal/RlBase.h"

#include <deque>
#include <mutex>
#include <functional>


namespace At0::Reyal
{
	/// <summary>
	/// Thread-Safe Queue
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T, size_t MaxSize = -1>
	class RL_API Queue
	{
	public:
		using Iterator = typename std::deque<T>::iterator;
		using ReverseIterator = typename std::deque<T>::reverse_iterator;
		using ConstInterator = typename std::deque<T>::const_iterator;
		using ConstReverseIterator = typename std::deque<T>::const_reverse_iterator;
	public:
		/// <summary>
		/// Default Queue Constructor
		/// </summary>
		Queue() = default;

		/// <summary>
		/// Adds an element to the front of the queue
		/// </summary>
		/// <param name="elem">Is the element to add</param>
		void PushFront(T&& elem)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push_front(std::move(elem));
			TrimBuffer();
			m_Condition.notify_all();
		}

		/// <summary>
		/// Adds an element to the back of the queue
		/// </summary>
		/// <param name="elem">Is the element to add</param>
		void PushBack(T&& elem)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push_back(std::move(elem));
			TrimBuffer();
			m_Condition.notify_all();
		}

		/// <summary>
		/// Adds an element to the front of the queue
		/// </summary>
		/// <param name="elem">Is the element to add</param>
		void PushFront(const T& elem)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push_front(std::move(elem));
			TrimBuffer();
			m_Condition.notify_all();
		}

		/// <summary>
		/// Adds an element to the back of the queue
		/// </summary>
		/// <param name="elem">Is the element to add</param>
		void PushBack(const T& elem)
		{
			std::scoped_lock lock(m_Mutex);
			m_Queue.push_back(std::move(elem));
			TrimBuffer();
			m_Condition.notify_all();
		}

		/// <summary>
		/// Removes the first element in the queue and returns it
		/// </summary>
		/// <returns>The first element in the queue</returns>
		T PopFront()
		{
			std::scoped_lock lock(m_Mutex);
			auto front = std::move(m_Queue.front());
			m_Queue.pop_front();
			return front;
		}

		/// <summary>
		/// Removes the last element in the queue and returns it
		/// </summary>
		/// <returns>The last element in the queue</returns>
		T PopBack()
		{
			std::scoped_lock lock(m_Mutex);
			auto back = std::move(m_Queue.back());
			m_Queue.pop_back();
			return back;
		}

		/// <summary>
		/// Getter for the first element in the queue
		/// </summary>
		/// <returns>A reference to the first element</returns>
		T& Front()
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.front();
		}

		/// <summary>
		/// Getter for the last element in the queue
		/// </summary>
		/// <returns>A reference to the last element</returns>
		T& Back()
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.back();
		}

		/// <summary>
		/// Checks if the queue is empty
		/// </summary>
		/// <returns>True if it's empty, false otherwise</returns>
		bool Empty() const
		{
			return m_Queue.empty();
		}

		/// <summary>
		/// Getter for the size of the queue
		/// </summary>
		/// <returns>The size of the queue</returns>
		size_t Size() const
		{
			return m_Queue.size();
		}

		/// <summary>
		/// Function will wait until the predicate returns true
		/// </summary>
		/// <typeparam name="F">Is any callable object without paraeters returning bool</typeparam>
		/// <param name="pred"></param>
		template<typename F, typename = std::enable_if_t<std::is_convertible_v<F, std::function<bool()>>>>
		void WaitFor(F&& pred)
		{
			std::unique_lock lock(m_Mutex);
			m_Condition.wait(lock, pred);
		}

		/// <summary>
		/// Gets the current condition variable used for waiting in WaitFor
		/// </summary>
		/// <returns>The condition variable</returns>
		std::condition_variable& GetWaiter()
		{
			return m_Condition;
		}

		Iterator begin()
		{
			std::scoped_lock lock(m_Mutex); 
			return m_Queue.begin();
		}

		Iterator end() 
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.end();
		}
		ReverseIterator rbegin() 
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.rbegin();
		}

		ReverseIterator rend() 
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.rend();
		}

		ConstInterator cbegin() 
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.cbegin();
		}
		ConstInterator cend() 
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.cend();
		}

		ConstReverseIterator crbegin() 
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.crbegin();
		}

		ConstReverseIterator crend() 
		{
			std::scoped_lock lock(m_Mutex);
			return m_Queue.crend();
		}

	private:
		/// <summary>
		/// Trims the queue down to the maximum allowed size, mutex is already locked at this stage
		/// </summary>
		void TrimBuffer()
		{
			while (m_Queue.size() > MaxSize)
			{
				m_Queue.pop_front();
			}
		}

	private:
		/// <summary>
		/// The queue which holds the data
		/// </summary>
		std::deque<T> m_Queue;

		/// <summary>
		/// Mutex for m_Queue
		/// </summary>
		std::mutex m_Mutex;

		/// <summary>
		/// Will wait for a predicate to return true
		/// </summary>
		std::condition_variable m_Condition;
	};
}


#endif // RL_QUEUE_H

