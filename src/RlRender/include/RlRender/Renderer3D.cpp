#include "rlrpch.h"
#include "RlRender/Renderer3D.h"

#include <RlDebug/RlAssert.h>


namespace At0::Reyal
{
    Renderer3D::Renderer3D()
        : m_API(nullptr)
    {

    }

    void Renderer3D::Init(_In_ WindowHandle window)
    {
        RL_MEXPECTS(!this->IsInitialized(), "Renderer3D was already initialized.");

        m_API = RendererAPI::Create();
        m_API->Init(window);
    }

    void Renderer3D::RenderTestTriangle()
    {
        RL_MEXPECTS(this->IsInitialized(), "Renderer3D was not initialized.");
        m_API->RenderTestTriangle();
    }
    
    void Renderer3D::EndDraw()
    {
        RL_MEXPECTS(this->IsInitialized(), "Renderer3D was not initialized.");
        m_API->EndDraw();
    }
}

