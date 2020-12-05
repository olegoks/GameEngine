#pragma once
#ifndef _LOGICENGINE_H_
#define _LOGICENGINE_H_

#include "../engine_types.hpp"
#include "../__graphic_engine_/Camera/Camera.hpp"

static enum class TransformationType:uint32_t {

	Rotate = 1,
	Translate = 2,
	Scale = 3

};


class LogicEngine final{

private:

	//Default KeystrProcFunc anf LogicFunc
	static const KeystrProcFunc kDefaultKeystrProcFunc;
	static const LogicFunc kDefaultLogicFunc;
	//static const NextKeystroke kDefaultNextKeystroke; 
	
	KeystrProcFunc keystr_proc_func_;
	LogicFunc logic_func_;
	//NextKeystroke next_keystroke_;

	bool engine_is_running_;
	Camera* camera_;
	DeviceData* device_data_;
	HostArrayOf<Model>* models_;
	KeystrokesQueue* keystrokes_queue_;
	//unsigned int previous_keyboard_process_;
	//unsigned int time_per_update_logic_;

public:

	explicit LogicEngine()noexcept(true);
	void UpdateWorldLogic()noexcept(true);
	//inline void SetPreviousKeyboardProcess(unsigned int time_point) noexcept { previous_keyboard_process_ = time_point; };
	inline void PlugKeystrProcFunction(const KeystrProcFunc& func) noexcept(true) { keystr_proc_func_ = func; };
	inline void PlugLogicFunction(const LogicFunc& func) noexcept { logic_func_ = func; }
	inline void StartEngine()noexcept { engine_is_running_ = true; };
	inline void PauseEngine()noexcept { engine_is_running_ = false; };
	inline void PlugCamera(Camera* camera_) { this->camera_ = camera_; };
	inline void PlugModels(HostArrayOf<Model>* models)noexcept(true) { models_ = models; };
	inline void PlugKeystrokesQueue(KeystrokesQueue* queue)noexcept(true) { keystrokes_queue_ = queue; };
	
	void RotateModel(const ModelId model_id, const float alpha_degree, const Vector3D& around_vector, const Vertex3D& around_vertex)noexcept(true);
	void ScaleModel(const ModelId model_id, const float coefficient)noexcept(true);
	void TranslateModel(const ModelId model_id, const Vertex3D& translate_vertex)noexcept(true);
	const Vertex3D& ModelPosition(const ModelId model_id)const noexcept(true);

};

#endif
