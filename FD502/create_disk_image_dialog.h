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
#pragma once
#include "floppy_disk_layout.h"
#include "vcc/ui/dialog_window.h"
#include <map>


namespace vcc::cartridges::fd502
{

	/// @brief Dialog window for creating new disk images.
	/// 
	/// This class provides the user interface for creating a new disk image. It supports
	/// creating disks in a variety of formats including DSK and DMK. In addition to the
	/// variety of formats the user is also able to select the number of tracks and whether
	/// the disk is single or double sided.
	class create_disk_image_dialog : public ::vcc::ui::dialog_window
	{
	public:

		/// @brief Construct the Create Disk Image dialog.
		/// 
		/// @param module_instance The instance of the library module containing the dialog
		/// resources.
		/// @param image_filename The filename of the disk image to create.
		create_disk_image_dialog(
			HINSTANCE module_instance,
			path_type image_filename);

		/// @brief Gets the filename of the disk image to create.
		path_type image_filename() const
		{
			return image_filename_;
		}


	protected:

		/// @inheritdoc
		bool on_init_dialog() override;

		/// @inheritdoc
		void on_ok() override;

		/// @inheritdoc
		INT_PTR on_command(WPARAM wParam, LPARAM lParam) override;

		/// @brief Updates the enabled state of controls based on settings.
		void update_control_states();


	private:

		/// @brief Type alias for disk image format identifiers.
		using disk_image_format_type = ::vcc::cartridges::fd502::detail::disk_image_format_id;

		/// @brief Defines default values for disk creation settings.
		struct defaults
		{
			/// @brief The disk image layout used to format the disk image.
			static const auto image_layout = disk_image_format_type::dsk;
			/// @brief Defines the default setting for creating double sided disk images.
			static const auto double_sided = false;
			/// @brief The number of tracks.
			static const auto track_count = 35u;
			/// @brief The number of sectors per track.
			static const auto sector_count = 18u;
			/// @brief The number of bytes per sector.
			static const auto sector_size = 256u;
		};

		/// @brief Defines the details associated with a disk image format selection in the 
		/// dialog.
		struct disk_image_selection_detail
		{
			/// @brief Type alias for UI control identifiers.
			UINT control_id;
			/// @brief Type alias for disk image format identifiers.
			disk_image_format_type format_type;
			/// @brief Type alias for file extensions.
			string_type extension;

			/// @brief Conversion operator to allow easy insertion into maps with UINT keys.
			operator std::pair<const UINT, disk_image_selection_detail>() const
			{
				return { control_id, *this };
			}

			/// @brief Conversion operator to allow easy insertion into maps with disk image format keys.
			operator std::pair<const disk_image_format_type, disk_image_selection_detail>() const
			{
				return { format_type, *this };
			}

			/// @brief conversion operator to allow easy insertion into maps with file extension keys.
			operator std::pair<const string_type, disk_image_selection_detail>() const
			{
				return { extension, *this };
			}
		};

		/// @brief Type alias for maps used to convert between UI control identifiers and
		/// disk image selection details.
		using disk_control_id_to_details_map_type = std::map<UINT, disk_image_selection_detail>;

		/// @brief Type alias for maps used to convert between disk image format identifiers and
		/// disk image selection details.
		using image_type_id_to_details_map_type = std::map<disk_image_format_type, disk_image_selection_detail>;

		/// @brief Type alias for maps used to convert between file extensions and
		/// disk image selection details.
		using file_extension_to_details_map_type = std::map<string_type, disk_image_selection_detail>;


	private:

		/// @brief A static read-only vector holding disk image selection details.
		static const std::vector<disk_image_selection_detail> disk_selection_details_;
		/// @brief Table used for converting UI control identifiers to disk image format
		/// identifiers.
		static const disk_control_id_to_details_map_type disk_control_id_to_details_map_;
		/// @brief Table used for converting disk image format identifiers to UI control 
		/// identifiers.
		static const image_type_id_to_details_map_type image_type_id_to_details_map_;
		/// @brief Table used for converting file extensions to disk image format
		/// identifiers.
		static const file_extension_to_details_map_type file_extension_to_details_map_;
		/// @brief Table used for converting UI control identifiers to track count values.
		static const std::map<UINT, size_type> track_count_id_to_value_;
		/// @brief Table used for converting track count values to UI control identifiers.
		static const std::map<size_type, UINT> track_count_value_to_id_;

		/// @brief The filename of the disk image to create.
		path_type image_filename_;
		/// @brief The disk image format used to create the disk image.
		disk_image_format_type disk_image_format_ = defaults::image_layout;
		/// @brief Flag indicating if the disk image should be created as a double sided
		/// disk.
		bool double_sided_ = defaults::double_sided;
		/// @brief The number of tracks the disk image will contain.
		size_type track_count_ = defaults::track_count;
		/// @brief Specifies if the file extension can be changed based on the selected
		/// disk image format.
		bool allow_extension_change_ = false;
	};

}
