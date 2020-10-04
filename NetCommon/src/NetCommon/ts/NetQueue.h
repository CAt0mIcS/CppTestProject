#pragma once

#include "../NetCommon.h"


namespace net
{
	namespace ts
	{
		template<typename T>
		class Queue
		{
		public:
			/// <summary>
			/// Default constructor for Queue
			/// </summary>
			Queue() = default;
			
			/// <summary>
			/// Deleted copy constructor
			/// </summary>
			/// <param name="other">Is the Queue to copy</param>
			Queue(const Queue<T>& other) = delete;

			/// <summary>
			/// Virtual Deconstructor, clears the queue when called
			/// </summary>
			virtual ~Queue()
			{
				Clear();
			}

			/// <summary>
			/// Gets the first object in the queue
			/// </summary>
			/// <returns>A reference to the first object in the queue</returns>
			const T& Front()
			{
				std::scoped_lock lock(muxQueue);
				return deqQueue.front();
			}

			/// <summary>
			/// Gets the last object in the queue
			/// </summary>
			/// <returns>A reference to the last object in the queue</returns>
			const T& Back()
			{
				std::scoped_lock lock(muxQueue);
				return deqQueue.back();
			}

			/// <summary>
			/// Adds item to the back of the queue
			/// </summary>
			/// <param name="item">Is the item which will be added to the back of the queue</param>
			/// <returns>A reference to the inserted item</returns>
			const T& PushBack(const T& item)
			{
				std::scoped_lock lock(muxQueue);
				deqQueue.push_back(item);
				return item;
			}

			/// <summary>
			/// Adds item to the front of the queue
			/// </summary>
			/// <param name="item">Is the item which will be added to the front of the queue</param>
			/// <returns>A reference to the inserted item</returns>
			const T& PushFront(const T& item)
			{
				std::scoped_lock lock(muxQueue);
				deqQueue.push_front(item);
				return item;
			}

			/// <summary>
			/// Gets the size of the deque
			/// </summary>
			/// <returns>The amount of elements in the container</returns>
			size_t Size() const
			{
				std::scoped_lock lock(muxQueue);
				return deqQueue.size();
			}

			/// <summary>
			/// Clears the queue
			/// </summary>
			void Clear()
			{
				std::scoped_lock lock(muxQueue);
				deqQueue.clear();
			}

			/// <summary>
			/// Removes item from the front of the queue and returns it
			/// </summary>
			/// <returns>The first item of the queue before the pop</returns>
			T PopFront()
			{
				std::scoped_lock lock(muxQueue);
				auto front = std::move(deqQueue.front());
				deqQueue.pop_front();
				return front;
			}

			/// <summary>
			/// Removes item from the back of the queue and returns it
			/// </summary>
			/// <returns>The last item of the queue before the pop</returns>
			T PopBack()
			{
				std::scoped_lock lock(muxQueue);
				auto back = std::move(deqQueue.back());
				deqQueue.pop_back();
				return back;
			}



		protected:
			std::mutex muxQueue;
			std::deque<T> deqQueue;
		};
	}
}
