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
#include <bit>


namespace vcc::media::geometry
{

	/// @brief Represents the geometry of a disk device or image.
	///
	/// This concrete implementation of `disk_geometry` describes common disk
	/// parameters such as the number of sectors per track and the sector size
	/// (in bytes). The values stored here are logical defaults and may vary by
	/// physical drive or image format.
	///
	/// The class enforces basic invariants:
	/// - `sector_count` must be non-zero.
	/// - `sector_size` must be non-zero and a power of two.
	class LIBCOMMON_EXPORT generic_disk_geometry : public disk_geometry
	{
	public:

		/// @brief Default-constructs a `generic_disk_geometry`.
		///
		/// Initializes members to common defaults: `sector_count_ = 18` and
		/// `sector_size_ = 256`. `heads` and `tracks` are initialized by the
		/// base `disk_geometry` default.
		generic_disk_geometry() = default;

		/// @brief Constructs a `generic_disk_geometry` with explicit parameters.
		///
		/// @param heads Number of heads (passed to base `disk_geometry`).
		/// @param tracks Number of tracks (passed to base `disk_geometry`).
		/// @param sector_count Number of sectors per track. Must be non-zero.
		/// @param sector_size Size in bytes of a sector. Must be non-zero and a power of two.
		///
		/// @throws std::invalid_argument if `sector_count` is zero, if `sector_size`
		///         is zero, or if `sector_size` is not a power of two.
		generic_disk_geometry(
			size_type heads,
			size_type tracks,
			size_type sector_count,
			size_type sector_size)
			:
			disk_geometry(heads, tracks),
			sector_count_(sector_count),
			sector_size_(sector_size)
		{
			if (sector_count_ == 0)
			{
				throw std::invalid_argument("Unable to create generic disk geometry. Sector count cannot be zero.");
			}

			if (sector_size_ == 0)
			{
				throw std::invalid_argument("Unable to create generic disk geometry. Sector size cannot be zero.");
			}

			if (!std::has_single_bit(sector_size_))
			{
				throw std::invalid_argument("Unable to create generic disk geometry. Sector size must be a power of two.");
			}
		}

		/// @brief Returns the number of sectors per track.
		///
		/// This value represents the nominal number of sectors per track for the
		/// geometry. Individual tracks or specific formats may vary.
		///
		/// @return The sectors-per-track count.
		[[nodiscard]] size_type sector_count() const noexcept
		{
			return sector_count_;
		}

		/// @brief Sets the number of sectors per track.
		///
		/// @param value New sectors-per-track value. Must be non-zero.
		///
		/// @throws std::invalid_argument if `value` is zero.
		void sector_count(size_type value)
		{
			if (value == 0)
			{
				throw std::invalid_argument("Unable to set sector count. Sector count cannot be zero.");
			}

			sector_count_ = value;
		}
		
		/// @brief Returns the sector size in bytes.
		///
		/// The sector size is a power of two and is expressed in bytes.
		///
		/// @return Sector size in bytes.
		[[nodiscard]] size_type sector_size() const noexcept
		{
			return sector_size_;
		}

		/// @brief Sets the number bytes per sector.
		///
		/// @param value New number of bytes per sector. Must be non-zero.
		///
		/// @throws std::invalid_argument if `value` is zero.
		void sector_size(size_type value)
		{
			if (sector_size_ == 0)
			{
				throw std::invalid_argument("Unable set sector size. Sector size cannot be zero.");
			}

			if (!std::has_single_bit(sector_size_))
			{
				throw std::invalid_argument("Unable set sector size. Sector size must be a power of two.");
			}

			sector_size_ = value;
		}


	private:

		/// @brief The number of sectors per track on the drive or image.
		/// 
		/// Specifies the number of sectors per track on the drive or image. This value is not
		/// absolute and may vary from track to track and from device to device. This member is
		/// initialized with a default value of 18.
		size_type sector_count_ = 18;
		/// @brief The number of bytes per sector. This value is not absolute and may vary from
		/// sector to sector, track to track, and device to device. This member is initialized
		/// with a default value of 256.
		size_type sector_size_ = 256;
	};


}
