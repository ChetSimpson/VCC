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
#include "vcc/media/disk_image_abstract_factory.h"


namespace vcc::media::disk_image_abstract_factories
{

	/// @brief Disk image factory for VDK disk images.
	class LIBCOMMON_EXPORT vdk_disk_image_abstract_factory : public ::vcc::media::disk_image_abstract_factory
	{
	public:

		/// @brief Create a VDK disk image instance.
		/// 
		/// This function examines the header buffer to validate that it contains a valid
		/// disk image header. If the header is valid, a disk image instance is created
		/// and returned.
		/// 
		/// @param stream The stream used to access the disk image file.
		/// @param stream_size The size of the stream in bytes.
		/// @param header_buffer The header buffer containing the disk image header.
		/// @param write_protected Whether the disk image is write-protected.
		/// @param error_condition The error condition to set if the creation fails.
		/// 
		/// @return A unique pointer to the created disk image, or null if creation failed.
		[[nodiscard]] std::unique_ptr<disk_image_type> create(
			stream_ptr_type& stream,
			const stream_size_type& stream_size,
			const header_buffer_type& header_buffer,
			bool write_protected,
			error_id_type& error_condition) const override;


	protected:

		/// @brief Check if the header buffer contains a valid VDK disk image header.
		/// 
		/// This function checks the signature fields and version fields for valid values
		/// and validates that the size of the stream matches the expected size based on the
		/// header layout, number of tracks, number of sectors, and sector size.
		/// 
		/// @param header_buffer The header buffer to check.
		/// @param stream_size The size of the stream.
		/// 
		/// @return An error identifier indicating success or failure.
		[[nodiscard]] error_id_type check(
			const header_buffer_type& header_buffer,
			const stream_size_type& stream_size) const;


	private:

		/// @brief The size in bytes of a sector.
		static constexpr auto sector_size_ = 256u;
		/// @brief The number of sectors per track.
		static constexpr auto sector_count_ = 18u;
		/// @brief The first valid sector identifier.
		static constexpr auto first_valid_sector_id_ = 1u;
	};

}
