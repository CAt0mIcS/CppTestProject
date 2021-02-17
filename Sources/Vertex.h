#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <algorithm>

#include "Utils/RAssert.h"


namespace At0::VulkanTesting
{
	class VertexLayout
	{
	public:
		enum ElementType
		{
			Position2D,
			Position3D,
			Texture2D,
			Normal,
			Tangent,
			Bitangent,
			Float3Color,
			Float4Color,
			BGRAColor,
			Count,
		};
		template<ElementType, typename FAKE = void>
		struct Map;
		template<typename FAKE>
		struct Map<Position2D, FAKE>
		{
			using SysType = glm::vec2;
			static constexpr const char* code = "P2";
		};
		template<typename FAKE>
		struct Map<Position3D, FAKE>
		{
			using SysType = glm::vec3;
			static constexpr const char* code = "P3";
		};
		template<typename FAKE>
		struct Map<Texture2D, FAKE>
		{
			using SysType = glm::vec2;
			static constexpr const char* code = "T2";
		};
		template<typename FAKE>
		struct Map<Normal, FAKE>
		{
			using SysType = glm::vec3;
			static constexpr const char* code = "N";
		};
		template<typename FAKE>
		struct Map<Tangent, FAKE>
		{
			using SysType = glm::vec3;
			static constexpr const char* code = "Nt";
		};
		template<typename FAKE>
		struct Map<Bitangent, FAKE>
		{
			using SysType = glm::vec3;
			static constexpr const char* code = "Nb";
		};
		template<typename FAKE>
		struct Map<Float3Color, FAKE>
		{
			using SysType = glm::vec3;
			static constexpr const char* code = "C3";
		};
		template<typename FAKE>
		struct Map<Float4Color, FAKE>
		{
			using SysType = glm::vec4;
			static constexpr const char* code = "C4";
		};
		// template<typename FAKE>
		// struct Map<BGRAColor, FAKE>
		//{
		//	using SysType = ::BGRAColor;
		//	static constexpr const char* code = "C8";
		//};

		VkFormat GetFormat(ElementType type) const
		{
			switch (type)
			{
			case Position2D: return VK_FORMAT_R32G32_SFLOAT;
			case Position3D: return VK_FORMAT_R32G32B32_SFLOAT;
			// case Texture2D: return ;
			case Normal: return VK_FORMAT_R32G32B32_SFLOAT;
			// case Tangent: return ;
			// case Bitangent: return ;
			case Float3Color: return VK_FORMAT_R32G32B32_SFLOAT;
			case Float4Color: return VK_FORMAT_R32G32B32A32_SFLOAT;
			case Count: return VK_FORMAT_UNDEFINED;
			}

			RAY_ASSERT(false, "Missing type implementation");
			return VK_FORMAT_UNDEFINED;
		}

		VkVertexInputBindingDescription GetBindingDescription(uint32_t binding = 0) const
		{
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = binding;
			bindingDescription.stride = Size();
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}

		std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions(
			std::vector<uint32_t> locations = {}, uint32_t binding = 0) const
		{
			std::vector<VkVertexInputAttributeDescription> attributeDescs{};
			attributeDescs.resize(elements.size());

			if (locations.empty())
			{
				for (uint32_t i = 0; i < attributeDescs.size(); ++i)
					locations.emplace_back(i);
			}

			for (uint32_t i = 0; i < elements.size(); ++i)
			{
				attributeDescs[i].binding = binding;
				attributeDescs[i].location = locations[i];
				attributeDescs[i].format = GetFormat(elements[i].GetType());
				attributeDescs[i].offset = elements[i].GetOffset();
			}

			return attributeDescs;
		}

		class Element
		{
		public:
			inline Element(ElementType type, size_t offset);
			inline size_t GetOffsetAfter() const;
			inline size_t GetOffset() const;
			inline size_t Size() const;
			inline static constexpr size_t SizeOf(ElementType type);
			inline ElementType GetType() const noexcept;
			inline const char* GetCode() const noexcept;


		private:
			ElementType type;
			size_t offset;
		};

	public:
		template<ElementType Type>
		const Element& Resolve() const
		{
			for (auto& e : elements)
			{
				if (e.GetType() == Type)
				{
					return e;
				}
			}
			assert("Could not resolve element type" && false);
			return elements.front();
		}
		inline const Element& ResolveByIndex(size_t i) const;

		template<typename... Args>
		VertexLayout& Append(Args&&... type)
		{
			(elements.emplace_back(type, Size()), ...);
			return *this;
		}
		inline size_t Size() const;
		inline size_t GetElementCount() const noexcept;
		inline std::string GetCode() const;

	private:
		std::vector<Element> elements;
	};

	class Vertex
	{
		friend class VertexInput;

	public:
		template<VertexLayout::ElementType Type>
		auto& Attr()
		{
			auto pAttribute = pData + layout.Resolve<Type>().GetOffset();
			return *reinterpret_cast<typename VertexLayout::Map<Type>::SysType*>(pAttribute);
		}
		template<typename T>
		void SetAttributeByIndex(size_t i, T&& val)
		{
			const auto& element = layout.ResolveByIndex(i);
			auto pAttribute = pData + element.GetOffset();
			switch (element.GetType())
			{
			case VertexLayout::Position2D:
				SetAttribute<VertexLayout::Position2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Position3D:
				SetAttribute<VertexLayout::Position3D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Texture2D:
				SetAttribute<VertexLayout::Texture2D>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Normal:
				SetAttribute<VertexLayout::Normal>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Tangent:
				SetAttribute<VertexLayout::Tangent>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Bitangent:
				SetAttribute<VertexLayout::Bitangent>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float3Color:
				SetAttribute<VertexLayout::Float3Color>(pAttribute, std::forward<T>(val));
				break;
			case VertexLayout::Float4Color:
				SetAttribute<VertexLayout::Float4Color>(pAttribute, std::forward<T>(val));
				break;
			// case VertexLayout::BGRAColor:
			//	SetAttribute<VertexLayout::BGRAColor>(pAttribute, std::forward<T>(val));
			//	break;
			default: assert("Bad element type" && false);
			}
		}

	protected:
		inline Vertex(char* pData, const VertexLayout& layout);

	private:
		// enables parameter pack setting of multiple parameters by element index
		template<typename First, typename... Rest>
		void SetAttributeByIndex(size_t i, First&& first, Rest&&... rest)
		{
			SetAttributeByIndex(i, std::forward<First>(first));
			SetAttributeByIndex(i + 1, std::forward<Rest>(rest)...);
		}
		// helper to reduce code duplication in SetAttributeByIndex
		template<VertexLayout::ElementType DestLayoutType, typename SrcType>
		void SetAttribute(char* pAttribute, SrcType&& val)
		{
			using Dest = typename VertexLayout::Map<DestLayoutType>::SysType;
			if constexpr (std::is_assignable<Dest, SrcType>::value)
			{
				*reinterpret_cast<Dest*>(pAttribute) = val;
			}
			else
			{
				assert("Parameter attribute type mismatch" && false);
			}
		}

	private:
		char* pData = nullptr;
		const VertexLayout& layout;
	};

	class ConstVertex
	{
	public:
		ConstVertex(const Vertex& v);
		template<VertexLayout::ElementType Type>
		const auto& Attr() const
		{
			return const_cast<Vertex&>(vertex).Attr<Type>();
		}

	private:
		Vertex vertex;
	};

	class VertexInput
	{
	public:
		inline VertexInput(VertexLayout layout, size_t size = 0u);
		inline const char* GetData() const;
		inline const VertexLayout& GetLayout() const noexcept;
		inline void Resize(size_t newSize);
		inline size_t Size() const;
		inline size_t SizeBytes() const;
		template<typename... Params>
		void EmplaceBack(Params&&... params)
		{
			assert(sizeof...(params) == layout.GetElementCount() &&
				   "Param count doesn't match number of vertex elements");
			buffer.resize(buffer.size() + layout.Size());
			Back().SetAttributeByIndex(0u, std::forward<Params>(params)...);
		}
		inline Vertex Back();
		inline Vertex Front();
		inline Vertex operator[](size_t i);
		inline ConstVertex Back() const;
		inline ConstVertex Front() const;
		inline ConstVertex operator[](size_t i) const;

	private:
		std::vector<char> buffer;
		VertexLayout layout;
	};


	// VertexLayout
	const VertexLayout::Element& VertexLayout::ResolveByIndex(size_t i) const
	{
		return elements[i];
	}

	size_t VertexLayout::Size() const
	{
		return elements.empty() ? 0u : elements.back().GetOffsetAfter();
	}

	size_t VertexLayout::GetElementCount() const noexcept { return elements.size(); }

	std::string VertexLayout::GetCode() const
	{
		std::string code;
		for (const auto& e : elements)
		{
			code += e.GetCode();
		}
		return code;
	}


	// VertexLayout::Element
	inline VertexLayout::Element::Element(ElementType type, size_t offset)
		: type(type), offset(offset)
	{
	}
	inline size_t VertexLayout::Element::GetOffsetAfter() const { return offset + Size(); }
	inline size_t VertexLayout::Element::GetOffset() const { return offset; }
	inline size_t VertexLayout::Element::Size() const { return SizeOf(type); }
	inline constexpr size_t VertexLayout::Element::SizeOf(ElementType type)
	{
		switch (type)
		{
		case Position2D: return sizeof(Map<Position2D>::SysType);
		case Position3D: return sizeof(Map<Position3D>::SysType);
		case Texture2D: return sizeof(Map<Texture2D>::SysType);
		case Normal: return sizeof(Map<Normal>::SysType);
		case Tangent: return sizeof(Map<Tangent>::SysType);
		case Bitangent: return sizeof(Map<Bitangent>::SysType);
		case Float3Color: return sizeof(Map<Float3Color>::SysType);
		case Float4Color:
			return sizeof(Map<Float4Color>::SysType);
			// case BGRAColor: return sizeof(Map<BGRAColor>::SysType);
		}
		assert("Invalid element type" && false);
		return 0u;
	}
	inline VertexLayout::ElementType VertexLayout::Element::GetType() const noexcept
	{
		return type;
	}
	inline const char* VertexLayout::Element::GetCode() const noexcept
	{
		switch (type)
		{
		case Position2D: return Map<Position2D>::code;
		case Position3D: return Map<Position3D>::code;
		case Texture2D: return Map<Texture2D>::code;
		case Normal: return Map<Normal>::code;
		case Tangent: return Map<Tangent>::code;
		case Bitangent: return Map<Bitangent>::code;
		case Float3Color: return Map<Float3Color>::code;
		case Float4Color:
			return Map<Float4Color>::code;
			// case BGRAColor: return Map<BGRAColor>::code;
		}
		assert("Invalid element type" && false);
		return "Invalid";
	}


	// Vertex
	inline Vertex::Vertex(char* pData, const VertexLayout& layout) : pData(pData), layout(layout)
	{
		assert(pData != nullptr);
	}
	inline ConstVertex::ConstVertex(const Vertex& v) : vertex(v) {}


	// VertexInput
	inline VertexInput::VertexInput(VertexLayout layout, size_t size) : layout(std::move(layout))
	{
		Resize(size);
	}

	inline void VertexInput::Resize(size_t newSize)
	{
		const auto size = Size();
		if (size < newSize)
		{
			buffer.resize(buffer.size() + layout.Size() * (newSize - size));
		}
	}

	inline const char* VertexInput::GetData() const { return buffer.data(); }

	inline const VertexLayout& VertexInput::GetLayout() const noexcept { return layout; }

	inline size_t VertexInput::Size() const { return buffer.size() / layout.Size(); }

	inline size_t VertexInput::SizeBytes() const { return buffer.size(); }

	inline Vertex VertexInput::Back()
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data() + buffer.size() - layout.Size(), layout };
	}

	inline Vertex VertexInput::Front()
	{
		assert(buffer.size() != 0u);
		return Vertex{ buffer.data(), layout };
	}

	inline Vertex VertexInput::operator[](size_t i)
	{
		assert(i < Size());
		return Vertex{ buffer.data() + layout.Size() * i, layout };
	}

	inline ConstVertex VertexInput::Back() const { return const_cast<VertexInput*>(this)->Back(); }

	inline ConstVertex VertexInput::Front() const
	{
		return const_cast<VertexInput*>(this)->Front();
	}

	inline ConstVertex VertexInput::operator[](size_t i) const
	{
		return const_cast<VertexInput&>(*this)[i];
	}


}  // namespace At0::VulkanTesting
