#include "CudaGraphicEngine.cuh"
#include "cuda.h"
#include "device_launch_parameters.h"

__global__ void ConvertInDisplayCoordinats(Vertex2D* const vertexs_2d, const unsigned int number_of_vertexs, const unsigned int display_width, const unsigned int display_height) {

	int threadIndex = threadIdx.x + blockIdx.x * blockDim.x;

	if (threadIndex < number_of_vertexs) {

		vertexs_2d[threadIndex].x = vertexs_2d[threadIndex].x * 1000.0f + display_width / 2;
		vertexs_2d[threadIndex].y = display_height / 2 - vertexs_2d[threadIndex].y * 1000.0f;

	}

}

__global__ void DrawLines(const Vertex2D* const vertexs_2d, const Polygon3D* const device_polygons, const Normal3D* normals, const unsigned int number_of_polygons, FramePixel* const display_buffer, const unsigned int display_width, const unsigned int display_height, Vector3D camera_dir) {

	int threadIndex = threadIdx.x + blockIdx.x * blockDim.x;

	if (threadIndex < number_of_polygons * 3) {

		const unsigned int polygon_number = threadIndex / 3;
		const unsigned int vertex_number = threadIndex % 3;

		Normal3D normal = normals[device_polygons[polygon_number].ratios[vertex_number].normal_n];
		float scalar = camera_dir.x * normal.x + camera_dir.y * normal.y + camera_dir.z * normal.z;
		//if (scalar <= 0.0f) 
		{

			const unsigned int first_vertex_index = device_polygons[polygon_number].ratios[vertex_number].vertex_n;
			const unsigned int second_vertex_number = ((vertex_number + 1) < 3) ? (vertex_number + 1) : 0;
			const unsigned int second_vertex_index = device_polygons[polygon_number].ratios[second_vertex_number].vertex_n;

			int x1 = vertexs_2d[first_vertex_index].x;
			int y1 = vertexs_2d[first_vertex_index].y;
			const int x2 = vertexs_2d[second_vertex_index].x;
			const int y2 = vertexs_2d[second_vertex_index].y;

			const bool coordinats_are_correct = (x1 > 0 && x1 < display_width) && (x2 > 0 && x2 < display_width) && (y1 > 0 && y1 < display_height) && (y2 > 0 && y2 < display_height);

			if (coordinats_are_correct) {

				const int deltaX = abs(x2 - x1);
				const int deltaY = abs(y2 - y1);
				const int signX = x1 < x2 ? 1 : -1;
				const int signY = y1 < y2 ? 1 : -1;
				//
				int error = deltaX - deltaY;

				display_buffer[display_width * y2 + x2].rgba_alpha = 0;
				display_buffer[display_width * y2 + x2].rgba_red = 255;
				display_buffer[display_width * y2 + x2].rgba_green = 0;
				display_buffer[display_width * y2 + x2].rgba_blue = 0;

				while (x1 != x2 || y1 != y2)
				{

					display_buffer[display_width * y1 + x1].rgba_alpha = 0;
					display_buffer[display_width * y1 + x1].rgba_red = 255;
					display_buffer[display_width * y1 + x1].rgba_green = 0;
					display_buffer[display_width * y1 + x1].rgba_blue = 0;

					const int error2 = error * 2;
					//
					if (error2 > -deltaY)
					{
						error -= deltaY;
						x1 += signX;
					}

					if (error2 < deltaX)
					{
						error += deltaX;
						y1 += signY;
					}

				}
			}
		}
	}

}

__global__ void ProjectVertexs(const Vertex3D* const vertexs_3d, Vertex2D* const vertexs_2d, const unsigned int number_of_vertexs, const Vertex3D cameraPosition, const Vector3D vectorX, const Vector3D vectorY, const Vector3D vectorZ, const float k) {

	int thread_index = threadIdx.x + blockIdx.x * blockDim.x;

	if (thread_index < number_of_vertexs) {

		//Offset

		//Central projection
		float offset_x = vertexs_3d[thread_index].x - cameraPosition.x;
		float offset_y = vertexs_3d[thread_index].y - cameraPosition.y;
		float offset_z = vertexs_3d[thread_index].z - cameraPosition.z;

		float new_x = offset_x * vectorX.x + offset_y * vectorX.y + offset_z * vectorX.z;
		float new_y = offset_x * vectorY.x + offset_y * vectorY.y + offset_z * vectorY.z;
		float new_z = offset_x * vectorZ.x + offset_y * vectorZ.y + offset_z * vectorZ.z;

		vertexs_2d[thread_index].x = (k * new_x / (new_z + k));
		vertexs_2d[thread_index].y = (k * new_y / (new_z + k));
		//Ortogonal
		//vertexs_2d[threadIndex].x = vertexs_3d[threadIndex].x;
		//vertexs_2d[threadIndex].y = vertexs_3d[threadIndex].y;

	}
}

__global__ void  SetScreenColor(FramePixel* const device_display_buffer, const FramePixel rgb_pixel, const unsigned int number_of_pixels) {

	int thread_index = threadIdx.x + blockIdx.x * blockDim.x;

	if (thread_index < number_of_pixels) device_display_buffer[thread_index] = rgb_pixel;

}

__host__ void GraphicEngine::FillDeviceFrame(const FramePixel& pixel)noexcept(true) {

	const size_t frame_size = device_frame_.Width() * device_frame_.Height();
	const size_t number_of_threads = 1024;
	const size_t number_of_blocks = (frame_size + number_of_threads - 1) / number_of_threads;

	SetScreenColor << < number_of_blocks, number_of_threads >> > (device_frame_.FramePixelPtr(), pixel, frame_size);

}
__host__ void CopyDeviceFrameToHostFrame(FrameHost& host_frame, FrameDevice& device_frame)noexcept(true) {

	cudaMemcpy((void*)host_frame.buffer_, (const void*) device_frame.buffer_, host_frame.Width() * host_frame.Height() * sizeof(FramePixel), cudaMemcpyDeviceToHost);

}

__host__ FrameHost& GraphicEngine::RenderMeshFrame()noexcept(true) {

	FillDeviceFrame(RgbColor{ 255, 166, 128 });
	/*or (size_t i = 0; i < host_frame_.Width(); i++)
		for (size_t j = 0; j < host_frame_.Height(); j++)
			host_frame_[{i, j}] = color;*/



	static Vertex2D* const device_vertexs_2d = static_cast<Vertex2D* const>(AllocateMemory(device_data_->PrimitivesCapacity().n_vertexs * sizeof(Vertex2D)));

	const Vertex3D& camera_position = camera_->GetPosition();
	//
	const Vector3D& vector_x = camera_->GetVectorX();
	const Vector3D& vector_y = camera_->GetVectorY();
	const Vector3D& vector_z = camera_->GetDirection();

	const float proj_plane_dist = camera_->GetDistanceToProjPlane();

	DevicePtrs ptrs;
	ptrs = device_data_->Ptrs();

	NPrimitives n_primitives;
	n_primitives = device_data_->PrimitivesCapacity();

	const size_t number_of_threads = 1024;
	size_t number_of_blocks = (n_primitives.n_vertexs + number_of_threads - 1) / number_of_threads;

	ProjectVertexs <<<number_of_blocks, number_of_threads>>> (ptrs.vertexs, device_vertexs_2d, n_primitives.n_vertexs, camera_position, vector_x, vector_y, vector_z, proj_plane_dist);

	ConvertInDisplayCoordinats <<<number_of_blocks, number_of_threads >>> (device_vertexs_2d, n_primitives.n_vertexs, device_frame_.Width(), device_frame_.Height());

	number_of_blocks = (n_primitives.n_polygons* 3 + number_of_threads - 1) / number_of_threads;

	DrawLines <<< number_of_blocks, number_of_threads >>> (device_vertexs_2d, ptrs.polygons, ptrs.normals, n_primitives.n_polygons , static_cast<FramePixel*>(static_cast<void*>(device_frame_)), device_frame_.Width(), device_frame_.Height(), vector_z);
	
	CopyDeviceFrameToHostFrame(host_frame_, device_frame_);
	
	return host_frame_;
}
