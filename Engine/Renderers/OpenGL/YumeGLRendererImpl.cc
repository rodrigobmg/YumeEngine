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
// File : YumeGraphics.h
// Date : 2.19.2016
// Comments :
//
//----------------------------------------------------------------------------
#include "YumeHeaders.h"

#include "Renderer/YumeGraphicsApi.h"
#include "YumeGLRendererImpl.h"



namespace YumeEngine
{
    YumeGLRendererImpl::YumeGLRendererImpl() :
      context_(0),
      systemFBO_(0),
      activeTexture_(0),
      enabledAttributes_(0),
      boundFBO_(0),
      boundVBO_(0),
      boundUBO_(0),
      pixelFormat_(0),
      fboDirty_(false)
  {
  }
}
