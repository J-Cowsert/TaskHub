#pragma once

namespace taskhub {

	class Layer {
	public:
		virtual ~Layer() = default;

		virtual void OnAttach() {}
		virtual void OnDetach() {}

		virtual void OnUpdate(/*Todo: DeltaTime*/) {}
		virtual void OnUIRender() {}
	};
}