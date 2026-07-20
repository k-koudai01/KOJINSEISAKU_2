#include "StageLoader.h"
#include "ObjectFactry.h"
#include "ObjectManager.h"
#include "Cube.h"

namespace
{
	
	// UE座標系からDx座標系に変換する
	VECTOR ConvertUEToDxPos(const VECTOR& uePos)
	{
		return VGet(uePos.x, uePos.z, uePos.y);
	}

	VECTOR ConvertUEToDxScale(const VECTOR& ueScale, float baseMultiplier = 10.0f, float minHeight = 10.0f)
	{
		VECTOR dxScale;
		dxScale.x = ueScale.x * baseMultiplier;
		dxScale.y = ueScale.z * baseMultiplier; 
		dxScale.z = ueScale.y * baseMultiplier; 

		
		// 最小高さを設定
		if(dxScale.y < minHeight)
		{
			dxScale.y = minHeight;
		}

		return dxScale;
	}
}

void StageLoader::LoadStage(const std::string& filename, const ObjectFactry& factory, ObjectManager& objMgr)
{
	// JSONファイルを読み込む
	std::vector<RawStageObj_Data> stageData = StageDateReader::ReadJson(filename);

	for(const auto& data : stageData)
	{
		// ファクトリーによるオブジェクト生成
		auto newObj = factory.CreateStageObject(data.type);

		if(newObj)
		{
			if(data.type == "Cube")
			{
				Cube* cube = static_cast<Cube*>(newObj.get());
				cube->SetPosition(ConvertUEToDxPos(data.pos));
				cube->SetScale(ConvertUEToDxScale(data.scale, 10.0f, 10.0f));
				cube->SetDirection(data.rot);
			}

			// 生成したオブジェクトをオブジェクトマネージャーに追加
			objMgr.Add(std::move(newObj));
		}
	}

}