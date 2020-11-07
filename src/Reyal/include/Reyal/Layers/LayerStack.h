#ifndef RL_LAYERSTACK_H
#define RL_LAYERSTACK_H

#include "Reyal/RlBase.h"
#include <vector>

// 'LayerStack::m_Layers': class 'std::vector<Reyal::Layer*>' needs to have dll-interface to be used by clients of class 'Reyal::LayerStack'
#pragma warning(disable : 4251)


namespace At0::Reyal
{
	class Layer;

	class RL_API LayerStack
	{
		using Iterator = std::vector<Layer*>::iterator;
		using ReverseIterator = std::vector<Layer*>::reverse_iterator;
		using ConstInterator = std::vector<Layer*>::const_iterator;
		using ConstReverseIterator = std::vector<Layer*>::const_reverse_iterator;
	public:
		/// <summary>
		/// Default LayerStack Constructor
		/// </summary>
		LayerStack() = default;

		/// <summary>
		/// Pushes a layer onto the stack
		/// </summary>
		/// <param name="layer">Is a raw, (heap-allocated) pointer to the layer</param>
		/// <returns>The pushed layer</returns>
		void PushLayer(_In_ Layer* layer);
		
		/// <summary>
		/// Removes a layer from the stack
		/// </summary>
		/// <param name="layer">Is the layer to remove</param>
		/// <returns>The removed layer</returns>
		void PopLayer(_In_ Layer* layer);

		Iterator begin() { return m_Layers.begin(); }
		Iterator end() { return m_Layers.end(); }
		ReverseIterator rbegin() { return m_Layers.rbegin(); }
		ReverseIterator rend() { return m_Layers.rend(); }

		ConstInterator cbegin() { return m_Layers.cbegin(); }
		ConstInterator cend() { return m_Layers.cend(); }
		ConstReverseIterator crbegin() { return m_Layers.crbegin(); }
		ConstReverseIterator crend() { return m_Layers.crend(); }

	private:
		std::vector<Layer*> m_Layers;
	};
}


#endif // RL_LAYERSTACK_H
