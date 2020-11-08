#include "rlpch.h"
#include "Reyal/Layers/LayerStack.h"

#include "Reyal/Layers/Layer.h"
#include <RlDebug/Instrumentor.h>


namespace At0::Reyal
{
	void LayerStack::PushLayer(_In_ Layer* layer)
	{
		RL_PROFILE_FUNCTION();
		m_Layers.emplace_back(layer);
	}

	void LayerStack::PopLayer(_In_ Layer* layer)
	{
		RL_PROFILE_FUNCTION();

		auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			delete layer;
			m_Layers.erase(it);
		}
	}
}


