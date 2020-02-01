#pragma once 



namespace easing
{
	inline double easeInSine(double t);
	inline double easeOutSine(double t);
	inline double easeInOutSine(double t);
	inline double easeInQuad(double t);
	inline double easeOutQuad(double t);
	inline double easeInOutQuad(double t);
	inline double easeInCubic(double t);
	inline double easeOutCubic(double t);
	inline double easeInOutCubic(double t);
	inline double easeInQuart(double t);
	inline double easeOutQuart(double t);
	inline double easeInOutQuart(double t);
	inline double easeInQuint(double t);
	inline double easeOutQuint(double t);
	inline double easeInOutQuint(double t);
	inline double easeInExpo(double t);
	inline double easeOutExpo(double t);
	inline double easeInOutExpo(double t);
	inline double easeInCirc(double t);
	inline double easeOutCirc(double t);
	inline double easeInOutCirc(double t);
	inline double easeInBack(double t);
	inline double easeOutBack(double t);
	inline double easeInOutBack(double t);
	inline double easeInElastic(double t);
	inline double easeOutElastic(double t);
	inline double easeInOutElastic(double t);
	inline double easeInBounce(double t);
	inline double easeOutBounce(double t);
	inline double easeInOutBounce(double t);

	enum easing_functions
	{
		EaseInSine,
		EaseOutSine,
		EaseInOutSine,
		EaseInQuad,
		EaseOutQuad,
		EaseInOutQuad,
		EaseInCubic,
		EaseOutCubic,
		EaseInOutCubic,
		EaseInQuart,
		EaseOutQuart,
		EaseInOutQuart,
		EaseInQuint,
		EaseOutQuint,
		EaseInOutQuint,
		EaseInExpo,
		EaseOutExpo,
		EaseInOutExpo,
		EaseInCirc,
		EaseOutCirc,
		EaseInOutCirc,
		EaseInBack,
		EaseOutBack,
		EaseInOutBack,
		EaseInElastic,
		EaseOutElastic,
		EaseInOutElastic,
		EaseInBounce,
		EaseOutBounce,
		EaseInOutBounce
	};

	typedef double(*easingFunction)(double);

	easingFunction getEasingFunction(easing_functions function);

}
