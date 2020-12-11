#include "__engine_1.1/Engine.hpp"

static constexpr size_t kRoadLength = 15;
static constexpr size_t kTreesNumber = 8;
static constexpr size_t kTreesFirstId = 0;
static constexpr size_t kCenterRoadFirstId = kTreesFirstId + kTreesNumber;
static constexpr size_t kRightRoadFirstId = kCenterRoadFirstId + kRoadLength;
static constexpr size_t kLeftRoadFirstId = kRightRoadFirstId + kRoadLength;
static constexpr size_t kRightRightRoadFirstId = kLeftRoadFirstId + kRoadLength;
static constexpr size_t kLeftLeftRoadFirstId = kRightRightRoadFirstId + kRoadLength;
static constexpr float kRoadPartWidth = 4.6f * 2.0f;
static constexpr float kRoadPartLength = 4.0f;
static constexpr size_t kCommonModelsNumber = kRoadLength * 5 + kTreesNumber/**/;
static constexpr size_t kCommonSolidRoadPartsNumber = kRoadLength * 2;
static constexpr size_t kCommonDisconRoadPartsNumber = kRoadLength * 3;
static constexpr size_t kNumberOfFaceCars = 5;
static constexpr size_t kMainCarId = kCommonModelsNumber + kNumberOfFaceCars;
static constexpr float kCarWidth = 1.9f;
static const Vertex3D kMainCarSpawn{ 0.0, 0.0, -10.0 };

static bool gas_pressed = false;
static bool stop_pressed = false;
//int getRandomNumber(int min, int max)
//{
//	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
//	// Равномерно распределяем рандомное число в нашем диапазоне
//	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
//}

static void ProcessKeyboard(KeyAction key)noexcept(true) {

	Engine& engine = Engine::Instance();

	const Vector3D rotate_vector{ 0.0f, 1.0f, 0.0f };
	const Vertex3D rotate_vertex{ 0.0f, 0.0f, 0.0f };

	switch (key.action_) {

		


		case Action::Up:
			switch (key.key_) {

			case Key::W:
				gas_pressed = false;
				break;
			}
		break;

		case Action::Down:
			switch (key.key_) {
			case Key::D:
				engine.TranslateModel(kMainCarId, Vertex3D{ 0.5, 0.0, 0.0 });
				break;
			case Key::A:

				engine.TranslateModel(kMainCarId, Vertex3D{ -0.5, 0.0, 0.0 });
				break;
			case Key::W:
				//engine.TranslateModel(kMainCarId, Vertex3D{ 0.0, 0.0, 0.1 });
				gas_pressed = true;
				break;
			}
			break;

	}

}

class Road final {

private:

	size_t n_parts_;
	size_t first_id_;

public:

	explicit Road(size_t first_id)noexcept(true):
		n_parts_{ kRoadLength },
		first_id_{ first_id }{}

	void LoadRoad()const noexcept(true) {

		Engine& engine = Engine::Instance();

		const Vertex3D& camera_pos = engine.CameraPosition(0);

		for (size_t i = first_id_; i < first_id_ + n_parts_; i++)
			engine.TranslateModel(i, Vertex3D{0.0, 0.0, camera_pos.z - 25.0f});

		float offset = 0.0f;

		for (size_t i = first_id_; i < first_id_ + n_parts_ ; i++){

			engine.TranslateModel(i, Vertex3D{ 0.0, 0.0, -offset + kRoadPartLength * static_cast<float>(kRoadLength) });
			offset += kRoadPartLength;

		}

	}

};

static void LogicFunction()noexcept(true) {

	Engine& engine = Engine::Instance();

	const Vertex3D beg_offset{ 0.0, 0.0, +60.0 };
	const Vertex3D& camera_pos = engine.CameraPosition(0);

	for (size_t i = 0; i < kCommonModelsNumber + kNumberOfFaceCars; i++) {

		engine.TranslateModel(i, Vertex3D{ 0.0, 0.0, -0.1 });

		const Vertex3D& model_pos = engine.ModelPosition(i);
		if (model_pos.z < camera_pos.z - 15.0) 
			engine.TranslateModel(i, beg_offset);

	}

	Vertex3D main_car_pos = engine.ModelPosition(kMainCarId);

	if (gas_pressed)
		engine.TranslateModel(kMainCarId, Vertex3D{ 0.0, 0.0, 0.05 });
	else	
 	if(engine.ModelPosition(kMainCarId).z > kMainCarSpawn.z && !gas_pressed)
		engine.TranslateModel(kMainCarId, Vertex3D{ 0.0, 0.0, -0.06 });

}

static void StartPosFunc()noexcept(true) {

	Engine& engine = Engine::Instance();

	engine.CameraPosition(0, Vertex3D{ 0.0, 0.0, -20.0 });
	engine.RotateCamera(0, 72.0f, Vector3D{ 1.0, 0.0, 0.0 }, Vertex3D{ 0.0, 0.0, 0.0 });
	engine.TranslateModel(0, Vertex3D{ -15.0, 0.0, 4.0 });
	engine.TranslateModel(1, Vertex3D{ 15.0, 0.0, 8.0 });
	engine.TranslateModel(2, Vertex3D{ -16.0, 0.0, -6.0 });
	engine.TranslateModel(3, Vertex3D{ 20.0, 0.0, 15.0 });

	engine.TranslateModel(4, Vertex3D{ -14.0, 0.0, 32.0 });
	engine.TranslateModel(5, Vertex3D{ 19.0, 0.0, 37.0 });
	engine.TranslateModel(6, Vertex3D{ -17.0, 0.0, -22.0 });
	engine.TranslateModel(7, Vertex3D{ 16.0, 0.0, 20.0 });

	//Translating Face cars
	size_t id = kCommonModelsNumber;
	engine.TranslateModel(id, Vertex3D{ -(kRoadPartWidth) / 4.0f + kRoadPartWidth, 0.0, 0.0 });
	engine.TranslateModel(id + 1, Vertex3D{ -kRoadPartWidth / 4.0f, 0.0, 30.0 });
	engine.TranslateModel(id + 2, Vertex3D{ -kRoadPartWidth / 4.0f, 0.0, -12.0 });
	engine.TranslateModel(id + 3, Vertex3D{ -kRoadPartWidth +(kRoadPartWidth) / 4.0f, 0.0, 17.0 });
	engine.TranslateModel(id + 4, Vertex3D{ kRoadPartWidth - (kRoadPartWidth) / 4.0f, 0.0, -15.0 });
 
	//engine.
	Road center_road{ kCenterRoadFirstId };
	center_road.LoadRoad();
	
	Road right_road{ kRightRoadFirstId };
	right_road.LoadRoad();

	for (size_t i = kRightRoadFirstId; i < kRightRoadFirstId + kRoadLength; i++)
		engine.TranslateModel(i, Vertex3D{ kRoadPartWidth / 2.0, 0.0, 0.0});

	Road left_road{ kLeftRoadFirstId };
	left_road.LoadRoad();

	for (size_t i = kLeftRoadFirstId; i < kLeftRoadFirstId + kRoadLength; i++)
		engine.TranslateModel(i, Vertex3D{ -kRoadPartWidth / 2.0, 0.0, 0.0 });

	Road right_right_road{ kRightRightRoadFirstId };
	right_right_road.LoadRoad();

	for (size_t i = kRightRightRoadFirstId; i < kRightRightRoadFirstId + kRoadLength; i++)
		engine.TranslateModel(i, Vertex3D{ kRoadPartWidth , 0.0, 0.0 });

	Road left_left_road{ kLeftLeftRoadFirstId };
	left_left_road.LoadRoad();

	for (size_t i = kLeftLeftRoadFirstId; i < kLeftLeftRoadFirstId + kRoadLength; i++)
		engine.TranslateModel(i, Vertex3D{ -kRoadPartWidth , 0.0, 0.0 });

	//Rotate face cars
	for (size_t i = kCommonModelsNumber; i < kMainCarId; i++)
		engine.RotateModel(i, 180, Vector3D{0.0, 1.0, 0.0}, Vertex3D{0.0,0.0, 0.0});

	//Translate MainCar to screen bottom
	engine.TranslateModel(kMainCarId, kMainCarSpawn);

}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow ){

	Engine& engine = Engine::Instance();

	engine.CreateConsole("Log console");

	engine.InitCuda();
	engine.InitKeystrProcFunc(ProcessKeyboard);
	engine.InitLogicFunc(LogicFunction);
	engine.InitStartPosFunc(StartPosFunc);
	engine.DefaultDir("D:\\C++\\ProjectEngine\\__obj_models\\");
	
	for (size_t i = 0; i < kTreesNumber; i++)
		engine.LoadModels({ "ColorTree.obj" });

	for (size_t i = 0; i < kCommonDisconRoadPartsNumber; i++)
		engine.LoadModels({"DorogaNew.obj"});

	for (size_t i = 0; i < kCommonSolidRoadPartsNumber; i++)
		engine.LoadModels({"Doroga2.obj"});
	
	for (size_t i = 0; i < kNumberOfFaceCars; i++)
		engine.LoadModels({"CarBlue.obj"});

	// main car
	engine.LoadModels({ "CarBlue.obj" });

	engine.InitWindow(hInstance);
	engine.StartMainLoop();
	engine.ShowWindow();
	engine.Stop();

	return 0;
}