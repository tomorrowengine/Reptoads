#pragma once

#include "memory/smart_ptr.h"
#include "UIElement.h"

namespace ui {
	class UISystemInterface {
	public:
		virtual void OnElementPressed(ptl::weak_ptr<UIElement> a_element) {};
		virtual void OnElementReleased(ptl::weak_ptr<UIElement> a_element) {};
		virtual void OnElementFocussed(ptl::weak_ptr<UIElement> a_element) {};
		virtual void OnElementFocusLost(ptl::weak_ptr<UIElement> a_element) {};
		virtual void OnEvent(ptl::weak_ptr<UIElement> a_element, UIEvent a_event) {};
	};
}