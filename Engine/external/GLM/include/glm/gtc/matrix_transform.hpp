



















#pragma once


#include "../mat4x4.hpp"
#include "../vec2.hpp"
#include "../vec3.hpp"
#include "../vec4.hpp"
#include "../ext/matrix_projection.hpp"
#include "../ext/matrix_clip_space.hpp"
#include "../ext/matrix_transform.hpp"

#if GLM_MESSAGES == GLM_ENABLE && !defined(GLM_EXT_INCLUDED)
#	pragma message("GLM: GLM_GTC_matrix_transform extension included")
#endif

#include "matrix_transform.inl"
