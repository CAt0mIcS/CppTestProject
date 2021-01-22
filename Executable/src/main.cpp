#include "pch.h"

#include "RMath.h"
using namespace At0::Ray;

// Position2D
// Position3D
// Texture2D
// Normal
// Tangent
// Bitangent
// Float3Color
// Float4Color
// BGRAColor
// Count


// Describes what VertexBuffer::m_Data looks like
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
		//BGRAColor,
		Count,
	};

public:
	VertexLayout()
	{
	}

	template<ElementType Type> struct Map;
	template<> struct Map<Position2D>
	{
		using SysType = Float2;
		static constexpr size_t Size = sizeof(SysType);
	};
	template<> struct Map<Position3D>
	{
		using SysType = Float3;
		static constexpr size_t Size = sizeof(SysType);
	};
	template<> struct Map<Texture2D>
	{
		using SysType = Float2;
		static constexpr size_t Size = sizeof(SysType);
	};
	template<> struct Map<Normal>
	{
		using SysType = Float3;
		static constexpr size_t Size = sizeof(SysType);
	};
	template<> struct Map<Tangent>
	{
		using SysType = Float3;
		static constexpr size_t Size = sizeof(SysType);
	};
	template<> struct Map<Bitangent>
	{
		using SysType = Float3;
		static constexpr size_t Size = sizeof(SysType);
	};
	template<> struct Map<Float3Color>
	{
		using SysType = Float3;
		static constexpr size_t Size = sizeof(SysType);
	};
	template<> struct Map<Float4Color>
	{
		using SysType = Float4;
		static constexpr size_t Size = sizeof(SysType);
	};
	template<> struct Map<Count>
	{
		using SysType = long double;
		static constexpr size_t Size = sizeof(SysType);
	};

	static constexpr uint32_t SizeOf(ElementType type)
	{
		switch (type)
		{
		case Position2D:	return sizeof(Float2);
		case Position3D:	return sizeof(Float3);
		case Texture2D:		return sizeof(Float2);
		case Normal:		return sizeof(Float3);
		case Tangent:		return sizeof(Float3);
		case Bitangent:		return sizeof(Float3);
		case Float3Color:	return sizeof(Float3);
		case Float4Color:	return sizeof(Float4);
		case Count:			return sizeof(long double);
		}

		//RAY_ASSERT(false, "[VertexLayout::SizeOf] Type (ID={0}) is invalid.", (uint32_t)type);
		return 0;
	}

	template<ElementType... Type>
	VertexLayout& Append()
	{
		(m_Elements.emplace_back(Type, QueryNextOffset()), ...);
		return *this;
	}

	uint32_t Size() const
	{
		return QueryNextOffset();
	}

	template<ElementType Type>
	uint32_t QueryOffset() const
	{
		uint32_t offset = 0;
		for (const Element& elem : m_Elements)
		{
			offset += elem.Offset();

			if (elem.Type() == Type)
				break;
		}

		return offset;
	}

public:
	class Element
	{
	public:
		Element(ElementType type, uint32_t offset)
			: m_Type(type), m_Offset(offset) {}

		uint32_t Offset() const
		{
			return m_Offset;
		}

		ElementType Type() const
		{
			return m_Type;
		}

	private:
		uint32_t m_Offset;
		ElementType m_Type;
	};

private:
	uint32_t QueryNextOffset() const
	{
		uint32_t offset = 0;
		for (const Element& elem : m_Elements)
		{
			offset += elem.Offset();
		}

		return offset + m_Elements.size() > 0 ? SizeOf(m_Elements.back().Type()) : 0;
	}

private:
	std::vector<Element> m_Elements;
};


// Basically a view into VertexBuffer::m_Data
class Vertex
{
	friend class VertexBuffer;
public:

	template<VertexLayout::ElementType Type>
	auto& Attribute()
	{
		return *(typename VertexLayout::Map<Type>::SysType*)(m_VertexStart + m_Layout.QueryOffset<Type>());
	}

	template<VertexLayout::ElementType Type>
	auto& Set(typename VertexLayout::Map<Type>::SysType&& data)
	{
		using SysType = typename VertexLayout::Map<Type>::SysType;

		SysType& type = *(SysType*)(m_VertexStart + m_Layout.QueryOffset<Type>());
		type = data;
		return type;
	}

private:
	Vertex(char* vertexStart, const VertexLayout& layout)
		: m_VertexStart(vertexStart), m_Layout(layout) {}

private:
	char* m_VertexStart;
	const VertexLayout& m_Layout;
};


class VertexBuffer
{
public:
	VertexBuffer(VertexLayout layout, uint32_t numVertices = 1)
		: m_Layout(std::move(layout))
	{
		m_Data.resize(m_Layout.Size() * numVertices);
	}

	Vertex operator[](size_t id)
	{
		return { m_Data.data() + id, m_Layout };
	}

private:
	VertexLayout m_Layout;

	// Holds all the vertex data.
	// Location anf offset for a specific Vertex is stored in the layout
	std::vector<char> m_Data;
};


int main()
{
	VertexLayout layout;
	//layout.Append<VertexLayout::Position3D, VertexLayout::Normal>();
	layout.Append<VertexLayout::Position3D, VertexLayout::Normal>();

	VertexBuffer vBuff(std::move(layout));
	Vertex v = vBuff[0];
	v.Set<VertexLayout::Normal>({ 1.0f, 1.0f, 1.0f });
	v.Set<VertexLayout::Position3D>({ 2.0f, 2.0f, 2.0f });
	Float3& pos = v.Attribute<VertexLayout::Position3D>();
	Float3& normal = v.Attribute<VertexLayout::Normal>();
}