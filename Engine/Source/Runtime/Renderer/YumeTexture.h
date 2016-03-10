//----------------------------------------------------------------------------
//Yume Engine
//Copyright (C) 2015  arkenthera
//This program is free software; you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation; either version 2 of the License, or
//(at your option) any later version.
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//You should have received a copy of the GNU General Public License along
//with this program; if not, write to the Free Software Foundation, Inc.,
//51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.*/
//----------------------------------------------------------------------------
//
// File : <Filename>
// Date : <Date>
// Comments :
//
//----------------------------------------------------------------------------
#ifndef __YumeTexture_h__
#define __YumeTexture_h__
//----------------------------------------------------------------------------
#include "YumeRequired.h"


static const int MAX_TEXTURE_QUALITY_LEVELS = 3;

#include "Math/YumeColor.h"
#include "Renderer/YumeRendererDefs.h"
#include "Renderer/YumeResource.h"
//----------------------------------------------------------------------------
namespace YumeEngine
{
	class YumeAPIExport YumeTexture : public YumeResource
	{
	public:
		YumeTexture();

		virtual ~YumeTexture();


		
		void SetNumLevels(unsigned levels);
		
		void SetFilterMode(TextureFilterMode filter);
		
		void SetAddressMode(TextureCoordinate coord,TextureAddressMode address);
		
		void SetShadowCompare(bool enable) ;
		
		void SetBorderColor(const YumeColor& color);
		
		void SetSRGB(bool enable);
		
		void SetBackupTexture(YumeTexture* texture) ;
		
		void SetMipsToSkip(int quality,int toSkip) ;

		
		unsigned GetFormat() const { return format_; }

		
		virtual bool IsCompressed() const = 0;

		
		unsigned GetLevels() const { return levels_; }

		
		int GetWidth() const { return width_; }

		
		int GetHeight() const { return height_; }

		
		int GetDepth() const { return depth_; }

		
		TextureFilterMode GetFilterMode() const { return filterMode_; }

		
		TextureAddressMode GetAddressMode(TextureCoordinate coord) const { return addressMode_[coord]; }

		
		bool GetShadowCompare() const { return shadowCompare_; }

		
		const YumeColor& GetBorderColor() const { return borderColor_; }

		
		bool GetSRGB() const { return sRGB_; }

		
		YumeTexture* GetBackupTexture() const { return backupTexture_.get(); }

		
		int GetMipsToSkip(int quality) const;
		
		int GetLevelWidth(unsigned level) const;
		
		int GetLevelHeight(unsigned level) const;
		
		int GetLevelDepth(unsigned level) const;

		
		TextureUsage GetUsage() const { return usage_; }

		
		virtual unsigned GetDataSize(int width,int height) const = 0;
		
		unsigned GetDataSize(int width,int height,int depth) const;
		
		virtual unsigned GetRowDataSize(int width) const = 0;
		
		unsigned GetComponents() const;

		
		bool GetParametersDirty() const { return parametersDirty_ || !sampler_; }

		
		void SetParameters();
		
		void SetParametersDirty();
		
		virtual void UpdateParameters() = 0;

		
		void* GetShaderResourceView() const { return shaderResourceView_; }

		
		void* GetSampler() const { return sampler_; }


		
		virtual void CheckTextureBudget(YumeHash type) = 0;

		
		static unsigned CheckMaxLevels(int width,int height,unsigned requestedLevels);
		
		static unsigned CheckMaxLevels(int width,int height,int depth,unsigned requestedLevels);
		
		virtual unsigned GetSRVFormat(unsigned format) = 0;
		
		virtual unsigned GetDSVFormat(unsigned format) = 0;
		
		virtual unsigned GetSRGBFormat(unsigned format) = 0;
	protected:
		
		void* shaderResourceView_;
		//OpenGL target
		unsigned target_;
		
		void* sampler_;
		
		unsigned format_;
		
		TextureUsage usage_;
		
		unsigned levels_;
		
		unsigned requestedLevels_;
		
		int width_;
		
		int height_;
		
		int depth_;
		
		bool shadowCompare_;
		
		TextureFilterMode filterMode_;
		
		TextureAddressMode addressMode_[MAX_COORDS];
		
		unsigned mipsToSkip_[MAX_TEXTURE_QUALITY_LEVELS];
		
		YumeColor borderColor_;
		
		bool sRGB_;
		
		bool parametersDirty_;
		
		SharedPtr<YumeTexture> backupTexture_;

	};
}


//----------------------------------------------------------------------------
#endif
