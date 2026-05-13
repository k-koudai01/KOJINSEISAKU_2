#pragma once
//#include <memory>
//#include <vector>
//#include <algorithm>
#include "appframe.h"
#include "ObjectBase.h"

class ObjectManager
{
public:

	// 追加
	void Add(std::unique_ptr<ObjectBase> obj);

	// 一括処理
	void ProcessAll();
	void RenderAll();

	// クリア
	void TerminateAll();
	void Clear();

	// 条件で削除
	template <class Pred>
	void RemoveIf(Pred pred)
	{
		_objects.erase
		(
			std::remove_if(_objects.begin(), _objects.end(),
			               [&](const std::unique_ptr<ObjectBase>& p) { return p && pred(*p); }),
			               _objects.end()
	    );			   
	}

private:
	std::vector<std::unique_ptr<ObjectBase>> _objects;
};

