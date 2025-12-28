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
/// @file
///
/// Contains definitions for disk creator errors.

namespace vcc::media
{

	/// @brief Error identifiers returned by disk image creators.
	///
	/// These values describe the outcome of an attempt to create or initialize
	/// a disk image file. They are used to indicate the specific failure mode
	/// when creation cannot be completed successfully.
	enum class disk_creator_error_id
	{
		/// @brief No error; operation succeeded.
		none,

		/// @brief An unspecified or unknown error occurred.
		unknown,

		/// @brief The creator was unable to create or open the target file.
		///
		/// This typically indicates filesystem permissions, path, or device errors.
		cannot_create_file,

		/// @brief The requested image size or parameters could not be validated.
		///
		/// Used when the creator cannot confirm that the requested layout/size is acceptable.
		cannot_validate_size,

		/// @brief The actual file size does not match the expected size after creation.
		///
		/// Indicates a mismatch between expected and observed sizes; may imply write/truncate issues.
		file_size_mismatch,

		/// @brief A write operation to the target file failed.
		///
		/// Used when an I/O error occurs while writing image data.
		write_error,

		/// @brief The creator failed to resize (truncate/extend) the file as requested.
		///
		/// May indicate filesystem or OS limitations when attempting to set file length.
		cannot_resize,

		/// @brief The creator was unable to seek to a required position in the file.
		///
		/// Indicates failures in repositioning the file pointer prior to read/write/resize.
		cannot_seek
	};

}
