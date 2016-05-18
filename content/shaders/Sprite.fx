cbuffer cbChangesPerFrame : register( b0 )
{
    matrix mvp_;
    matrix world;
    float3 camera;
    float3 light;
};


Texture2D colorMap_ : register( t0 );
SamplerState colorSampler_ : register( s0 );


struct VS_Input {
    float3 pos  : POSITION;
    float4 tex : COLOR0;
    float3 size : NORMAL0;
    float4 color : COLOR1;
};

struct GSPS_INPUT {
    float4 Pos : SV_POSITION;
    float4 Tex : COLOR0;
    float3 Size : NORMAL0;    
    float4 Color : COLOR1;
};

struct PS_Input {
    float4 pos  : SV_POSITION;
    float2 tex0 : TEXCOORD0;
    float4 color : COLOR0;
};


GSPS_INPUT VS_Main( VS_Input vertex ) {
    GSPS_INPUT vsOut = ( GSPS_INPUT )0;
    vsOut.Pos = float4(vertex.pos,1.0);//mul( vertex.pos, mvp_ );
    vsOut.Tex = vertex.tex;
    vsOut.Color = vertex.color;
    vsOut.Size = vertex.size;
    return vsOut;
}

// We expand each point into a quad (4 vertices), so the maximum number of vertices
 // we output per geometry shader invocation is 4.
[maxvertexcount(4)]
void GS_Main(point GSPS_INPUT gin[1], inout TriangleStream<PS_Input> triStream)
{    
    float VP_ARRAY[8] = { -0.5f, 0.5f, 0.5f, 0.5f, 0.5f, -0.5f, -0.5f, -0.5f };
    float2 viewportPosition = float2(512,384);
    float2 t[4];
    float4 ret;
    float tw = gin[0].Tex.z / 1024.0;
    float th = gin[0].Tex.w / 1024.0;
    ret.x = gin[0].Tex.x / 1024.0;
    ret.z = ret.x + tw;
    ret.y = gin[0].Tex.y / 1024.0;
    ret.w = ret.y + th;
    t[0] = float2(ret.x,ret.y);
    t[1] = float2(ret.z,ret.y);
    t[2] = float2(ret.z,ret.w);
    t[3] = float2(ret.x,ret.w);

    float4 pos = gin[0].Pos;
    //pos -= float3(viewportPosition, 0.0f);
    PS_Input gout;
    [unroll]
    for(int i = 0; i < 4; ++i) {
        float px = VP_ARRAY[i * 2] * tw;
        float py = VP_ARRAY[i * 2 + 1] * th;
        float sx = px * gin[0].Size.x;
        float sy = py * gin[0].Size.y;
        float xt = cos(gin[0].Size.z) * sx - sin(gin[0].Size.z) * sy;
        float yt = sin(gin[0].Size.z) * sx + cos(gin[0].Size.z) * sy;
        xt += pos.x;
        yt += pos.y;
        //gout.pos = mul(float4(pos.x, pos.y, 0.0f, 1.0f), mvp_); 
        gout.pos = float4(pos.x, pos.y, 2.0f, 1.0f); 
        gout.tex0 = t[i];
        gout.color = gin[0].Color;
        //gout.PrimID = primID;        
        triStream.Append(gout);
    }
    //triStream.RestartStrip();
}


float4 PS_Main( PS_Input frag ) : SV_TARGET {
    return colorMap_.Sample( colorSampler_, frag.tex0 ) * frag.color;    
}