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


// Describes what VertexData::m_Data looks like
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

	size_t SizeBytes() const
	{
		return QueryNextOffset();
	}

	template<ElementType Type>
	size_t QueryOffset() const
	{
		size_t offset = 0;
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
		Element(ElementType type, size_t offset)
			: m_Type(type), m_Offset(offset) {}

		size_t Offset() const
		{
			return m_Offset;
		}

		ElementType Type() const
		{
			return m_Type;
		}

	private:
		size_t m_Offset;
		ElementType m_Type;
	};

private:
	size_t QueryNextOffset() const
	{
		size_t offset = 0;
		for (const Element& elem : m_Elements)
		{
			offset += elem.Offset();
		}

		//return offset + m_Elements.size() > 0 ? SizeOf(m_Elements.back().Type()) : 0;
		if (m_Elements.size() > 0)
			offset += SizeOf(m_Elements.back().Type());

		return offset;
	}

private:
	std::vector<Element> m_Elements;
};


// Basically a view into VertexData::m_Data
class Vertex
{
	friend class VertexData;
public:

	template<VertexLayout::ElementType Type>
	auto& Attribute()
	{
		return *(typename VertexLayout::Map<Type>::SysType*)(m_VertexStart + m_Layout.QueryOffset<Type>());
	}

	template<VertexLayout::ElementType Type>
	auto& Set(const typename VertexLayout::Map<Type>::SysType& data)
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

// Use std::array? and size template?
class VertexData
{
public:
	VertexData(VertexLayout layout, size_t numVertices)
		: m_Layout(std::move(layout))
	{
		m_Data.resize(m_Layout.SizeBytes() * numVertices);
	}

	size_t SizeBytes() const
	{
		return m_Data.size();
	}

	size_t Size() const
	{
		return m_Data.size() / m_Layout.SizeBytes();
	}

	Vertex operator[](size_t id)
	{
		return { m_Data.data() + (m_Layout.SizeBytes() * id), m_Layout };
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

	std::vector<Float3> positions
	{
		{1.0f, 1.0f, 1.0f},
		{0.0f, -1.0f, 1.0f},
		{-1.0f, 0.0f, 0.5f},
		{0.5f, 0.5f, 0.0f}
	};

	std::vector<Float3> normals
	{
		{2.0f, 2.0f, 2.0f},
		{2.0f, -2.0f, 2.0f},
		{-2.0f, 2.0f, 2.5f},
		{2.5f, 2.5f, 2.0f}
	};

	VertexData vData(std::move(layout), positions.size());

	vData[0].Set<VertexLayout::Position3D>(positions[0]);
	vData[0].Set<VertexLayout::Normal>(normals[0]);

	vData[1].Set<VertexLayout::Position3D>(positions[1]);
	vData[1].Set<VertexLayout::Normal>(normals[1]);

	auto& pos = vData[0].Attribute<VertexLayout::Position3D>();
	auto& normal = vData[0].Attribute<VertexLayout::Normal>();

	auto& pos2 = vData[1].Attribute<VertexLayout::Position3D>();
	auto& normal2 = vData[1].Attribute<VertexLayout::Normal>();
}