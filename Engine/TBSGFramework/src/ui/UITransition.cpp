#include "ui/UITransition.h"

#include <algorithm>

namespace ui
{
	float UITransition::linear(float start, float end, float step)
	{
		return start + step * (end - start);
	};

	float UITransition::easeInSine(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInSine(static_cast<double>(step))));
	};

	float UITransition::easeOutSine(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeOutSine(static_cast<double>(step))));
	};

	float UITransition::easeInOutSine(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInOutSine(static_cast<double>(step))));
	};

	float UITransition::easeInQuad(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInQuad(static_cast<double>(step))));
	};

	float UITransition::easeOutQuad(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeOutQuad(static_cast<double>(step))));
	};

	float UITransition::easeInOutQuad(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInOutQuad(static_cast<double>(step))));
	};

	float UITransition::easeInCubic(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInCubic(static_cast<double>(step))));
	};

	float UITransition::easeOutCubic(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeOutCubic(static_cast<double>(step))));
	};

	float UITransition::easeInOutCubic(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInOutCubic(static_cast<double>(step))));
	};

	float UITransition::easeInQuart(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInQuart(static_cast<double>(step))));
	};

	float UITransition::easeOutQuart(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeOutQuart(static_cast<double>(step))));
	};

	float UITransition::easeInOutQuart(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInOutQuart(static_cast<double>(step))));
	};

	float UITransition::easeInQuint(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInQuint(static_cast<double>(step))));
	};

	float UITransition::easeOutQuint(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeOutQuint(static_cast<double>(step))));
	};

	float UITransition::easeInOutQuint(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInOutQuint(static_cast<double>(step))));
	};

	float UITransition::easeInExpo(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInExpo(static_cast<double>(step))));
	};

	float UITransition::easeOutExpo(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeOutExpo(static_cast<double>(step))));
	};

	float UITransition::easeInOutExpo(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInOutExpo(static_cast<double>(step))));
	};

	float UITransition::easeInCirc(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInCirc(static_cast<double>(step))));
	};

	float UITransition::easeOutCirc(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeOutCirc(static_cast<double>(step))));
	};

	float UITransition::easeInOutCirc(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInOutCirc(static_cast<double>(step))));
	};

	float UITransition::easeInBack(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInBack(static_cast<double>(step))));
	};

	float UITransition::easeOutBack(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeOutBack(static_cast<double>(step))));
	};

	float UITransition::easeInOutBack(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInOutBack(static_cast<double>(step))));
	};

	float UITransition::easeInElastic(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInElastic(static_cast<double>(step))));
	};

	float UITransition::easeOutElastic(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeOutElastic(static_cast<double>(step))));
	};

	float UITransition::easeInOutElastic(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInOutElastic(static_cast<double>(step))));
	};

	float UITransition::easeInBounce(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInBounce(static_cast<double>(step))));
	};

	float UITransition::easeOutBounce(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeOutBounce(static_cast<double>(step))));
	};

	float UITransition::easeInOutBounce(float start, float end, float step)
	{
		return UITransition::linear(start, end, static_cast<float>(easing::easeInOutBounce(static_cast<double>(step))));
	};

	UITransition::UITransition(float start, float end, float duration, std::function<float(float, float, float)> method, bool expireOnEnd)
	{
		this->start = start;
		this->end = end;
		this->duration = duration;
		this->method = method;
		this->expireOnEnd = expireOnEnd;
	}

	UITransition::UITransition()
	{
		this->start = 0.0f;
		this->end = 1.0f;
		this->duration = 1.0f;
		this->method = UITransition::linear;
	}

	void UITransition::Reset()
	{
		step = 0.0f;
	}

	void UITransition::Start()
	{
		started = true;
	}

	void UITransition::Pause()
	{
		started = false;
	}

	void UITransition::Stop()
	{
		Pause();
		Reset();
	}

	void UITransition::Update(float deltaTime)
	{
		if (started)
		{
			step = (std::max)(0.0f, (std::min)(duration, step + deltaTime * direction));

			if ((step >= duration && direction > 0) || (step <= 0.0f && direction < 0))
			{
				switch (loopMode)
				{
				case UITransitionLoopMode::None: {
					Pause();
					if (expireOnEnd)
					{
						expired = true;
					}
				} break;
				case UITransitionLoopMode::Loop: {
					step = 0.0f;
				} break;
				case UITransitionLoopMode::AutoReverseLoop: {
					direction *= -1.0f;
				} break;
				}
			}
		}
	}

	float UITransition::Get()
	{
		return method(start, end, 1.0f / duration * step);
	}
}
