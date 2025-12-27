////////////////////////////////////////////////////////////////////////////////
//	Copyright 2015 by Joseph Forgione
//	This file is part of VCC (Virtual Color Computer).
//	
//	VCC (Virtual Color Computer) is free software: you can redistribute itand/or
//	modify it under the terms of the GNU General Public License as published by
//	the Free Software Foundation, either version 3 of the License, or (at your
//	option) any later version.
//	
//	VCC (Virtual Color Computer) is distributed in the hope that it will be
//	useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
//	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
//	Public License for more details.
//	
//	You should have received a copy of the GNU General Public License along with
//	VCC (Virtual Color Computer). If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////
#pragma once
/// @file
/// 
/// @brief Defines utility functions for UI operations.
#include "vcc/detail/exports.h"
#include <Windows.h>


namespace vcc::ui
{

	/// @brief Closes a specified dialog or force exits the application.
	///
	/// Closes a specific dialog window or terminates the application if closing the window
	/// fails.
	/// @param window_handle The window to close.
	LIBCOMMON_EXPORT void close_cartridge_dialog_window(HWND window_handle);


	/// @brief Center a window to its parent window.
	/// 
	/// @param window_handle The window to center.
	LIBCOMMON_EXPORT void center_window_to_parent(HWND window_handle);

}
