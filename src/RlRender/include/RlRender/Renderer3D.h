#ifndef RLR_RENDERER3D_H
#define RLR_RENDERER3D_H

#include "RlRBase.h"
#include "RendererAPI.h"

#pragma warning(disable : 4251)


namespace At0::Reyal
{
	class RLR_API Renderer3D
	{
	public:
		/// <summary>
		/// Renderer3D Constructor
		/// </summary>
		Renderer3D();

		/// <summary>
		/// Initializes Renderer aand RendererAPI
		/// </summary>
		/// <param name="window">Is the platform specific window to render to</param>
		void Init(_In_ WindowHandle window);

		/// <summary>
		/// Checks if the renderer was initialized
		/// </summary>
		/// <returns>True if initialized, false otherwise</returns>
		bool IsInitialized() const { return m_API != nullptr; }

		/// <summary>
		/// DEBUG: TODO: Renders test triangle onto the screen
		/// </summary>
		void RenderTestTriangle();

		/// <summary>
		/// Draws the scene to the screen
		/// </summary>
		void EndDraw();

	private:
		/// <summary>
		/// Rendering calls will be dispatched to the platform's API
		/// </summary>
		std::unique_ptr<RendererAPI> m_API;
	};

}


#endif // RLR_RENDERER3D_H