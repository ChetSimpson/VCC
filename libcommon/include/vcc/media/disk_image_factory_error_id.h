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
/// @file
///
/// Contains definitions for disk image factory errors.


namespace vcc::media
{

	/// @brief Represents error codes produced by a disk image factory when parsing or
	/// loading an image.
	enum class disk_image_factory_error_id
	{
		/// @brief No error occurred.
		none,
		/// @brief The disk image signature is invalid.
		invalid_signature,
		/// @brief The disk image version is unsupported.
		unsupported_version,
		/// @brief The disk image header size is invalid.
		invalid_header_size,
		/// @brief The disk image header size is not what was expected.
		header_size_mismatch,
		/// @brief An error occurred during a file I/O operation.
		file_io_error
	};

}
