////////////////////////////////////////////////////////////////////////////////
//	Copyright 2015 by Joseph Forgione
//	This file is part of VCC (Virtual Color Computer).
//	
//	VCC (Virtual Color Computer) is free software: you can redistribute it and/or
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
#include "create_disk_image_dialog.h"
#include "resource.h"
#include "vcc/ui/utility.h"
#include <fstream>


namespace vcc::cartridges::fd502
{

	const std::map<
		UINT,
		create_disk_image_dialog::disk_image_format_type> create_disk_image_dialog::disk_type_id_to_enum_map_ =
	{
		{IDC_NEWDISK_JVC_FORMAT, disk_image_format_type::jvc},
		{IDC_NEWDISK_VDK_FORMAT, disk_image_format_type::vdk},
		{IDC_NEWDISK_DMK_FORMAT, disk_image_format_type::dmk}
	};

	const std::map<
		create_disk_image_dialog::disk_image_format_type,
		UINT> create_disk_image_dialog::disk_type_enum_map_to_id_ =
	{
		{disk_image_format_type::jvc, IDC_NEWDISK_JVC_FORMAT},
		{disk_image_format_type::vdk, IDC_NEWDISK_VDK_FORMAT},
		{disk_image_format_type::dmk, IDC_NEWDISK_DMK_FORMAT}
	};

	const std::map<
		UINT,
		create_disk_image_dialog::size_type> create_disk_image_dialog::track_count_id_to_value_ =
	{
		{IDC_NEWDISK_35TRACKS, 35},
		{IDC_NEWDISK_40TRACKS, 40},
		{IDC_NEWDISK_80TRACKS, 80}
	};

	const std::map<
		create_disk_image_dialog::size_type,
		UINT> create_disk_image_dialog::track_count_value_to_id_ =
	{
		{35, IDC_NEWDISK_35TRACKS},
		{40, IDC_NEWDISK_40TRACKS},
		{80, IDC_NEWDISK_80TRACKS}
	};


	create_disk_image_dialog::create_disk_image_dialog(
		HINSTANCE module_instance,
		path_type image_filename)
		:
		dialog_window(module_instance, IDD_CREATE_NEW_DISK),
		image_filename_(std::move(image_filename))
	{
	}


	bool create_disk_image_dialog::on_init_dialog()
	{
		// We don't need the return value as we always return true so keyboard focus
		// is given to the default control (if there is one).
		(void)dialog_window::on_init_dialog();

		::vcc::ui::center_window_to_parent(handle());

		disk_image_layout_ = defaults::image_layout;
		double_sided_ = defaults::double_sided;
		track_count_ = defaults::track_count;

		set_button_check(disk_type_enum_map_to_id_.at(disk_image_layout_), true);
		set_button_check(track_count_value_to_id_.at(track_count_), true);
		set_button_check(IDC_NEWDISK_DOUBLESIDED, double_sided_);
		set_control_text(IDC_NEWDISK_FILENAME, image_filename_.filename());
		update_control_states();

		return TRUE;
	}

	INT_PTR create_disk_image_dialog::on_command(
		WPARAM wParam,
		[[maybe_unused]] LPARAM lParam)
	{

		switch (LOWORD(wParam))
		{
		case IDC_NEWDISK_DMK_FORMAT:
		case IDC_NEWDISK_JVC_FORMAT:
		case IDC_NEWDISK_VDK_FORMAT:
			disk_image_layout_ = disk_type_id_to_enum_map_.at(LOWORD(wParam));
			break;

		case IDC_NEWDISK_35TRACKS:
		case IDC_NEWDISK_40TRACKS:
		case IDC_NEWDISK_80TRACKS:
			track_count_ = track_count_id_to_value_.at(LOWORD(wParam));
			break;

		case IDC_NEWDISK_DOUBLESIDED:
			double_sided_ = is_button_checked(IDC_NEWDISK_DOUBLESIDED);
			break;

		default:
			break;
		}

		update_control_states();

		return TRUE;
	}

	void create_disk_image_dialog::update_control_states()
	{
		EnableWindow(GetDlgItem(handle(), IDC_NEWDISK_80TRACKS), double_sided_);
	}

	void create_disk_image_dialog::on_ok()
	{
		if (track_count_ == 80 && !double_sided_)
		{
			MessageBox(
				handle(),
				"Unable to create disk image. JVC disk images formatted with 80 tracks must be double sided.\n",
				"Format Parameter Error!",
				MB_ICONHAND | MB_OK);

			return;
		}

		if (disk_image_layout_ != disk_image_format_type::jvc)
		{
			MessageBox(
				handle(),
				"Unable to create disk image. DREAM can only create JVC disk images.",
				"Format Parameter Error!",
				MB_ICONHAND | MB_OK);

			return;
		}

		if (std::filesystem::exists(image_filename_))
		{
			const auto result = MessageBox(
				handle(),
				"The disk image already exists. Would you\nlike to override the existing image?",
				"Disk Image Already Exists!",
				MB_ICONQUESTION | MB_YESNO);

			if (result == IDNO)
			{
				return;
			}
		}

		// TODO-CHET: This only create JVC disk images and is temporary until the other
		// disk image formats can be implemented.
		std::basic_ofstream<uint8_t> output_file(image_filename_, std::ios_base::binary);
		if (!output_file.is_open())
		{
			MessageBox(
				handle(),
				("Cannot open \"" + image_filename_.string() + "\"").c_str(),
				"Unable to create disk image!",
				MB_ICONERROR | MB_OK);

			return;
		}

		const auto heads(double_sided_ ? 2 : 1);
		const std::vector<uint8_t> sector_data(defaults::sector_size, 0xff);
		const auto sectors_to_write(heads * track_count_ * defaults::sector_count);
		for (auto lsn(0u); lsn < sectors_to_write; ++lsn)
		{
			output_file.write(sector_data.data(), sector_data.size());
		}

		dialog_window::on_ok();
	}


}
