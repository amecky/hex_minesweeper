cbuffer cbChangesPerFrame : register( b0 )
{
    matrix mvp_;
    matrix world;
    float3 camera;
    float3 light;
};


Texture2D colorMap_ : register( t0 );
SamplerState colorSampler_ : register( s0 );


struct VS_Input
{
    float4 pos  : POSITION;
    float3 normal : NORMAL;
    float2 tex0 : TEXCOORD0;
    float4 color : COLOR0;
};

struct PS_Input
{
    float4 pos  : SV_POSITION;
    float2 tex0 : TEXCOORD0;
    float4 color : COLOR0;
    float3 norm : NORMAL;
    float3 lightVec : TEXCOORD1;
    float3 viewVec : TEXCOORD2;
};


PS_Input VS_Main( VS_Input vertex )
{
    PS_Input vsOut = ( PS_Input )0;
    vsOut.pos = mul( vertex.pos, mvp_ );
    vsOut.tex0 = vertex.tex0;
    vsOut.color = vertex.color;
    vsOut.norm = mul(vertex.normal,(float3x3)world);
    vsOut.norm = normalize(vsOut.norm);
    vsOut.lightVec = normalize(light);
    vsOut.viewVec = normalize(camera);
    return vsOut;
}


float4 PS_Main( PS_Input frag ) : SV_TARGET
{
    float3 ambientColor = float3(0.3f,0.3f,0.3f);
    float4 lightColor = colorMap_.Sample( colorSampler_, frag.tex0 ) * frag.color;    
    //float4 lightColor = frag.color;
    ambientColor *= lightColor.rgb;
    float3 lightVec = normalize(frag.pos - frag.lightVec);
    //float3 lightVec = normalize(frag.lightVec);
    float3 normal = normalize(frag.norm);
    float diffuseTerm = clamp(dot(normal,lightVec),0.0f,1.0f);
    float specularTerm = 0.0f;
    if ( diffuseTerm > 0.0f ) {
        float3 viewVec = normalize(frag.viewVec);
        //float3 halfVec = normalize(lightVec + viewVec);
        //float3 halfVec = normalize(lightVec + viewVec);
        float3 halfVec = normalize(normalize(viewVec - frag.pos) - lightVec);
        specularTerm = pow(saturate(dot(normal,halfVec)),25);        
    }
    float3 finalColor = ambientColor +  lightColor.rgb * diffuseTerm + lightColor * specularTerm;
   	return float4(finalColor,lightColor.a);

}