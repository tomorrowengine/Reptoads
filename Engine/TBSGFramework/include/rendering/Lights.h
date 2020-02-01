#pragma once
#include <glm/vec3.hpp>
#include "core/Assertion.h"

enum class LightDist { d7, d13, d20, d32, d50, d65, d100, d160, d200, d325, d600, d3250 };




















class IPointLight
{
public:
	virtual ~IPointLight() = default;

	virtual void SetPosition(const glm::vec3& a_position) = 0;
	virtual void SetColor(const glm::vec3& a_color) = 0;

	void SetAttenuation(LightDist a_maxLightDistance)
	{
		switch (a_maxLightDistance) {
		case LightDist::d7:		SetLinearAttenuation(0.7f  );	SetQuadraticAttenuation(1.8f	);	break;
		case LightDist::d13:	SetLinearAttenuation(0.35f );	SetQuadraticAttenuation(0.44f	);	break;
		case LightDist::d20:	SetLinearAttenuation(0.22f );	SetQuadraticAttenuation(0.20f	);	break;
		case LightDist::d32:	SetLinearAttenuation(0.14f );	SetQuadraticAttenuation(0.07f	);	break;
		case LightDist::d50:	SetLinearAttenuation(0.09f );	SetQuadraticAttenuation(0.032f	);	break;
		case LightDist::d65:	SetLinearAttenuation(0.07f );	SetQuadraticAttenuation(0.017f	);	break;
		case LightDist::d100:	SetLinearAttenuation(0.045f);	SetQuadraticAttenuation(0.0075f	);	break;
		case LightDist::d160:	SetLinearAttenuation(0.027f);	SetQuadraticAttenuation(0.0028f	);	break;
		case LightDist::d200:	SetLinearAttenuation(0.022f);	SetQuadraticAttenuation(0.0019f	);	break;
		case LightDist::d325:	SetLinearAttenuation(0.014f);	SetQuadraticAttenuation(0.0007f	);	break;
		case LightDist::d600:	SetLinearAttenuation(0.007f);	SetQuadraticAttenuation(0.0002f	);	break;
		case LightDist::d3250:	SetLinearAttenuation(0.0014f);	SetQuadraticAttenuation(0.000007f);	break;
		default: ASSERT_MSG(false, "PUNCTUAL LIGHT VALUE NOT IMPLEMENTED!");
		}
	}
	virtual void SetConstantAttenuation(const float a_la) = 0;
	virtual void SetLinearAttenuation(const float a_la) = 0;
	virtual void SetQuadraticAttenuation(const float a_qa) = 0;
	virtual float GetConstantAttenuation() const = 0;
	virtual float GetLinearAttenuation() const = 0;
	virtual float GetQuadraticAttenuation() const = 0;
};

class ISpotLight
{
public:
	virtual ~ISpotLight() = default;
	
	virtual void SetPosition(const glm::vec3& a_position) = 0;
	virtual void SetDirection(const glm::vec3& a_position) = 0;
	virtual void SetColor(const glm::vec3& a_color) = 0;
	virtual void SetInnerCutOffAngle(float a_angleInDegrees) = 0;
	virtual void SetOuterCutOffAngle(float a_angleInDegrees) = 0;

	void SetAttenuation(LightDist a_maxLightDistance)
	{
		switch (a_maxLightDistance) {
		case LightDist::d7:		SetLinearAttenuation(0.7f);	SetQuadraticAttenuation(1.8f);	break;
		case LightDist::d13:	SetLinearAttenuation(0.35f);	SetQuadraticAttenuation(0.44f);	break;
		case LightDist::d20:	SetLinearAttenuation(0.22f);	SetQuadraticAttenuation(0.20f);	break;
		case LightDist::d32:	SetLinearAttenuation(0.14f);	SetQuadraticAttenuation(0.07f);	break;
		case LightDist::d50:	SetLinearAttenuation(0.09f);	SetQuadraticAttenuation(0.032f);	break;
		case LightDist::d65:	SetLinearAttenuation(0.07f);	SetQuadraticAttenuation(0.017f);	break;
		case LightDist::d100:	SetLinearAttenuation(0.045f);	SetQuadraticAttenuation(0.0075f);	break;
		case LightDist::d160:	SetLinearAttenuation(0.027f);	SetQuadraticAttenuation(0.0028f);	break;
		case LightDist::d200:	SetLinearAttenuation(0.022f);	SetQuadraticAttenuation(0.0019f);	break;
		case LightDist::d325:	SetLinearAttenuation(0.014f);	SetQuadraticAttenuation(0.0007f);	break;
		case LightDist::d600:	SetLinearAttenuation(0.007f);	SetQuadraticAttenuation(0.0002f);	break;
		case LightDist::d3250:	SetLinearAttenuation(0.0014f);	SetQuadraticAttenuation(0.000007f);	break;
		default: ASSERT_MSG(false, "PUNCTUAL LIGHT VALUE NOT IMPLEMENTED!");
		}
	}
	virtual void SetConstantAttenuation(const float a_la) = 0;
	virtual void SetLinearAttenuation(const float a_la) = 0;
	virtual void SetQuadraticAttenuation(const float a_qa) = 0;
	virtual float GetConstantAttenuation() const = 0;
	virtual float GetLinearAttenuation() const = 0;
	virtual float GetQuadraticAttenuation() const = 0;
};

class IDirectionalLight
{
public:
	virtual ~IDirectionalLight() = default;
	virtual void SetDirection(const glm::vec3& a_direction) = 0;
	virtual void SetColor(const glm::vec3& a_color) = 0;
	virtual void SetStrength(float a_strength) = 0;
};

#ifdef PLATFORM_WINDOWS

#include "core/ConceptCompatibility.h"
#include "core/meta/TypeTraitExtensions.h"


template<typename TPointLight>
CONCEPT PointLightConcept = REQUIRES(TPointLight pointLight)
{
	glm::vec3 position, color;
	float ca, la, qa;

	CONCEPT_FUNCTION(void, pointLight.SetPosition(position));
	CONCEPT_FUNCTION(void, pointLight.SetColor(color));
	CONCEPT_FUNCTION(void, pointLight.SetConstantAttenuation(ca));
	CONCEPT_FUNCTION(void, pointLight.SetLinearAttenuation(la));
	CONCEPT_FUNCTION(void, pointLight.SetQuadraticAttenuation(qa));

	return true;
}(std::declval<TPointLight>());


template<CONCEPT_TYPE(PointLightConcept) TPointLight>
constexpr void EnforcePointLightConcept(const TPointLight& pointLight)
{
	static_assert(meta::is_complete_t<TPointLight>::value,
		"TPointLight template parameter is an incomplete type!"
		" Check if it's not forward declared!");

#define CHECK_FUNCTION(return_type, expression) static_assert(std::is_same_v<return_type, decltype(expression)>, "Function invocation '"#expression"' did not return '"#return_type"'")
	CHECK_FUNCTION(void, pointLight.SetPosition(glm::vec3{}));
	CHECK_FUNCTION(void, pointLight.SetColor(glm::vec3{}));
	CHECK_FUNCTION(void, pointLight.SetConstantAttenuation(1.5f));
	CHECK_FUNCTION(void, pointLight.SetLinearAttenuation(1.5f));
	CHECK_FUNCTION(void, pointLight.SetQuadraticAttenuation(1.5f));
#undef CHECK_FUNCTION

}
#endif
