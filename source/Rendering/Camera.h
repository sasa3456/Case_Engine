//-----------------------------------------------------
// © Copyright 2024 Case Engine. All Rights Reserved. 
//-----------------------------------------------------


// Includes
#pragma once
#include <utility>


// Namespace Case_Engine
namespace Case_Engine
{
	struct CameraParameters
	{
		float aspect_ratio;
		float near_plane;
		float far_plane;
		float fov;
		float speed;
		float sensitivity;
		Vector3 position;
		Vector3 look_at;
	};

	class Camera
	{
	public:
		Camera() = default;
		explicit Camera(CameraParameters const&);

		Matrix View() const;
		Matrix Proj() const;
		Matrix ViewProj() const;
		BoundingFrustum Frustum() const;

		Vector3 Position() const
		{
			return position;
		}
		Vector3 Up() const
		{
			return up_vector;
		}
		Vector3 Right() const
		{
			return right_vector;
		}
		Vector3 Forward() const
		{
			return look_vector;
		}

		float Near() const;
		float Far() const;
		float Fov() const;
		float AspectRatio() const;

		void SetPosition(Vector3 const& pos);
		void SetNearAndFar(float n, float f);
		void SetAspectRatio(float ar);
		void SetFov(float fov);

		void Zoom(int32_t increment);
		void OnResize(uint32_t w, uint32_t h);
		void Tick(float dt);
		void Enable(bool _enabled) { enabled = _enabled; }

		void Walk(float dt);

	public:

		float speed_factor = 1.0f;
				
	private:
		Vector3 position;
		Vector3 right_vector;
		Vector3 up_vector;
		Vector3 look_vector;
		Matrix view_matrix;
		Matrix projection_matrix;

		float aspect_ratio;
		float fov;
		float near_plane, far_plane;
		float speed;
		float sensitivity;
		bool  enabled;

	private:
		void UpdateViewMatrix();
		void Strafe(float dt);
		void Jump(float dt);
		void Pitch(int64_t dy);
		void Yaw(int64_t dx);
		void SetLens(float fov, float aspect, float zn, float zf);
		void SetView();
	};

}