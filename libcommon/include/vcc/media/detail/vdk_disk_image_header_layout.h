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
#pragma once


namespace vcc::media::detail
{

	/// @brief Defines the offsets into the header data of a VDK disk image.
	struct vdk_disk_image_header_layout
	{
		/// @brief Specifies the number of header bytes in a VDK disk image.
		static constexpr auto base_size = 0x0cu;

		/// @brief Defines the offsets of fields within the VDK disk image header.
		struct field_offsets
		{
			/// @brief The offset into the header data containing the most significant byte of
			/// the file signature. This value is always 'd' (hex 0x64).
			static constexpr auto signature_msb = 0u;
			/// @brief The offset into the header containing the least significant byte of
			/// the file signature. This value is always 'k' (hex 0x6B).
			static constexpr auto signature_lsb = 1u;
			/// @brief The offset into the header containing the size of the header.
			static constexpr auto header_size = 2u;
			/// @brief The offset into the header containing the LSB of the header size.
			static constexpr auto header_size_lsb = 2u;
			/// @brief The offset into the header containing the MSB of the header size.
			static constexpr auto header_size_msb = 3u;
			/// @brief The offset into the header containing the disk image format.
			static constexpr auto version = 4u;
			/// @brief The offset into the header containing the version compatibility information
			static constexpr auto compatibility_version = 5u;
			/// @brief The offset into the header containing the identify of the file source.
			static constexpr auto identify_of_file_source = 6u;
			/// @brief The offset into the header containing the version of the file source.
			static constexpr auto version_of_file_source = 7u;
			/// @brief The offset into the header containing the number of tracks.
			static constexpr auto number_of_tracks = 8u;
			/// @brief The offset into the header containing the number of heads.
			static constexpr auto number_of_head = 9u;
			/// @brief The offset into the header containing the flags.
			/// @todo Document the meaning of the flags.
			static constexpr auto flags = 10u;
			/// @brief The offset into the header containing the compression flags and name length.
			static constexpr auto name_and_compression = 11u;
		};

		/// @brief Defines constant values used in the VDK disk image header fields.
		struct values
		{
			/// @brief The expected most significant byte of the file signature.
			static constexpr auto signature_msb = 0x64u; // 'd'
			/// @brief The expected least significant byte of the file signature.
			static constexpr auto signature_lsb = 0x6Bu; // 'k'
			/// @brief The current version of the VDK disk image format known to the API.
			static constexpr auto current_version = 0x10u;
		};

		/// @brief Defines bit masks for the flags2 field in the VDK disk image header.
		struct name_and_compression
		{
			/// @brief The bit mask for the compression flag in the flags2 field.
			static constexpr auto compression_mask = 0b00000111u;
			/// @brief The bit mask for the name length in the flags2 field.
			static constexpr auto name_length_mask = 0b11111000u;
			/// @brief The number of bits to shift the name length to the right to get
			/// its actual value.
			static constexpr auto name_length_shift = 3u;
		};
	};

}
