#pragma once

#ifndef _CAMERA_HPP_
#define _CAMERA_HPP_

#include "../../engine_types.hpp"
#include <cmath>

struct SizeOfProjectionPlane {

	SizeOfProjectionPlane() noexcept :length(1920), width(1080) {}
	SizeOfProjectionPlane(unsigned int length_, unsigned int width_) noexcept : length(length_), width(width_) {};
	unsigned int length;
	unsigned int width;

};

class Camera {

private:

	Vector3D vector_x_;
	Vector3D vector_y_;
	Vector3D vector_z_;
	Vertex3D observer_pos_;
	float proj_plane_dist_;
	size_t width_;
	size_t height_;

public:

	explicit Camera()noexcept(true);
	void SetVectorZ(float x, float y, float z)noexcept(true);
	void ChangeDistanceToProgPlane(float offset_distance)noexcept(true);
	void Translate(const Vertex3D& delta_vertex)noexcept(true);
	void Rotate(const float alpha_deegree, const Vector3D& around_vector, const Vertex3D& around_point)noexcept(true);
	inline const Vertex3D& GetPosition()const noexcept(true) { return observer_pos_; }
	inline float GetDistanceToProjPlane() const noexcept(true) { return proj_plane_dist_; };
	inline Vector3D GetDirection()const noexcept(true) { return vector_z_; }
	inline Vector3D GetVectorX()const noexcept(true) { return vector_x_; }
	inline Vector3D GetVectorY()const noexcept(true) { return vector_y_; }

	inline void Position(const float x, const float y, const float z)noexcept(true) {
		
		observer_pos_.x = x;
		observer_pos_.y = y;
		observer_pos_.z = z;

	}

	inline void Position(const Vertex3D& new_pos)noexcept(true) {

		observer_pos_ = new_pos;

	}

	inline const Vertex3D& Position()const noexcept(true) { return observer_pos_; };

	inline void SetDirection(const float x, const float y, const float z)noexcept(true) {
		
		vector_z_.x = x;
		vector_z_.y = y;
		vector_z_.z = z;

	}

	inline void SetPlaneSize(const unsigned int width, const unsigned int height)noexcept(true) {

		width_ = width;
		height_ = height;

	}

	inline void SetDistanceToPlane(const float distance)noexcept(true) { proj_plane_dist_ = distance; }

};

#endif