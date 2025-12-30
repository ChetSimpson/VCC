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
#include "vcc/media/disk_image_file_creators/basic_disk_image_file_creator.h"
#include "vcc/media/detail/vdk_disk_image_header_layout.h"
#include <array>


namespace vcc::media::disk_image_file_creators
{

	/// @brief Disk image file creator for VDK disk images.
	class vdk_disk_image_file_creator : public ::vcc::media::disk_image_file_creators::basic_disk_image_file_creator
	{
	public:

		/// @brief Type alias for the VDK disk image header layout.
		using header_layout_type = ::vcc::media::detail::vdk_disk_image_header_layout;

		/// @brief Inherit the constructor.
		using basic_disk_image_file_creator::basic_disk_image_file_creator;


	protected:


		/// @brief Calculate the size of the image header for a VDK disk image.
		/// 
		/// This function returns the fixed size of the VDK disk image header (without
		/// the variable length name field).
		/// 
		/// @return The size of the image header in bytes.
		LIBCOMMON_EXPORT file_size_type calculate_header_size() const noexcept override;

		/// @brief Write the VDK disk image header to the output stream.
		/// 
		/// @param output The output stream to write the header to.
		/// @param geometry The geometry of the disk image.
		/// @param calculated_header_size The calculated size of the header.
		/// 
		/// @return An error identifier indicating success or failure.
		LIBCOMMON_EXPORT error_id_type write_image_header(
			std::ostream& output,
			const geometry_type& geometry,
			file_size_type calculated_header_size) override;


	private:

		/// @brief A template for the VDK disk image header containing default values.
		static const std::array<unsigned char, header_layout_type::base_size> header_template_;
	};

}
