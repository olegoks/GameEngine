#include "Camera.hpp"

static const float pi = 3.14159265;

Camera::Camera()noexcept(true):
	width_{ 1920 },
	height_{ 1080 },
	observer_pos_{ 1.0f, 0.0f, -10.0f },
	vector_x_{ 1.0f, 0.0f, 0.0f },
	vector_y_{ 0.0f, 1.0f, 0.0f },
	vector_z_{ 0.0f, 0.0f, 1.0f },
	proj_plane_dist_{ 0.5f } {

	float length_y = sqrt(vector_y_.x * vector_y_.x + vector_y_.y * vector_y_.y + vector_y_.z * vector_y_.z);

	vector_y_.x /= length_y;
	vector_y_.y /= length_y;
	vector_y_.z /= length_y;

	float length_z = sqrt(vector_z_.x * vector_z_.x + vector_z_.y * vector_z_.y + vector_z_.z * vector_z_.z);

	vector_z_.x /= length_z;
	vector_z_.y /= length_z;
	vector_z_.z /= length_z;

};


void Camera::Rotate(float alpha_deegree, const Vector3D& around_vector, const Vertex3D& around_vertex)noexcept(true) {

	float alpha_rad = alpha_deegree * pi / 180.0f;
	RotatePrimitive(observer_pos_, alpha_rad, around_vector, around_vertex);
	RotatePrimitive(vector_x_, alpha_rad, around_vector, around_vertex);
	RotatePrimitive(vector_y_, alpha_rad, around_vector, around_vertex);
	RotatePrimitive(vector_z_, alpha_rad, around_vector, around_vertex);

	//float alpha_rad = alpha_deegree * pi / 180.0f;

	//const float sin_alpha = sin(alpha_rad);
	//const float cos_alpha = cos(alpha_rad);

	//float VX = around_vector.x;
	//float VY = around_vector.y;
	//float VZ = around_vector.z;

	//float XP = observer_pos_.x - around_vertex.x;
	//float YP = observer_pos_.y - around_vertex.y;
	//float ZP = observer_pos_.z - around_vertex.z;

	//observer_pos_.x = (cos_alpha + (1 - cos_alpha) * VX * VX) * XP + ((1 - cos_alpha) * VX * VY - sin_alpha * VZ) * YP + ((1 - cos_alpha) * VX * VZ + sin_alpha * VY) * ZP + around_vertex.x;
	//observer_pos_.y = ((1 - cos_alpha) * VY * VX + sin_alpha * VZ) * XP + (cos_alpha + (1 - cos_alpha) * VY * VY) * YP + ((1 - cos_alpha) * VY * VZ - sin_alpha * VX) * ZP + around_vertex.y;
	//observer_pos_.z = ((1 - cos_alpha) * VZ * VX - sin_alpha * VY) * XP + ((1 - cos_alpha) * VZ * VY + sin_alpha * VX) * YP + (cos_alpha + (1 - cos_alpha) * VZ * VZ) * ZP + around_vertex.z;

	//XP = this->vector_x_.x - around_vertex.x;
	//YP = this->vector_x_.y - around_vertex.y;
	//ZP = this->vector_x_.z - around_vertex.z;

	//this->vector_x_.x = (cos_alpha + (1 - cos_alpha) * VX * VX) * XP + ((1 - cos_alpha) * VX * VY - sin_alpha * VZ) * YP + ((1 - cos_alpha) * VX * VZ + sin_alpha * VY) * ZP + around_vertex.x;
	//this->vector_x_.y = ((1 - cos_alpha) * VY * VX + sin_alpha * VZ) * XP + (cos_alpha + (1 - cos_alpha) * VY * VY) * YP + ((1 - cos_alpha) * VY * VZ - sin_alpha * VX) * ZP + around_vertex.y;
	//this->vector_x_.z = ((1 - cos_alpha) * VZ * VX - sin_alpha * VY) * XP + ((1 - cos_alpha) * VZ * VY + sin_alpha * VX) * YP + (cos_alpha + (1 - cos_alpha) * VZ * VZ) * ZP + around_vertex.z;

	//XP = this->vector_y_.x - around_vertex.x;
	//YP = this->vector_y_.y - around_vertex.y;
	//ZP = this->vector_y_.z - around_vertex.z;

	//this->vector_y_.x = (cos_alpha + (1 - cos_alpha) * VX * VX) * XP + ((1 - cos_alpha) * VX * VY - sin_alpha * VZ) * YP + ((1 - cos_alpha) * VX * VZ + sin_alpha * VY) * ZP + around_vertex.x;
	//this->vector_y_.y = ((1 - cos_alpha) * VY * VX + sin_alpha * VZ) * XP + (cos_alpha + (1 - cos_alpha) * VY * VY) * YP + ((1 - cos_alpha) * VY * VZ - sin_alpha * VX) * ZP + around_vertex.y;
	//this->vector_y_.z = ((1 - cos_alpha) * VZ * VX - sin_alpha * VY) * XP + ((1 - cos_alpha) * VZ * VY + sin_alpha * VX) * YP + (cos_alpha + (1 - cos_alpha) * VZ * VZ) * ZP + around_vertex.z;

	//XP = this->vector_z_.x - around_vertex.x;
	//YP = this->vector_z_.y - around_vertex.y;
	//ZP = this->vector_z_.z - around_vertex.z;

	//this->vector_z_.x = (cos_alpha + (1 - cos_alpha) * VX * VX) * XP + ((1 - cos_alpha) * VX * VY - sin_alpha * VZ) * YP + ((1 - cos_alpha) * VX * VZ + sin_alpha * VY) * ZP + around_vertex.x;
	//this->vector_z_.y = ((1 - cos_alpha) * VY * VX + sin_alpha * VZ) * XP + (cos_alpha + (1 - cos_alpha) * VY * VY) * YP + ((1 - cos_alpha) * VY * VZ - sin_alpha * VX) * ZP + around_vertex.y;
	//this->vector_z_.z = ((1 - cos_alpha) * VZ * VX - sin_alpha * VY) * XP + ((1 - cos_alpha) * VZ * VY + sin_alpha * VX) * YP + (cos_alpha + (1 - cos_alpha) * VZ * VZ) * ZP + around_vertex.z;


}

void Camera::Translate(const Vertex3D& delta_vertex)noexcept(true) {

	observer_pos_.x += delta_vertex.x;
	observer_pos_.y += delta_vertex.y;
	observer_pos_.z += delta_vertex.z;

}

void Camera::ChangeDistanceToProgPlane(float offset_distance)noexcept(true) {

	proj_plane_dist_ += offset_distance;

}

void Camera::SetVectorZ(float x, float y, float z)noexcept(true) {

	const float vector_length = sqrt(x * x + y * y + z * z);

	observer_pos_.x = x / vector_length;
	observer_pos_.y = y / vector_length;
	observer_pos_.z = z / vector_length;

}


