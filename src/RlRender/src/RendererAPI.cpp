#include "rlrpch.h"
#include "RendererAPI.h"

#include "RlRender/Platform/D3D11/D3D11RendererAPI.h"

#include <RlDebug/RlAssert.h>


namespace At0::Reyal
{
	RendererAPI::API RendererAPI::s_Api = RendererAPI::API::D3D11;

	std::unique_ptr<RendererAPI> RendererAPI::Create()
	{
		RL_EXPECTS(s_Api >= API::FIRST && s_Api <= API::LAST);

		switch (GetAPI())
		{
		case API::D3D11:			return std::make_unique<D3D11RendererAPI>();
		}

		RL_ASSERT(false, "Unknown Renderer API");
		return nullptr;
	}
}


