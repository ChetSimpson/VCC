////////////////////////////////////////////////////////////////////////////////
// Copyright 2015 by Joseph Forgione
// Copyright 2026 by Chet Simpson
// 
// This file is part of DREAM! The Tandy Color Computer Emulator.
//	
// DREAM! is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//	
// DREAM! is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//	
// You should have received a copy of the GNU General Public License along with
// DREAM!. If not, see <http://www.gnu.org/licenses/>.
////////////////////////////////////////////////////////////////////////////////
#include "vcc/media/disk_image_file_creators/vdk_disk_image_file_creator.h"
#include <limits>
#include <cstdint>


namespace vcc::media::disk_image_file_creators
{
	
	// Template for the VDK disk image header borrowed from XRoar
	const std::array<
		unsigned char,
		vdk_disk_image_file_creator::header_layout_type::base_size> vdk_disk_image_file_creator::header_template_ =
	{
		// 'dk' magic
		header_layout_type::values::signature_msb,
		header_layout_type::values::signature_lsb,
		0x00, 0x00,  // header length, populated later (LSB @ offset 2, MSB @ offset 3)
		header_layout_type::values::current_version,  // VDK version
		header_layout_type::values::current_version,  // VDK backwards compatibility version
		0x00,  // file source
		0x00,  // version of file source
		0x00,  // number of tracks, populated later
		0x00,  // number of heads, populated later
		0x00,  // flags
		0x00,  // name length & compression flag, populated later
	};


	vdk_disk_image_file_creator::file_size_type vdk_disk_image_file_creator::calculate_header_size() const noexcept
	{
		return header_layout_type::base_size;
	}

	vdk_disk_image_file_creator::error_id_type vdk_disk_image_file_creator::write_image_header(
		std::ostream& output,
		const geometry_type& geometry,
		file_size_type calculated_header_size)
	{
		static_assert(
			vdk_disk_image_file_creator::header_template_.size() <= std::numeric_limits<std::uint16_t>::max(),
			"VDK disk image header size exceeds maximum storable size.");

		if (calculated_header_size != header_layout_type::base_size)
		{
			throw std::invalid_argument("VDK disk image header size mismatch.");
		}

		if (geometry.head_count() > std::numeric_limits<std::uint8_t>::max())
		{
			throw std::invalid_argument("VDK disk image head count exceeds maximum storable value.");
		}

		if (geometry.track_count() > std::numeric_limits<std::uint8_t>::max())
		{
			throw std::invalid_argument("VDK disk image track count exceeds maximum storable value.");
		}

		auto header = header_template_;
		using field_offsets = header_layout_type::field_offsets;
		// Store header size as a 16-bit value and split into LSB/MSB according to layout
		const std::uint16_t header_size = static_cast<std::uint16_t>(header.size());
		header[field_offsets::header_size_lsb] = header_size & 0xFFu;
		header[field_offsets::header_size_msb] = (header_size >> 8) & 0xFFu;

		// Populate geometry fields
		header[field_offsets::number_of_tracks] = geometry.track_count() & 0xFFu;
		header[field_offsets::number_of_head] = geometry.head_count() & 0xFFu;

		// Ensure flags / name/compression reserved bytes are zero (explicit for clarity)
		header[field_offsets::flags] = 0x00;
		header[field_offsets::name_and_compression] = 0x00;

		// Write header and check stream state explicitly
		output.write(
			reinterpret_cast<const char*>(header.data()),
			header.size());
		if (!output)
		{
			return error_id_type::cannot_write;
		}

		return error_id_type::none;
	}

}
