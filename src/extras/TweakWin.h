//////////////////////////////////////////////////////////////////////////////
// This file is part of the Maple Engine                              		//
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ImGuiWin.h"

namespace maple
{
	class TweakWin : public ImGuiWin
	{
	public:
		virtual auto render() -> void;
	};

	class ObjectsWin : public ImGuiWin
	{
	public:
		virtual auto render() -> void;
	private:
	};
};