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
#include "vcc/ui/utility.h"


namespace vcc::ui
{
	//------------------------------------------------------------
	// Center a dialog box in parent window
	//------------------------------------------------------------
	void center_window_to_parent(HWND window_handle)
	{
		RECT rPar;
		GetWindowRect(GetParent(window_handle), &rPar);

		RECT rDlg;
		GetWindowRect(window_handle, &rDlg);

		const auto x = rPar.left + (rPar.right - rPar.left - (rDlg.right - rDlg.left)) / 2;
		const auto y = rPar.top + (rPar.bottom - rPar.top - (rDlg.bottom - rDlg.top)) / 2;
		SetWindowPos(window_handle, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

	void close_cartridge_dialog_window(HWND window_handle)
	{
		if (window_handle)
		{
			if (IsWindowEnabled(window_handle))
			{
				SendMessage(window_handle, WM_CLOSE, 0, 0);
			}
			else
			{
				MessageBox(
					nullptr,
					"A system dialog was left open. VCC will close.",
					"Unload Cartridge Error",
					MB_ICONEXCLAMATION);
				DWORD pid = GetCurrentProcessId();
				HANDLE h = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
				TerminateProcess(h, 0);
			}
		}
	}

}
