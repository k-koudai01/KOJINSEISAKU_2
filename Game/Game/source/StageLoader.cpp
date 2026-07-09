#include "StageLoader.h"
#include "Cube.h"

std::vector<std::shared_ptr<Cube>> StageLoader::_cubes;

namespace {
	// UEの座標（FVector）をDxLibの座標（VECTOR）に変換する汎用関数
	VECTOR ConvertUEToDxPos(const VECTOR& uePos)
	{
		return VGet(uePos.x, uePos.z, uePos.y); // Xはそのままで、YとZを入れ替える
	}

	// UEのスケールをDxLibのスケールに変換し、かつ最小の厚みを保証する汎用関数
	VECTOR ConvertUEToDxScale(const VECTOR& ueScale, float baseMultiplier = 10.0f, float minHeight = 10.0f)
	{
		VECTOR dxScale;
		dxScale.x = ueScale.x * baseMultiplier;
		dxScale.y = ueScale.z * baseMultiplier; // UEのZ（高さ厚み）がDxLibのY
		dxScale.z = ueScale.y * baseMultiplier; // UEのY（奥行き）がDxLibのZ

		// 最小の厚み（高さ）を安全に保証する
		if(dxScale.y < minHeight) {
			dxScale.y = minHeight;
		}
		return dxScale;
	}
}

void StageLoader::LoadStage(const std::string& filename)
{
	
	Clear();

	
	std::vector<RawStageObj_Data> stageData = StageDateReader::ReadJson(filename);

	
	for(const auto& data : stageData)
	{
		if(data.type == "Cube")
		{
			
			auto newCube = std::make_shared<Cube>();

		
			newCube->Initialize();
			newCube->SetPosition(ConvertUEToDxPos(data.pos));
			newCube->SetScale(ConvertUEToDxScale(data.scale, 10.0f, 10.0f));
			newCube->SetDirection(data.rot);

			
			_cubes.push_back(newCube);
		}
	}
}

void StageLoader::Update()
{
	for(auto& cube : _cubes)
	{
		cube->Process();
	}
}

void StageLoader::Draw()
{
	for(auto& cube : _cubes)
	{
		cube->Render();
	}
}

void StageLoader::Clear()
{
	_cubes.clear();
}