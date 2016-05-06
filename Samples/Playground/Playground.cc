//--------------------------------------------------------------------------------
//This is a file from Arkengine
//
//
//Copyright (c) arkenthera.All rights reserved.
//
//BasicRenderWindow.cpp
//--------------------------------------------------------------------------------

#include "Core/YumeHeaders.h"
#include "Playground.h"
#include "Logging/logging.h"
#include "Core/YumeMain.h"
#include <boost/shared_ptr.hpp>

#include "Input/YumeInput.h"

#include "Renderer/YumeTexture2D.h"

#include "Renderer/YumeResourceManager.h"

#include "Renderer/YumeRHI.h"



#include "UI/YumeDebugOverlay.h"

#include "Engine/YumeEngine.h"

#include "Core/SharedPtr.h"

#include "UI/YumeUI.h"

#include "Renderer/StaticModel.h"

#include "Renderer/Scene.h"
#include "Renderer/Light.h"
#include <Renderer/RenderPass.h>

#include "Core/MersenneTwister.h"


YUME_DEFINE_ENTRY_POINT(YumeEngine::PlaygroundDemo);

#define TURNOFF 1
#define NO_MODEL
#define OBJECTS_CAST_SHADOW
#define NO_SKYBOX
#define NO_PLANE
#define CORNELL
#define DISABLE_EVERYTHING

#define SCALE ((1<<15))

namespace YumeEngine
{

	MTRand Rng;

	PlaygroundDemo::PlaygroundDemo()
		: rot_(Quaternion::IDENTITY),
		rotationAngle_(0.0f)
	{
		REGISTER_ENGINE_LISTENER;

		m_RadiusMultiplier  = 1;
		m_AngleBias         = 30;
		m_NumDirs		    = 25;
		m_NumSteps		    = 32;
		m_Contrast          = 1.25f;
		m_Attenuation       = 1.0f;
		m_AORadius			= 10;

		m_BlurRadius     = 7;
		m_Sharpness      = 16.0f;
		m_EdgeThreshold  = 0.1f;

		m_NumSamples     = 1;
	}

	PlaygroundDemo::~PlaygroundDemo()
	{

	}



	void PlaygroundDemo::Start()
	{
		YumeResourceManager* rm_ = gYume->pResourceManager;

		gYume->pInput->AddListener(this);

#ifndef DISABLE_CEF
		overlay_ = new YumeDebugOverlay;
		gYume->pUI->AddUIElement(overlay_);
		overlay_->SetVisible(true);
#endif


		StaticModel* dragon= CreateModel("Models/dragon/dragon.yume",DirectX::XMFLOAT3(0,5,0),DirectX::XMFLOAT4(0,0,0,0),DirectX::XMFLOAT3(15,15,15));
		StaticModel* dragon2= CreateModel("Models/dragon/dragon.yume",DirectX::XMFLOAT3(15,5,15),DirectX::XMFLOAT4(0,0,0,0),DirectX::XMFLOAT3(15,15,15));
		StaticModel* dragon3= CreateModel("Models/dragon/dragon.yume",DirectX::XMFLOAT3(30,5,0),DirectX::XMFLOAT4(0,0,0,0),DirectX::XMFLOAT3(15,15,15));
		StaticModel* plane = CreateModel("Models/Primitives/plane.yume",DirectX::XMFLOAT3(0,0,0));

		MaterialPtr diff = YumeAPINew Material;
		diff->SetShaderParameter("DiffuseColor",DirectX::XMFLOAT4(0.4f,0.4f,0.4f,1));
		diff->SetShaderParameter("SpecularColor",DirectX::XMFLOAT4(0.7f,0.7f,0.7f,1));

		plane->SetMaterial(diff);
		dragon->SetMaterial(diff);
		dragon2->SetMaterial(diff);
		dragon3->SetMaterial(diff);
		

		/*StaticModel* plane = CreateModel("Models/cornell/cornellbox.yume");*/


		DirectX::XMFLOAT3 min = gYume->pRenderer->GetMinBb();
		DirectX::XMFLOAT3 max = gYume->pRenderer->GetMaxBb();

		DirectX::XMFLOAT3 delta = DirectX::XMFLOAT3(max.x - min.x,max.y - min.y,max.z - min.z);


		float x = 0;
		float y = 5;

		Light* dirLight = new Light;
		dirLight->SetName("DirLight");
		dirLight->SetType(LT_DIRECTIONAL);
		/*dirLight->SetPosition(DirectX::XMVectorSet(0,20,0,0));*/
		dirLight->SetPosition(DirectX::XMVectorSet(0,50,0,0));
		dirLight->SetDirection(DirectX::XMVectorSet(0,-1,0,0));
		dirLight->SetRotation(DirectX::XMVectorSet(-1,0,0,0));
		dirLight->SetColor(YumeColor(1,1,1,0));


		//CreateCircleOfPointLights(DirectX::XMFLOAT3(0,0,0),30,45);
		//CreateCircleOfPointLights(DirectX::XMFLOAT3(0,0,0),30,60);
		//CreateCircleOfPointLights(DirectX::XMFLOAT3(0,0,0),30,90);
		//CreateCircleOfPointLights(DirectX::XMFLOAT3(0,0,0),30,120);



		/*CreateLight(DirectX::XMFLOAT3(0,19,-15),YumeColor(1,1,1),40);*/
		/*CreateLight(DirectX::XMFLOAT3(0,10,0),YumeColor(1,1,1),40);*/

		gYume->pRenderer->GetScene()->AddNode(dirLight);


		RenderTargetDesc desc;
		desc.Index = 5;

		randomMap_ = (gYume->pRHI->CreateTexture2D());
		randomMap_->SetName("RandomVec");
		randomMap_->SetDesc(desc);
		randomMap_->SetSize(64,64,gYume->pRHI->GetRGBA16FormatNs(),TEXTURE_STATIC,1,1);

		RenderPass* d = gYume->pRenderer->GetDefaultPass();

		d->AddTexture(5,"HorizonBasedAO",randomMap_);

		UpdateSSAO();
		UpdateAngleBias();
		UpdateRadius();

		UpdateBlur();
		UpdateSharpness();

		float width = gYume->pRHI->GetWidth() * 0.5f;
		float height = gYume->pRHI->GetHeight() * 0.5f;

		float fovy = 60 * M_DEGTORAD;

		m_FocalLen[0]      = 1.0f / tanf(fovy * 0.5f) *  (float)width / (float)height;
		m_FocalLen[1]      = 1.0f / tanf(fovy * 0.5f);
		m_InvFocalLen[0]   = 1.0f / m_FocalLen[0];
		m_InvFocalLen[1]   = 1.0f / m_FocalLen[1];
		m_InvResolution[0] = 1.0f / width;
		m_InvResolution[1] = 1.0f / height;
		m_Resolution[0]    = (float)width;
		m_Resolution[1]    = (float)height;

		d->SetShaderParameter("g_NumSteps",(float)m_NumSteps);
		d->SetShaderParameter("g_Attenuation",m_Attenuation);
		d->SetShaderParameter("g_FocalLen",Vector2(m_FocalLen[0],m_FocalLen[1]));
		d->SetShaderParameter("g_InvFocalLen",Vector2(m_InvFocalLen[0],m_InvFocalLen[1]));

		d->SetShaderParameter("g_Resolution",Vector2(m_Resolution[0],m_Resolution[1]));
		d->SetShaderParameter("g_InvResolution",Vector2(m_InvResolution[0],m_InvResolution[1]));

	}
	inline float sqr(float x)
	{
		return x * x;
	}

	void PlaygroundDemo::UpdateBlur()
	{
		RenderPass* d = gYume->pRenderer->GetDefaultPass();


		float radius     = m_BlurRadius;
		float sigma      = (radius+1) / 2;
		float inv_sigma2 = 1.0f / (2*sigma*sigma);

		d->SetShaderParameter("g_BlurFalloff",(float)inv_sigma2);
		d->SetShaderParameter("g_BlurRadius",(float)radius);

		d->SetShaderParameter("g_EdgeThreshold",m_EdgeThreshold);
	}

	void PlaygroundDemo::UpdateSharpness()
	{
		RenderPass* d = gYume->pRenderer->GetDefaultPass();

		float sharpness = sqr(m_Sharpness / m_AORadius);
		d->SetShaderParameter("g_Sharpness",sharpness);
	}

	void PlaygroundDemo::UpdateRadius()
	{
		float R = m_RadiusMultiplier * m_AORadius;
		gYume->pRenderer->GetDefaultPass()->SetShaderParameter("g_R",R);
		gYume->pRenderer->GetDefaultPass()->SetShaderParameter("g_inv_R",1.0f / R);
		gYume->pRenderer->GetDefaultPass()->SetShaderParameter("g_sqr_R",R * R);
	}

	void PlaygroundDemo::UpdateAngleBias()
	{
		float angle = m_AngleBias * M_PI/ 180;
		gYume->pRenderer->GetDefaultPass()->SetShaderParameter("g_AngleBias",angle);
		gYume->pRenderer->GetDefaultPass()->SetShaderParameter("g_TanAngleBias",tan(angle));
		UpdateContrast();
	}

	void PlaygroundDemo::UpdateContrast()
	{
		float contrast = m_Contrast / (1.0f - sin(m_AngleBias * M_PI / 180));
		gYume->pRenderer->GetDefaultPass()->SetShaderParameter("g_Contrast",contrast);
	}

	void PlaygroundDemo::UpdateSSAO()
	{
		Rng.seed((unsigned)0);

		gYume->pRenderer->GetDefaultPass()->SetShaderParameter("g_NumDir",(float)m_NumDirs);

		signed short f[64*64*4];
		for(int i=0; i<64*64*4; i+=4)
		{
			float angle = 2.0f*M_PI * Rng.randExc() / (float)m_NumDirs;
			f[i] = (signed short)(SCALE*cos(angle));
			f[i+1] = (signed short)(SCALE*sin(angle));
			f[i+2] = (signed short)(SCALE*Rng.randExc());
			f[i+3] = 0;
		}

		randomMap_->SetData(0,0,0,64,64,f);

	}

	void PlaygroundDemo::CreateCircleOfPointLights(DirectX::XMFLOAT3 origin,float numberOfLights,float rad)
	{
		float radius = rad;
		int numPoints = numberOfLights;
		for(int pointNum = 0; pointNum < numPoints; ++pointNum)
		{
			float i = (pointNum * 1.0f) / numPoints;
			float angle = i * M_PI * 2;
			float xPos =  sinf(angle)*radius + origin.x;
			float zPos =  cosf(angle)*radius + origin.z;

			float r = Random(360);
			float g = Random(360);
			float b = Random(360);
			CreateLight(DirectX::XMFLOAT3(xPos,5,zPos),YumeColor(r / 360.0f,g / 360.0f,b / 360.0f,0),30);
		}
	}

	void PlaygroundDemo::CreateLight(DirectX::XMFLOAT3 pos,YumeColor color,float range)
	{
		Light* pointLight = new Light;
		pointLight->SetName("PointLight");
		pointLight->SetType(LT_POINT);
		pointLight->SetPosition(DirectX::XMVectorSet(pos.x,pos.y,pos.z,0),true);
		pointLight->SetColor(color);
		pointLight->SetRange(range);

		lights_.push_back(pointLight);

		gYume->pRenderer->GetScene()->AddNode(pointLight);
	}

	void PlaygroundDemo::SSAOOffsetVectors()
	{

	}

	void PlaygroundDemo::CreateCube(Vector3 Pos,Quaternion Rot,Vector3 size,YumeColor color)
	{

	}

	StaticModel* PlaygroundDemo::CreateModel(const YumeString& name,DirectX::XMFLOAT3 Pos,DirectX::XMFLOAT4 rot,DirectX::XMFLOAT3 scale)
	{
		StaticModel* sm_ = new StaticModel(name);
		sm_->Initialize();
		sm_->SetName("Cornell");
		sm_->SetPosition(DirectX::XMLoadFloat3(&Pos),true);
		sm_->SetRotation(DirectX::XMLoadFloat4(&rot));
		sm_->SetScale(scale.x,scale.y,scale.z);


		gYume->pRenderer->GetScene()->AddNode(sm_);
		/*gYume->pRenderer->UpdateMeshBb(sm_->getge)*/

		return sm_;
	}

	void PlaygroundDemo::CreateCube(Vector3 Pos,Quaternion Rot,float size,YumeColor color)
	{

	}
	void PlaygroundDemo::CreateSphere(Vector3 Pos,Quaternion Rot,float size,YumeColor color)
	{

	}
	void PlaygroundDemo::CreateCylinder(Vector3 Pos,Quaternion Rot,float size,YumeColor color)
	{

	}
	void PlaygroundDemo::CreatePyramid(Vector3 Pos,Quaternion Rot,float size,YumeColor color)
	{

	}
	void PlaygroundDemo::CreateCone(Vector3 Pos,Quaternion Rot,float size,YumeColor color)
	{
	}
	void PlaygroundDemo::MoveCamera(float timeStep)
	{
	}


	void PlaygroundDemo::HandleUpdate(float timeStep)
	{

	}
	void PlaygroundDemo::HandleRenderUpdate(float timeStep)
	{

	}

	void PlaygroundDemo::Setup()
	{
		BaseApplication::Setup();

		engineVariants_["WindowWidth"] = 1600;
		engineVariants_["WindowHeight"] = 900;
	}
}
