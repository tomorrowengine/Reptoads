











#pragma once

#include "../glm.hpp"

#ifndef GLM_ENABLE_EXPERIMENTAL
#	error "GLM: GLM_GTX_vec_swizzle is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
#endif

namespace glm {
	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> xx(const glm::vec<1, T, Q> &v) {
		return glm::vec<2, T, Q>(v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> xx(const glm::vec<2, T, Q> &v) {
		return glm::vec<2, T, Q>(v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> xx(const glm::vec<3, T, Q> &v) {
		return glm::vec<2, T, Q>(v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> xx(const glm::vec<4, T, Q> &v) {
		return glm::vec<2, T, Q>(v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> xy(const glm::vec<2, T, Q> &v) {
		return glm::vec<2, T, Q>(v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> xy(const glm::vec<3, T, Q> &v) {
		return glm::vec<2, T, Q>(v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> xy(const glm::vec<4, T, Q> &v) {
		return glm::vec<2, T, Q>(v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> xz(const glm::vec<3, T, Q> &v) {
		return glm::vec<2, T, Q>(v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> xz(const glm::vec<4, T, Q> &v) {
		return glm::vec<2, T, Q>(v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> xw(const glm::vec<4, T, Q> &v) {
		return glm::vec<2, T, Q>(v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> yx(const glm::vec<2, T, Q> &v) {
		return glm::vec<2, T, Q>(v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> yx(const glm::vec<3, T, Q> &v) {
		return glm::vec<2, T, Q>(v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> yx(const glm::vec<4, T, Q> &v) {
		return glm::vec<2, T, Q>(v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> yy(const glm::vec<2, T, Q> &v) {
		return glm::vec<2, T, Q>(v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> yy(const glm::vec<3, T, Q> &v) {
		return glm::vec<2, T, Q>(v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> yy(const glm::vec<4, T, Q> &v) {
		return glm::vec<2, T, Q>(v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> yz(const glm::vec<3, T, Q> &v) {
		return glm::vec<2, T, Q>(v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> yz(const glm::vec<4, T, Q> &v) {
		return glm::vec<2, T, Q>(v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> yw(const glm::vec<4, T, Q> &v) {
		return glm::vec<2, T, Q>(v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> zx(const glm::vec<3, T, Q> &v) {
		return glm::vec<2, T, Q>(v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> zx(const glm::vec<4, T, Q> &v) {
		return glm::vec<2, T, Q>(v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> zy(const glm::vec<3, T, Q> &v) {
		return glm::vec<2, T, Q>(v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> zy(const glm::vec<4, T, Q> &v) {
		return glm::vec<2, T, Q>(v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> zz(const glm::vec<3, T, Q> &v) {
		return glm::vec<2, T, Q>(v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> zz(const glm::vec<4, T, Q> &v) {
		return glm::vec<2, T, Q>(v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> zw(const glm::vec<4, T, Q> &v) {
		return glm::vec<2, T, Q>(v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> wx(const glm::vec<4, T, Q> &v) {
		return glm::vec<2, T, Q>(v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> wy(const glm::vec<4, T, Q> &v) {
		return glm::vec<2, T, Q>(v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> wz(const glm::vec<4, T, Q> &v) {
		return glm::vec<2, T, Q>(v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<2, T, Q> ww(const glm::vec<4, T, Q> &v) {
		return glm::vec<2, T, Q>(v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xxx(const glm::vec<1, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xxx(const glm::vec<2, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xxx(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xxy(const glm::vec<2, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xxy(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xxz(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xyx(const glm::vec<2, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xyx(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xyy(const glm::vec<2, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xyy(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xyz(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xzx(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xzy(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xzz(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xwx(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xwy(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xwz(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> xww(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.x, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yxx(const glm::vec<2, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yxx(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yxy(const glm::vec<2, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yxy(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yxz(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yyx(const glm::vec<2, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yyx(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yyy(const glm::vec<2, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yyy(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yyz(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yzx(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yzy(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yzz(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> ywx(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> ywy(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> ywz(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> yww(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.y, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zxx(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zxy(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zxz(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zyx(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zyy(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zyz(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zzx(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zzy(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zzz(const glm::vec<3, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zwx(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zwy(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zwz(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> zww(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.z, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> wxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.w, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> wxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.w, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> wxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.w, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> wxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.w, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> wyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.w, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> wyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.w, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> wyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.w, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> wyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.w, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> wzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.w, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> wzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.w, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> wzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.w, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> wzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.w, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> wwx(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.w, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> wwy(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.w, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> wwz(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.w, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<3, T, Q> www(const glm::vec<4, T, Q> &v) {
		return glm::vec<3, T, Q>(v.w, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxxx(const glm::vec<1, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxxx(const glm::vec<2, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxxx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxxy(const glm::vec<2, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxxy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxxz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxyx(const glm::vec<2, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxyx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxyy(const glm::vec<2, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxyy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxyz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxzx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxzy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxzz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxwx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxwy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxwz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xxww(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.x, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyxx(const glm::vec<2, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyxx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyxy(const glm::vec<2, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyxy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyxz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyyx(const glm::vec<2, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyyx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyyy(const glm::vec<2, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyyy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyyz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyzx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyzy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyzz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xywx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xywy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xywz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xyww(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.y, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzxx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzxy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzxz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzyx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzyy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzyz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzzx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzzy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzzz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzwx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzwy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzwz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xzww(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.z, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xwxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.w, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xwxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.w, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xwxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.w, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xwxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.w, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xwyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.w, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xwyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.w, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xwyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.w, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xwyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.w, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xwzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.w, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xwzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.w, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xwzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.w, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xwzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.w, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xwwx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.w, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xwwy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.w, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xwwz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.w, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> xwww(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.x, v.w, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxxx(const glm::vec<2, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxxx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxxy(const glm::vec<2, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxxy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxxz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxyx(const glm::vec<2, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxyx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxyy(const glm::vec<2, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxyy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxyz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxzx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxzy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxzz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxwx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxwy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxwz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yxww(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.x, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyxx(const glm::vec<2, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyxx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyxy(const glm::vec<2, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyxy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyxz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyyx(const glm::vec<2, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyyx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyyy(const glm::vec<2, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyyy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyyz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyzx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyzy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyzz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yywx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yywy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yywz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yyww(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.y, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzxx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzxy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzxz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzyx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzyy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzyz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzzx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzzy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzzz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzwx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzwy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzwz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> yzww(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.z, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> ywxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.w, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> ywxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.w, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> ywxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.w, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> ywxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.w, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> ywyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.w, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> ywyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.w, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> ywyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.w, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> ywyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.w, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> ywzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.w, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> ywzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.w, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> ywzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.w, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> ywzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.w, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> ywwx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.w, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> ywwy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.w, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> ywwz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.w, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> ywww(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.y, v.w, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxxx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxxy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxxz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxyx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxyy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxyz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxzx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxzy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxzz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxwx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxwy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxwz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zxww(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.x, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyxx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyxy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyxz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyyx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyyy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyyz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyzx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyzy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyzz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zywx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zywy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zywz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zyww(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.y, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzxx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.x, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzxy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.x, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzxz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.x, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzyx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.y, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzyy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.y, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzyz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.y, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzzx(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.z, v.x);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzzy(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.z, v.y);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzzz(const glm::vec<3, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.z, v.z);
	}

	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzwx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzwy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzwz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zzww(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.z, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zwxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.w, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zwxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.w, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zwxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.w, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zwxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.w, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zwyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.w, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zwyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.w, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zwyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.w, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zwyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.w, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zwzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.w, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zwzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.w, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zwzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.w, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zwzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.w, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zwwx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.w, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zwwy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.w, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zwwz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.w, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> zwww(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.z, v.w, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wxxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.x, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wxxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.x, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wxxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.x, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wxxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.x, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wxyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.x, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wxyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.x, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wxyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.x, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wxyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.x, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wxzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.x, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wxzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.x, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wxzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.x, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wxzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.x, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wxwx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.x, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wxwy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.x, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wxwz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.x, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wxww(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.x, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wyxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.y, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wyxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.y, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wyxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.y, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wyxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.y, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wyyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.y, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wyyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.y, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wyyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.y, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wyyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.y, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wyzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.y, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wyzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.y, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wyzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.y, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wyzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.y, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wywx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.y, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wywy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.y, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wywz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.y, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wyww(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.y, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wzxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.z, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wzxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.z, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wzxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.z, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wzxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.z, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wzyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.z, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wzyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.z, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wzyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.z, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wzyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.z, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wzzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.z, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wzzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.z, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wzzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.z, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wzzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.z, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wzwx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.z, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wzwy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.z, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wzwz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.z, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wzww(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.z, v.w, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wwxx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.w, v.x, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wwxy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.w, v.x, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wwxz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.w, v.x, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wwxw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.w, v.x, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wwyx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.w, v.y, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wwyy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.w, v.y, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wwyz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.w, v.y, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wwyw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.w, v.y, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wwzx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.w, v.z, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wwzy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.w, v.z, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wwzz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.w, v.z, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wwzw(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.w, v.z, v.w);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wwwx(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.w, v.w, v.x);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wwwy(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.w, v.w, v.y);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wwwz(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.w, v.w, v.z);
	}

	
	template<typename T, qualifier Q>
	GLM_INLINE glm::vec<4, T, Q> wwww(const glm::vec<4, T, Q> &v) {
		return glm::vec<4, T, Q>(v.w, v.w, v.w, v.w);
	}

}
