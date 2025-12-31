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
#include <cstdint>


namespace vcc::media
{

	/// @brief Describes the byte layout of a disk image file.
	///
	/// This struct records offsets and sizes (in bytes) for common regions of a
	/// disk image such as an optional header and footer, the track data area,
	/// and the overall image/file sizes.
	struct disk_image_file_layout
	{
		/// @brief Integer type used to represent sizes (in bytes).
		using size_type = std::uintmax_t;
		/// @brief Integer type used to represent offsets (in bytes).
		using offset_type = std::uintmax_t;

		/// @brief Offset, in bytes, from the start of the file to the image header.
		/// Defaults to 0 when no header is present or unknown.
		offset_type header_offset = 0;
		/// @brief Size, in bytes, of the image header. Zero when no header exists.
		size_type header_size = 0;

		/// @brief Offset, in bytes, from the start of the file to the image footer.
		/// Zero when no footer is present or unknown.
		offset_type footer_offset = 0;
		/// @brief Size, in bytes, of the image footer. Zero when no footer exists.
		size_type footer_size = 0;

		/// @brief Size, in bytes, of a single track or other track-aligned block.
		/// Interpretation depends on the specific image format/creator.
		size_type track_size = 0;
		/// @brief Total size, in bytes, of the logical disk image data region
		/// (typically the area containing tracks/sectors).
		size_type image_size = 0;
		/// @brief Size, in bytes, of the underlying file on disk.
		/// May be equal to header_size + image_size + footer_size, but can differ
		/// for formats with padding or metadata.
		size_type file_size = 0;
	};

}
