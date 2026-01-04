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
#include "vcc/media/disk_image.h"
#include "vcc/media/disk_image_factory_error_id.h"
#include <iostream>
#include <memory>
#include <array>


namespace vcc::media
{

	/// @brief Abstract factory for creating disk image instances.
	///
	/// This factory provides a way to create disk image instances without specifying the
	/// exact class of the object that will be created.
	class LIBCOMMON_EXPORT disk_image_abstract_factory
	{
	public:

		/// @brief Type alias for the disk image created by the factory.
		using disk_image_type = ::vcc::media::disk_image;
		/// @brief Type alias for the stream type used to access the disk image file.
		using stream_type = std::iostream;
		/// @brief Type alias of the managed pointer to a stream used to access the disk
		/// image file.
		using stream_ptr_type = std::unique_ptr<stream_type>;
		/// @brief Type alias to lengths, positions, and offsets within a stream.
		using stream_size_type = stream_type::pos_type;
		/// @brief TYpe alias for the header buffer used to create the disk image.
		using header_buffer_type = std::array<unsigned char, 128>;
		/// @brief Type alias for error condition identifiers returned the create
		/// function.
		using error_id_type = disk_image_factory_error_id;


	public:

		virtual ~disk_image_abstract_factory() = default;

		/// @brief Create a disk image instance.
		/// 
		/// Creates a disk image instance using the provided stream, header buffer and
		/// other parameters. The function will use the contents of the header and stream
		/// size to determine the appropriate disk image type to create. If the disk image
		/// is not supported by this factory, a null pointer is returned and the
		/// `error_condition` parameter is set.
		/// 
		/// @param stream The stream used to access the disk image file.
		/// @param stream_size The size of the stream in bytes.
		/// @param header_buffer The header buffer containing the disk image header.
		/// @param write_protected Whether the disk image is write-protected.
		/// @param error_condition The error condition to set if the creation fails.
		/// 
		/// @return A unique pointer to the created disk image, or null if creation failed.
		[[nodiscard]] virtual std::unique_ptr<disk_image_type> create(
			stream_ptr_type& stream,
			const stream_size_type& stream_size,
			const header_buffer_type& header_buffer,
			bool write_protected,
			error_id_type& error_condition) const = 0;
	};

}
