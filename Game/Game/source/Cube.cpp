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
    VECTOR v[8];
    v[0] = VGet(-hx,  hy, -hz); // 前・上・左
    v[1] = VGet( hx,  hy, -hz); // 前・上・右
    v[2] = VGet(-hx, -hy, -hz); // 前・下・左
    v[3] = VGet( hx, -hy, -hz); // 前・下・右
    v[4] = VGet(-hx,  hy,  hz); // 後・上・左
    v[5] = VGet( hx,  hy,  hz); // 後・上・右
    v[6] = VGet(-hx, -hy,  hz); // 後・下・左
    v[7] = VGet( hx, -hy,  hz); // 後・下・右

    // 回転行列
    MATRIX matRotX = MGetRotX(_vDir.x * DX_PI_F / 180.0f);
    MATRIX matRotY = MGetRotY(_vDir.y * DX_PI_F / 180.0f); 
    MATRIX matRotZ = MGetRotZ(_vDir.z * DX_PI_F / 180.0f); 

    MATRIX matRot = MMult(MMult(matRotZ, matRotX), matRotY);

	// 頂点を回転させる
    for(int i = 0; i < 8; i++)
    {
        v[i] = VTransform(v[i], matRot); // 回転を適用
		v[i] = VAdd(v[i], _vPos);        // 位置
    }

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
    // 前面
    vertices[idx++].pos = v[0]; vertices[idx++].pos = v[1]; vertices[idx++].pos = v[2];
    vertices[idx++].pos = v[2]; vertices[idx++].pos = v[1]; vertices[idx++].pos = v[3];
    // 背面
    vertices[idx++].pos = v[5]; vertices[idx++].pos = v[4]; vertices[idx++].pos = v[7];
    vertices[idx++].pos = v[7]; vertices[idx++].pos = v[4]; vertices[idx++].pos = v[6];
    // 上面
    vertices[idx++].pos = v[4]; vertices[idx++].pos = v[5]; vertices[idx++].pos = v[0];
    vertices[idx++].pos = v[0]; vertices[idx++].pos = v[5]; vertices[idx++].pos = v[1];
    // 下面
    vertices[idx++].pos = v[2]; vertices[idx++].pos = v[3]; vertices[idx++].pos = v[6];
    vertices[idx++].pos = v[6]; vertices[idx++].pos = v[3]; vertices[idx++].pos = v[7];
    // 左面
    vertices[idx++].pos = v[4]; vertices[idx++].pos = v[0]; vertices[idx++].pos = v[6];
    vertices[idx++].pos = v[6]; vertices[idx++].pos = v[0]; vertices[idx++].pos = v[2];
    // 右面
    vertices[idx++].pos = v[1]; vertices[idx++].pos = v[5]; vertices[idx++].pos = v[3];
    vertices[idx++].pos = v[3]; vertices[idx++].pos = v[5]; vertices[idx++].pos = v[7];

	// 36個の頂点を描画    
    DrawPolygon3D(vertices, 12, DX_NONE_GRAPH, TRUE);
}