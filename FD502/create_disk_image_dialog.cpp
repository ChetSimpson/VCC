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
#include "vcc/media/disk_image_file_creators/basic_disk_image_file_creator.h"
#include "vcc/media/disk_image_file_creators/vdk_disk_image_file_creator.h"
#include <fstream>


namespace vcc::cartridges::fd502
{

	const std::vector<create_disk_image_dialog::disk_image_selection_detail>
		create_disk_image_dialog::disk_selection_details_ =
	{
		{ IDC_NEWDISK_DSK_FORMAT, disk_image_format_type::dsk, ".dsk" },
		{ IDC_NEWDISK_VDK_FORMAT, disk_image_format_type::vdk, ".vdk" },
		{ IDC_NEWDISK_DMK_FORMAT, disk_image_format_type::dmk, ".dmk" }
	};

	const create_disk_image_dialog::disk_control_id_to_details_map_type
		create_disk_image_dialog::disk_control_id_to_details_map_(
			disk_selection_details_.begin(),
			disk_selection_details_.end());

	const create_disk_image_dialog::image_type_id_to_details_map_type
		create_disk_image_dialog::image_type_id_to_details_map_(
			disk_selection_details_.begin(),
			disk_selection_details_.end());

	const create_disk_image_dialog::file_extension_to_details_map_type
		create_disk_image_dialog::file_extension_to_details_map_(
			disk_selection_details_.begin(),
			disk_selection_details_.end());

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

		disk_image_format_ = defaults::image_layout;
		double_sided_ = defaults::double_sided;
		track_count_ = defaults::track_count;
		allow_extension_change_ = true;

		// If the filename has an extension we want to determine the image type associated
		// with it and select the control that represents it. 
		if (image_filename_.has_extension())
		{
			auto extension(image_filename_.extension().string());
			std::transform(
				extension.begin(),
				extension.end(),
				extension.begin(),
				[](unsigned char c) { return std::tolower(c); });

			if(const auto details(file_extension_to_details_map_.find(extension));
			   details != file_extension_to_details_map_.end())
			{
				disk_image_format_ = details->second.format_type;
			}
			else
			{
				// An extension has been provided but it's not one known to be used with
				// a disk image file so we prevent it from being changed in the filename
				// if a different format is selected.
				allow_extension_change_ = false;
			}
		}
		else
		{
			// There is no extension so we use the one from the initial image format.
			image_filename_.replace_extension(image_type_id_to_details_map_.at(disk_image_format_).extension);
		}

		set_button_check(image_type_id_to_details_map_.at(disk_image_format_).control_id, true);
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
		switch (const auto command_id(LOWORD(wParam)); command_id)
		{
		case IDC_NEWDISK_DSK_FORMAT:
		case IDC_NEWDISK_VDK_FORMAT:
		case IDC_NEWDISK_DMK_FORMAT:
			disk_image_format_ = disk_control_id_to_details_map_.at(command_id).format_type;
			// Replace the file extension in the filename if allowed.
			if (allow_extension_change_)
			{
				const auto& details = image_type_id_to_details_map_.at(disk_image_format_);

				image_filename_.replace_extension(details.extension);
				set_control_text(IDC_NEWDISK_FILENAME, image_filename_.filename());
			}
			break;

		case IDC_NEWDISK_35TRACKS:
		case IDC_NEWDISK_40TRACKS:
		case IDC_NEWDISK_80TRACKS:
			track_count_ = track_count_id_to_value_.at(command_id);
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

		if (std::filesystem::exists(image_filename_))
		{
			const auto result = MessageBox(
				handle(),
				"The disk image already exists. Would you\nlike to overwrite the existing image?",
				"Disk Image Already Exists!",
				MB_ICONQUESTION | MB_YESNO);

			if (result == IDNO)
			{
				return;
			}
		}

		using ::vcc::media::disk_image_file_creators::basic_disk_image_file_creator;
		using ::vcc::media::disk_image_file_creators::vdk_disk_image_file_creator;
		using ::vcc::media::disk_image_file_creator;

		// TODO-CHET: This only create JVC disk images and is temporary until the other
		// disk image formats can be implemented.
		std::unique_ptr<disk_image_file_creator> image_creator;
		switch (disk_image_format_)
		{
		case disk_image_format_type::dsk:
			image_creator = std::make_unique<basic_disk_image_file_creator>(
				defaults::sector_count,
				defaults::sector_size);
			break;

		case disk_image_format_type::vdk:
			image_creator = std::make_unique<vdk_disk_image_file_creator>(
				defaults::sector_count,
				defaults::sector_size);
			break;

		case disk_image_format_type::dmk:
			// TODO-CHET: Add when DMK format is supported.
			break;
		}

		if (image_creator == nullptr)
		{
			MessageBox(
				handle(),
				"Unable to create disk image. DREAM can only create disk images of the type you selected.",
				"Format Parameter Error!",
				MB_ICONHAND | MB_OK);

			return;
		}

		using error_id_type = disk_image_file_creator::error_id_type;

		disk_image_file_creator::geometry_type geometry;
		geometry.head_count(double_sided_ ? 2 : 1);
		geometry.track_count(track_count_);
		
		if (const auto create_result(image_creator->create(image_filename_, geometry));
			create_result != error_id_type::none)
		{
			auto error_message("Cannot create disk image \"" + image_filename_.string() + "\"\n\n");
			switch(create_result)
			{
			case error_id_type::unknown:
				error_message += "An unknown error occurred while creating the disk image.";
				break;

			case error_id_type::cannot_create_file:
				error_message += "Unable to create file.";
				break;

			case error_id_type::cannot_validate_size:
				error_message += "The disk image was created but size of the file cannot be validated.";
				break;

			case error_id_type::file_size_mismatch:
				error_message += "The disk image was created but the size of the file is not what is expected.";
				break;

			case error_id_type::cannot_write:
				error_message += "Unable to write to disk image file.";
				break;

			case error_id_type::cannot_resize:
				error_message += "The disk image file was created but its size could not be set.";
				break;

			case error_id_type::cannot_seek:
				error_message += "Unable to seek within the disk image file.";
				break;
			}

			MessageBox(
				handle(),
				error_message.c_str(),
				"Unable to create disk image!",
				MB_ICONERROR | MB_OK);

			return;
		}


		dialog_window::on_ok();
	}

}
