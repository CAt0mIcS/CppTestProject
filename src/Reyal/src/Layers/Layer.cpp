#include "rlpch.h"
#include "Layer.h"

#include <RlDebug/Instrumentor.h>


namespace At0::Reyal
{
    Layer::Layer(const std::wstring_view name)
        : m_Name(name)
    {
        RL_PROFILE_FUNCTION();

    }
}

