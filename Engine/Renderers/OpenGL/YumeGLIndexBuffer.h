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
#ifndef __YumeD3D11IndexBuffer_h__
#define __YumeD3D11IndexBuffer_h__
//----------------------------------------------------------------------------
#include "YumeGLRequired.h"
#include "YumeGLGpuResource.h"
#include "Renderer/YumeIndexBuffer.h"
//----------------------------------------------------------------------------
namespace YumeEngine
{
	class YumeRHI;

	class YumeGLExport YumeGLIndexBuffer : public YumeIndexBuffer,public YumeGLResource
	{
	public:
		YumeGLIndexBuffer();
		virtual ~YumeGLIndexBuffer();

		void OnDeviceReset();
		void Release();

		void SetShadowed(bool enable);
		bool SetSize(unsigned vertexCount,bool largeIndices,bool dynamic = false);
		bool SetData(const void* data);
		bool SetDataRange(const void* data,unsigned start,unsigned count,bool discard = false);
		void* Lock(unsigned start,unsigned count,bool discard = false);
		void Unlock();

		bool GetUsedVertexRange(unsigned start,unsigned count,unsigned& minVertex,unsigned& vertexCount);

		bool Create();
		bool UpdateToGPU();
		void* MapBuffer(unsigned start,unsigned count,bool discard) { return 0; };
		void UnmapBuffer() { };
	};
}


//----------------------------------------------------------------------------
#endif
