//////////////////////////////////////////////////////////////////////////////
// This file is part of the PharaohStroy MMORPG client                      //
// Copyright ?2020-2022 Prime Zeng                                          //
//                                                                          //
// This program is free software: you can redistribute it and/or modify     //
// it under the terms of the GNU Affero General Public License as           //
// published by the Free Software Foundation, either version 3 of the       //
// License, or (at your option) any later version.                          //
//                                                                          //
// This program is distributed in the hope that it will be useful,          //
// but WITHOUT ANY WARRANTY; without even the implied warranty of           //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            //
// GNU Affero General Public License for more details.                      //
//                                                                          //
// You should have received a copy of the GNU Affero General Public License //
// along with this program.  If not, see <http://www.gnu.org/licenses/>.    //
//////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>

#include <vector>

namespace maple
{
	class ImGuiWin 
	{
	public:
		using Ptr = std::shared_ptr<ImGuiWin>;
		virtual auto render() -> void = 0;
	};

	class ImGuiWinManager
	{
	public:
		static auto render() -> void;
		static auto add(ImGuiWin::Ptr)-> void;
		static auto get()->ImGuiWinManager&;
	private:
		std::vector<ImGuiWin::Ptr> windows;
	};
}