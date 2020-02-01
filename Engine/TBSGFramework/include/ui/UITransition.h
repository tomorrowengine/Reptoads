#pragma once

#include <functional>
#include "ui/easing.h"

namespace ui
{
	enum class UITransitionLoopMode
	{
		None,
		Loop,
		AutoReverseLoop
	};

	class UITransition
	{
	public:
		UITransition(float start, float end, float duration, std::function<float(float, float, float)> method = UITransition::linear, bool expireOnEnd = false);
		UITransition();

		~UITransition() = default;
		void Reset();
		void Start();
		void Pause();
		void Stop();
		void Expire() { expired = true; }
		void Update(float deltaTime);
		float Get();

		const bool Started() { return started; }
		const bool Expired() { return expired; }

		void SetLoopMode(const UITransitionLoopMode loopMode) { this->loopMode = loopMode; }
		const UITransitionLoopMode GetLoopMode() const { return loopMode; }

		static float linear(float start, float end, float step);
		static float easeInSine(float start, float end, float step);
		static float easeOutSine(float start, float end, float step);
		static float easeInOutSine(float start, float end, float step);
		static float easeInQuad(float start, float end, float step);
		static float easeOutQuad(float start, float end, float step);
		static float easeInOutQuad(float start, float end, float step);
		static float easeInCubic(float start, float end, float step);
		static float easeOutCubic(float start, float end, float step);
		static float easeInOutCubic(float start, float end, float step);
		static float easeInQuart(float start, float end, float step);
		static float easeOutQuart(float start, float end, float step);
		static float easeInOutQuart(float start, float end, float step);
		static float easeInQuint(float start, float end, float step);
		static float easeOutQuint(float start, float end, float step);
		static float easeInOutQuint(float start, float end, float step);
		static float easeInExpo(float start, float end, float step);
		static float easeOutExpo(float start, float end, float step);
		static float easeInOutExpo(float start, float end, float step);
		static float easeInCirc(float start, float end, float step);
		static float easeOutCirc(float start, float end, float step);
		static float easeInOutCirc(float start, float end, float step);
		static float easeInBack(float start, float end, float step);
		static float easeOutBack(float start, float end, float step);
		static float easeInOutBack(float start, float end, float step);
		static float easeInElastic(float start, float end, float step);
		static float easeOutElastic(float start, float end, float step);
		static float easeInOutElastic(float start, float end, float step);
		static float easeInBounce(float start, float end, float step);
		static float easeOutBounce(float start, float end, float step);
		static float easeInOutBounce(float start, float end, float step);

	private:
		float duration{};
		float step{};
		float start{};
		float end{};
		float direction{ 1.0f };

		bool started{};
		bool expireOnEnd{};
		bool expired{};

		std::function<float(float, float, float)> method{};

		UITransitionLoopMode loopMode{};
	};
}


