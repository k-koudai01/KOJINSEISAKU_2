#include "StageDateReader.h"
#include "../nlohmann/json.hpp"

using json = nlohmann::json;

std::vector <RawStageObj_Data> StageDateReader::ReadJson(const std::string& filename)
{
	std::vector<RawStageObj_Data> resultList;

	std::ifstream file(filename);
	if(!file.is_open()) { return resultList; } // ファイルが開けなかったら空で返す

	json jsonDate;
	try 
	{
		file >> jsonDate; // ノーマンさんの機能で解析
	}
	catch(json::parse_error& e)
	{
		return resultList; // 解析に失敗したら空で返す
	}

	// JSON内の "stage_objects" 配列をループで1個ずつ処理
	for(const auto& objData : jsonDate["stage_objects"])
	{
		RawStageObj_Data rawObj;

		// 文字列データの取得
		rawObj.name = objData["name"].get<std::string>();
		rawObj.type = objData["type"].get<std::string>();

		// VECTORデータの取得
		// 座標
		rawObj.pos.x = static_cast<float>(objData["pos"]["x"].get<double>());
		rawObj.pos.y = static_cast<float>(objData["pos"]["y"].get<double>());
		rawObj.pos.z = static_cast<float>(objData["pos"]["z"].get<double>());

		// 回転
		rawObj.rot.x = static_cast<float>(objData["rot"]["roll"].get<double>());
		rawObj.rot.y = static_cast<float>(objData["rot"]["pitch"].get<double>());
		rawObj.rot.z = static_cast<float>(objData["rot"]["yaw"].get<double>());

		// 大きさ
		rawObj.scale.x = static_cast<float>(objData["scale"]["x"].get<double>());
		rawObj.scale.y = static_cast<float>(objData["scale"]["y"].get<double>());
		rawObj.scale.z = static_cast<float>(objData["scale"]["z"].get<double>());

		// リストに追加
		resultList.push_back(rawObj);
	}
	return resultList;
}