#include "Cube.h"
#include "appframe.h"

Cube::Cube() : _vPos(VGet(0, 0, 0)), _vDir(VGet(0, 0, 0)), _vScale(VGet(1, 1, 1))
{
}

Cube::~Cube()
{
}

bool Cube::Initialize()
{
	return true;
}

bool Cube::Process()
{
	return true;
}

bool Cube::Render()
{
	CubeRender();
	return true;
}

void Cube::CubeRender()
{
    // 中心座標（_pos）と大きさ（_scale）から、箱の半分のサイズを計算
    float hx = _vScale.x * 0.5f;
    float hy = _vScale.y * 0.5f;
    float hz = _vScale.z * 0.5f;

    // 箱の8つの頂点座標を計算
    VECTOR v0 = VGet(_vPos.x - hx, _vPos.y + hy, _vPos.z - hz); // 前・上・左
    VECTOR v1 = VGet(_vPos.x + hx, _vPos.y + hy, _vPos.z - hz); // 前・上・右
    VECTOR v2 = VGet(_vPos.x - hx, _vPos.y - hy, _vPos.z - hz); // 前 = 下・左
    VECTOR v3 = VGet(_vPos.x + hx, _vPos.y - hy, _vPos.z - hz); // 前・下・右
    VECTOR v4 = VGet(_vPos.x - hx, _vPos.y + hy, _vPos.z + hz); // 後・上・左
    VECTOR v5 = VGet(_vPos.x + hx, _vPos.y + hy, _vPos.z + hz); // 後・上・右
    VECTOR v6 = VGet(_vPos.x - hx, _vPos.y - hy, _vPos.z + hz); // 後・下・左
    VECTOR v7 = VGet(_vPos.x + hx, _vPos.y - hy, _vPos.z + hz); // 後・下・右

    // 白色の色データを作成
    COLOR_U8 color = { 255, 255, 255, 255 };

	// 36個の頂点データを作成
    VERTEX3D vertices[36];
    for(int i = 0; i < 36; ++i)
    {
        vertices[i].dif = color;
        vertices[i].u = 0.0f; vertices[i].v = 0.0f;
    }

    int idx = 0;
    // 前面 (v0, v1, v2, v3)
    vertices[idx++].pos = v0; vertices[idx++].pos = v1; vertices[idx++].pos = v2;
    vertices[idx++].pos = v2; vertices[idx++].pos = v1; vertices[idx++].pos = v3;
    // 背面 (v5, v4, v7, v6)
    vertices[idx++].pos = v5; vertices[idx++].pos = v4; vertices[idx++].pos = v7;
    vertices[idx++].pos = v7; vertices[idx++].pos = v4; vertices[idx++].pos = v6;
    // 上面 (v4, v5, v0, v1)
    vertices[idx++].pos = v4; vertices[idx++].pos = v5; vertices[idx++].pos = v0;
    vertices[idx++].pos = v0; vertices[idx++].pos = v5; vertices[idx++].pos = v1;
    // 下面 (v2, v3, v6, v7)
    vertices[idx++].pos = v2; vertices[idx++].pos = v3; vertices[idx++].pos = v6;
    vertices[idx++].pos = v6; vertices[idx++].pos = v3; vertices[idx++].pos = v7;
    // 左面 (v4, v0, v6, v2)
    vertices[idx++].pos = v4; vertices[idx++].pos = v0; vertices[idx++].pos = v6;
    vertices[idx++].pos = v6; vertices[idx++].pos = v0; vertices[idx++].pos = v2;
    // 右面 (v1, v5, v3, v7)
    vertices[idx++].pos = v1; vertices[idx++].pos = v5; vertices[idx++].pos = v3;
    vertices[idx++].pos = v3; vertices[idx++].pos = v5; vertices[idx++].pos = v7;

	// 36個の頂点を描画    
    DrawPolygon3D(vertices, 12, DX_NONE_GRAPH, TRUE);
}