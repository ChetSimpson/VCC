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
#include "vcc/media/disk_image_creator.h"
#include "vcc/media/disk_image_layout.h"


namespace vcc::media::disk_image_creators
{

	/// @brief Basic, reusable disk image file creator implementation.
	///
	/// This class provides a concrete base implementation for creating simple
	/// disk image files on disk using a specified `geometry`. It implements the
	/// public `create` entry point from `::vcc::media::disk_image_creator`
	/// and supplies a set of protected and private virtual hooks that derived
	/// classes can override to change header/footer layout, track sizing, and
	/// how headers/footers are written.
	///
	/// The class enforces that its default sector and sector-size parameters
	/// provided to the constructor are non-zero. The `create` operation returns
	/// an `error_id_type` which indicates success (`error_id_type::none`) or a
	/// specific failure mode.
	class LIBCOMMON_EXPORT basic_disk_image_creator : public ::vcc::media::disk_image_creator
	{
	public:

		/// @brief Type used for file and region sizes (in bytes).
		using file_size_type = std::uintmax_t;
		/// @brief Layout description type returned by layout calculations.
		using layout_type = ::vcc::media::disk_image_layout;


	public:

		/// @brief Construct a basic disk image creator with defaults for track layout.
		///
		/// @param default_sectors_per_track Default number of sectors per track used
		///        when no more specific information is available. Must be non-zero.
		/// @param default_bytes_per_sector Default bytes-per-sector used when no
		///        more specific information is available. Must be non-zero.
		///
		/// @throws std::invalid_argument if either parameter is zero.
		basic_disk_image_creator(
			file_size_type default_sectors_per_track,
			file_size_type default_bytes_per_sector);

		/// @brief Create a disk image file at `file_path` using `geometry`.
		///
		/// Implements the abstract `create` method from the base interface. The
		/// function performs layout calculation, file creation/allocation, writes
		/// optional header/footer elements, closes the file and validates the
		/// final file size. On failure the returned `error_id_type` describes the
		/// failure mode and the partially created file is removed.
		///
		/// @param file_path Path where the image should be created.
		/// @param geometry Geometry that describes the logical disk layout.
		/// @return `error_id_type::none` on success or a specific error id on failure.
		error_id_type create(
			const path_type& file_path,
			const geometry_type& geometry) final;


	protected:
		
		/// @brief Calculate the layout (header/footer offsets, track and file sizes).
		///
		/// Override to customize header/footer sizes or track-size computation.
		///
		/// @param geometry Source geometry used to compute track/image size.
		/// @param layout Output layout populated by this call.
		/// @return `error_id_type::none` on success or an error id on failure.
		virtual error_id_type calculate_layout(
			const geometry_type& geometry,
			layout_type& layout) const;
		/// @brief Calculate the header size in bytes (default 0).
		///
		/// Override to return a non-zero header size when needed.
		virtual file_size_type calculate_header_size() const noexcept;
		/// @brief Calculate the footer size in bytes (default 0).
		///
		/// Override to return a non-zero footer size when needed.
		virtual file_size_type calculate_footer_size() const noexcept;
		/// @brief Calculate bytes-per-track for the given geometry.
		///
		/// Default implementation uses the configured defaults:
		/// `default_sectors_per_track_ * default_bytes_per_sector_`.
		virtual file_size_type calculate_track_size(const geometry_type& geometry) const noexcept;


	private:

		/// @brief Create and (optionally) pre-allocate the target file to `initial_size`.
		///
		/// This helper will create parent directories as needed and attempts to
		/// resize the file when a non-zero `initial_size` is requested. On
		/// failure it returns an appropriate `error_id_type`. The caller handles
		/// cleanup of partially created files via scope guards.
		virtual error_id_type create_file(
			std::ofstream& output,
			const path_type& file_path,
			file_size_type initial_size);

		/// @brief Write header/footer/image elements into an already-created file.
		///
		/// Performs sanity checks on the provided `layout` and writes header and
		/// footer regions by calling `write_image_header` and
		/// `write_image_footer`. Returns an error id on failure.
		virtual error_id_type write_elements(
			std::ostream& output,
			layout_type& layout,
			const geometry_type& geometry);

		/// @brief Write the image header region. Default implementation does nothing.
		///
		/// Override to emit a format-specific header into `output` at the current
		/// stream position. Return `error_id_type::none` on success.
		virtual error_id_type write_image_header(
			std::ostream& output,
			const geometry_type& geometry);

		/// @brief Write the image footer region. Default implementation does nothing.
		///
		/// Override to emit a format-specific footer into `output` at the current
		/// stream position. Return `error_id_type::none` on success.
		virtual error_id_type write_image_footer(
			std::ostream& output,
			const geometry_type& geometry);

		/// @brief Validate that the file on disk matches the expected layout size.
		///
		/// Uses the filesystem APIs to check existence, regular-file status and the
		/// actual file size. This method is `noexcept` and returns an appropriate
		/// `error_id_type` rather than throwing.
		virtual error_id_type validate_image_size(
			const path_type& file_path,
			layout_type& layout) const noexcept;

	private:

		/// @brief Default number of sectors per track used when calculating track size.
		/// Guaranteed to be non-zero by the constructor.
		const file_size_type default_sectors_per_track_;
		/// @brief Default bytes per sector used when calculating track size.
		/// Guaranteed to be non-zero by the constructor.
		const file_size_type default_bytes_per_sector_;
	};

}