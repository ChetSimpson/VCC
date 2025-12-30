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
#include "vcc/media/disk_geometry.h"
#include "vcc/media/disk_creator_error_id.h"
#include "vcc/detail/exports.h"
#include <filesystem>
#include <memory>


namespace vcc::media
{

	/// @brief Abstract interface for creating disk image files.
	///
	/// Implementations of this interface are responsible for creating and initializing
	/// disk image files according to a specified geometry. The `create` method returns
	/// an `error_id_type` indicating success or the specific failure mode.
	class LIBCOMMON_EXPORT disk_image_creator
	{
	public:

		/// @brief Type used to represent file system paths.
		using path_type = std::filesystem::path;
		/// @brief Type representing disk geometry parameters.
		using geometry_type = ::vcc::media::disk_geometry;
		/// @brief Error identifier type returned by operations.
		using error_id_type = disk_creator_error_id;

		/// @brief Virtual destructor.
		///
		/// Ensures derived destructors are invoked correctly through base
		/// pointers.
		virtual ~disk_image_creator() = default;

		/// @brief Create a disk image file at the specified path using the given geometry.
		///
		/// Implementations should create and initialize the file identified by
		/// `file_path` to match `geometry`. The function returns an
		/// `error_id_type` indicating success (`error_id_type::none`) or the
		/// specific failure reason.
		///
		/// @param file_path Filesystem path where the disk image should be created.
		/// @param geometry Geometry describing the disk image layout to create.
		///
		/// @return An `error_id_type` value describing the outcome of the operation.
		virtual error_id_type create(
			const path_type& file_path,
			const geometry_type& geometry) = 0;
	};

}
