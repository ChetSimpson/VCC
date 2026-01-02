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
#include "vcc/detail/exports.h"
#include <cstddef>
#include <stdexcept>


namespace vcc::media
{

	/// @brief Represents the geometry of a disk device or image.
	/// 
	/// The disk geometry encapsulates the number of heads (sides) and the number
	/// of tracks per head for a floppy-style disk. Instances of this class are
	/// small value objects and perform basic validation to ensure the geometry is
	/// meaningful (head and track counts must be greater than 0).
	/// 
	/// Example:
	/// @code
	/// vcc::media::disk_geometry geom(2 /* heads */, 80 /* tracks */);
	/// auto heads = geom.head_count();
	/// auto tracks = geom.track_count();
	/// @endcode
	class LIBCOMMON_EXPORT disk_geometry
	{
	public:

		/// @brief Type alias used for counts and sizes in the geometry.
		using size_type = std::size_t;


	public:

		/// @brief Default-constructs a disk geometry with default values.
		///
		/// Default values are 1 head and 35 tracks. Use the parameterized
		/// constructor or the mutators to change the geometry.
		disk_geometry() = default;

		/// @brief Construct a disk geometry with the specified head and track counts.
		/// 
		/// @param heads Number of heads (sides) on the disk. Must be greater than 0.
		/// @param tracks Number of tracks per head. Must be greater than 0.
		/// 
		/// @throws std::invalid_argument if @p heads or @p tracks is zero.
		disk_geometry(size_type heads, size_type tracks)
			:
			head_count_(heads),
			track_count_(tracks)
		{
			if (head_count_ == 0)
			{
				throw std::invalid_argument("Unable to create disk geometry. Head count cannot be zero.");
			}

			if(track_count_ == 0)
			{
				throw std::invalid_argument("Unable to create disk geometry. Track count cannot be zero.");
			}
		}


		/// @brief Set the number of heads (sides) for the disk geometry.
		/// 
		/// @param value New head count. Must be greater than 0.
		/// 
		/// @throws std::invalid_argument if @p value is zero.
		void head_count(size_type value)
		{
			if (value == 0)
			{
				throw std::invalid_argument("Unable to set head count. Head count cannot be zero.");
			}

			head_count_ = value;
		}

		/// @brief Retrieve the configured number of heads (sides).
		/// 
		/// @return The number of heads. Guaranteed to be greater than 0.
		[[nodiscard]] size_type head_count() const noexcept
		{
			return head_count_;
		}

		/// @brief Set the number of tracks per head for the disk geometry.
		/// 
		/// @param value New track count. Must be greater than 0.
		/// 
		/// @throws std::invalid_argument if @p value is zero.
		void track_count(size_type value)
		{
			if(value == 0)
			{
				throw std::invalid_argument("Unable to set track count. Track count cannot be zero.");
			}

			track_count_ = value;
		}

		/// @brief Retrieve the configured number of tracks per head.
		/// 
		/// @return The number of tracks. Guaranteed to be greater than 0.
		[[nodiscard]] size_type track_count() const noexcept
		{
			return track_count_;
		}


	private:

		/// @brief The number of heads on the drive or image. Defaults to 1.
		size_type head_count_ = 1;
		/// @brief The number of tracks on the drive or image. Defaults to 35.
		size_type track_count_ = 35;
	};

}
