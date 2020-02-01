#pragma once



 
#pragma warning( push )
#pragma warning( disable : 4127)
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#pragma warning( pop ) 
namespace gfx
{
	
	
	
	enum class Space
	{
		Local,
		World,
	};

	class Camera
	{
	public:

		Camera();
		virtual ~Camera();

		void set_LookAt(const glm::vec3& eye, const glm::vec3& target, const glm::vec3& up);
		glm::mat4 get_ViewMatrix() const;
		glm::mat4 get_InverseViewMatrix() const;

		
		void set_Projection(float fovy, float aspect, float zNear, float zFar);
		glm::mat4 get_ProjectionMatrix() const;
		glm::mat4 get_InverseProjectionMatrix() const;

		
		void set_FoV(float fovy);

		
		float get_FoV() const;
		float get_ZNear() const { return m_zNear; }
		float get_ZFar() const { return m_zFar; }

		
		void set_Translation(glm::vec3 translation);
		glm::vec3 get_Translation() const;

		
		void set_Rotation(glm::quat rotation);
		
		glm::quat get_Rotation() const;

		void Translate(glm::vec3 translation, Space space = Space::Local);
		void Rotate(glm::quat quaternion);

		
		glm::vec3 m_Translation;
		
		glm::quat m_Rotation;

	protected:
		virtual void UpdateViewMatrix() const;
		virtual void UpdateInverseViewMatrix() const;
		virtual void UpdateProjectionMatrix() const;
		virtual void UpdateInverseProjectionMatrix() const;

		
		
		
		
		
		
		
		
		
		
		
		
		
		


		mutable glm::mat4 m_ViewMatrix, m_InverseViewMatrix;
		mutable glm::mat4 m_ProjectionMatrix, m_InverseProjectionMatrix;

		
		float m_vFoV;   
		float m_AspectRatio; 
		float m_zNear;      
		float m_zFar;       

		
		mutable bool m_ViewDirty, m_InverseViewDirty;
		
		mutable bool m_ProjectionDirty, m_InverseProjectionDirty;

	private:

	};
}
