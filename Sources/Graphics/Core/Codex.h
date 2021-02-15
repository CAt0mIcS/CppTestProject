#pragma once

#include <unordered_map>
#include <memory>

#include "Graphics/Vulkan/Bindable.h"
#include "Utils/RLogger.h"


namespace At0::VulkanTesting
{
	class Codex
	{
	public:
		template<typename T, typename... Args>
		static std::shared_ptr<T> Resolve(Args&&... args)
		{
			static_assert(std::is_base_of<Bindable, T>::value,
				"Template argument must be derived from Bindable");
			return Get().InternalResolve<T>(std::forward<Args>(args)...);
		}

	public:
		// Object needs to be destroyed if the refcount reaches 1
		// This class will check for that
		template<typename T>
		class SharedPointer : public std::shared_ptr<T>
		{
			using Callback = void (*)(SharedPointer<T>& ptr);
			using Base = std::shared_ptr<T>;

		public:
			using Base::Base;

			SharedPointer(const std::shared_ptr<T>& ptr) : Base(ptr) {}
			SharedPointer() {}

			SharedPointer(const SharedPointer<T>& ptr) : Base(ptr) {}

			SharedPointer<T>& operator=(SharedPointer<T>&& right)
			{
				if (this->get() != right.get())
				{
					// if (this->get())
					//	m_DecCallback(*this);

					Base::operator=(std::move(right));
					m_DecCallback = right.m_DecCallback;
				}

				return *this;
			}

			SharedPointer<T>& operator=(const SharedPointer<T>& right)
			{
				if (this->get() != right.get())
				{
					// if (this->get())
					//	m_DecCallback(*this);

					Base::operator=(std::move(right));
					m_DecCallback = right.m_DecCallback;
				}

				return *this;
			}

			~SharedPointer()
			{
				Base::~Base();
				m_DecCallback(*this);
			}

			void Reset()
			{
				Base::reset();

				if (this->get())
					m_DecCallback(*this);
			}

		private:
			Callback m_DecCallback = PointerRefCountDecreasedCallback;
		};

	private:
		static Codex& Get()
		{
			static Codex codex;
			return codex;
		}

		template<typename T, typename... Args>
		std::shared_ptr<T> InternalResolve(Args&&... args)
		{
			std::string uniqueID = T::GetUID(std::forward<Args>(args)...);
			auto it = m_BindableMap.find(uniqueID);

			// Key does not exist, create bindable
			if (it == m_BindableMap.end())
			{
				m_BindableMap[uniqueID] =
					Codex::SharedPointer<T>(std::make_shared<T>(std::forward<Args>(args)...));
				return std::static_pointer_cast<T>(m_BindableMap[uniqueID]);
			}
			// Key exists, return it
			else
			{
				return std::static_pointer_cast<T>(it->second);
			}
		}

		template<typename T>
		static void PointerRefCountDecreasedCallback(Codex::SharedPointer<T>& ptr)
		{
			long useCount = ptr.use_count();
			if (useCount == 1)
				ptr.Reset();
		}

	private:
		std::unordered_map<std::string, Codex::SharedPointer<Bindable>> m_BindableMap;
	};
}  // namespace At0::VulkanTesting
