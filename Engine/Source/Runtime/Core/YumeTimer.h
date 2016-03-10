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
// File : <Filename> YumeGraphics.h
// Date : 2.19.2016
// Comments :
//
//----------------------------------------------------------------------------
#ifndef __YumeTimer_h__
#define __YumeTimer_h__
//----------------------------------------------------------------------------
#include "YumeRequired.h"

#include "YumeEventHub.h"
#include <boost/timer.hpp>

//----------------------------------------------------------------------------
namespace YumeEngine
{
	
	class YumeAPIExport YumeLowresTimer
	{
	public:
		
		YumeLowresTimer();

		
		unsigned GetMSec(bool reset);
		
		void Reset();

	private:
		
		unsigned startTime_;
	};

	class YumeAPIExport YumeHiresTimer
	{
		friend class YumeTime;

	public:
		
		YumeHiresTimer();

		
		long long GetUSec(bool reset);
		
		void Reset();

		
		static bool IsSupported() { return supported; }

		
		static long long GetFrequency() { return frequency; }

	private:
		
		long long startTime_;

		
		static bool supported;
		
		static long long frequency;
	};

	class YumeAPIExport YumeTimerEventListener
	{
	public:
		virtual ~YumeTimerEventListener() { };

		virtual void HandleBeginFrame(int frameNumber) { };
		virtual void HandleEndFrame(int frameNumber) { };
	};

	class YumeAPIExport YumeTime
	{
	public:
		YumeTime();
		~YumeTime();

		void BeginFrame(float dt);
		void EndFrame();

		static void Sleep(unsigned mSce);
		int GetFrameNumber() const { return frameNumber_; }
		float GetTimeStep() const { return timeStep_; }

		YumeString GetTimeStamp();
		double GetElapsedTime();

		void fireFrameBegin(int frameNumber);
		void fireFrameEnd(int frameNumber);
		
	protected:
		typedef YumeVector<YumeTimerEventListener*>::type TimeEventListeners;
		TimeEventListeners listenerList_;
	public:
		void AddTimeEventListener(YumeTimerEventListener* listener);
		void RemoveTimeEventListener(YumeTimerEventListener* listener);
	protected:
		int frameNumber_;
		double timeStep_;
		boost::timer totalTime_;
	};
}

//----------------------------------------------------------------------------
#endif
