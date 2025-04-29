#include "common.hlsl"

StructuredBuffer<VS_IN> vertexBuffer : register(t0); // �X�L�j���O�������{���ꂽ���_�o�b�t�@

struct CSVS_IN
{
    uint vertexID : SV_VertexID;
};

void main(in CSVS_IN In, out PS_IN Out)
{
    VS_IN vtx = vertexBuffer[In.vertexID];
	
	// �Ȃɂ����_�ɐF�X��������
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);

    Out.Position = mul(vtx.Position, wvp);
    Out.TexCoord = vtx.TexCoord;
    Out.Diffuse = vtx.Diffuse * Material.Diffuse;
    //Out.Diffuse = 1.0f;
    
}