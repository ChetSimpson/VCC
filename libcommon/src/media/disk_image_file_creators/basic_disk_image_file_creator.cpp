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
#include "vcc/media/disk_image_file_creators/basic_disk_image_file_creator.h"
#include "vcc/media/exceptions.h"
#include "vcc/utils/scope_guard.h"
#include <fstream>
#include <functional>
#include <filesystem>
#include <system_error>


namespace vcc::media::disk_image_file_creators
{

	basic_disk_image_file_creator::basic_disk_image_file_creator(
		file_size_type default_sectors_per_track,
		file_size_type default_bytes_per_sector)
		:
		default_sectors_per_track_(default_sectors_per_track),
		default_bytes_per_sector_(default_bytes_per_sector)
	{
		if (default_sectors_per_track_ == 0)
		{
			throw std::invalid_argument("Cannot create disk image creator. Default sectors per track is zero.");
		}

		if(default_bytes_per_sector_ == 0)
		{
			throw std::invalid_argument("Cannot create disk image creator. Default bytes per sector is zero.");
		}
	}
	

	basic_disk_image_file_creator::error_id_type basic_disk_image_file_creator::create(
		const path_type& file_path,
		const geometry_type& geometry)
	{
		if (file_path.empty())
		{
			throw std::invalid_argument("Cannot create disk image. Path is empty.");
		}


		layout_type layout;
		if (const auto return_error = calculate_layout(geometry, layout);
			return_error != error_id_type::none)
		{
			return return_error;
		}


		// Create a scoped file deleter to clean up the file if we fail.
		::vcc::utils::scope_guard scoped_file_deleter(
			[&file_path]() noexcept
			{
				std::error_code ec;

				if (std::filesystem::exists(file_path, ec))
				{
					std::filesystem::remove(file_path, ec);
				}
			}
		);


		// Open file with the total size of the disk image file.
		std::ofstream output_stream;

		if (const auto return_error = create_file(output_stream, file_path, layout.file_size);
			return_error != error_id_type::none)
		{
			return return_error;
		}

		if (!output_stream.is_open())
		{
			throw std::runtime_error("Failed to open file after create_file succeeded.");
		}


		// Write image elements such as the header and footer.
		if (const auto return_error = write_elements(output_stream, layout, geometry);
			return_error != error_id_type::none)
		{
			return return_error;
		}

		// The disk image has been created, close it and validate the size.
		output_stream.close();
		if (const auto return_error = validate_image_size(file_path, layout);
			return_error != error_id_type::none)
		{
			return return_error;
		}

		// Cancel the scoped file deleter as we have succeeded.
		scoped_file_deleter.cancel();

		return error_id_type::none;
	}


	basic_disk_image_file_creator::error_id_type basic_disk_image_file_creator::calculate_layout(
		const geometry_type& geometry,
		layout_type& layout) const
	{
		// Calculate local sizes.
		const auto header_size = calculate_header_size();
		const auto footer_size = calculate_footer_size();
		const auto track_size = calculate_track_size(geometry); // bytes per track/side
		const file_size_type image_data_size = geometry.track_count() * geometry.head_count() * track_size;
		const file_size_type file_size = header_size + image_data_size + footer_size;

		// Guard against empty image request (avoid seeking to -1)
		if (file_size == 0)
		{
			throw ::vcc::media::geometry_error("Cannot calculate disk image layout. Calculated file size is zero.");
		}

		layout.header_offset = 0;
		layout.header_size = header_size;
		layout.footer_offset = file_size - footer_size;
		layout.footer_size = footer_size;
		layout.track_size = track_size;
		layout.image_size = image_data_size;
		layout.file_size = file_size;

		return error_id_type::none;
	}

	basic_disk_image_file_creator::file_size_type basic_disk_image_file_creator::calculate_header_size() const noexcept
	{
		return 0;
	}

	basic_disk_image_file_creator::file_size_type basic_disk_image_file_creator::calculate_footer_size() const noexcept
	{
		return 0;
	}

	// Track size represent bytes per single track/side (sectors_per_track * bytes_per_sector)
	basic_disk_image_file_creator::file_size_type basic_disk_image_file_creator::calculate_track_size(const geometry_type& /*geometry*/) const noexcept
	{
		return default_sectors_per_track_ * default_bytes_per_sector_;
	}

	basic_disk_image_file_creator::error_id_type basic_disk_image_file_creator::create_file(
		std::ofstream& output_stream,
		const path_type& file_path,
		file_size_type initial_size)
	{
		if (output_stream.is_open())
		{
			throw std::invalid_argument("Cannot create file. Output stream is already open.");
		}

		if (file_path.empty())
		{
			throw std::invalid_argument("Cannot create file. Path is empty.");
		}

		// Create a scoped file deleter to clean up the file if we fail.
		::vcc::utils::scope_guard scoped_file_deleter(
			[&output_stream, &file_path]() noexcept
			{
				std::error_code ec;

				// TODO-CHET: This is a bit ugly, but we are closing the stream before
				// in the context of a destructor. If the stream's close() function throws,
				// we would be terminating the process. So we swallow any exceptions here.
				try
				{
					if (output_stream.is_open())
					{
						output_stream.close();
					}
				}
				catch (...)
				{
					// Swallow all exceptions in noexcept context
				}

				if (std::filesystem::exists(file_path, ec))
				{
					std::filesystem::remove(file_path, ec);
				}
			}
		);


		std::error_code ec;

		// Ensure parent directory exists
		if (const auto parent = file_path.parent_path();
			!parent.empty() && !std::filesystem::exists(parent, ec))
		{
			if (ec)
			{
				return error_id_type::cannot_create_file;
			}

			if (!std::filesystem::create_directories(parent, ec) || ec)
			{
				return error_id_type::cannot_create_file;
			}
		}

		// Create/truncate the file so it exists.
		output_stream.open(file_path, std::ios::binary | std::ios::out | std::ios::trunc);
		if (!output_stream.is_open())
		{
			return error_id_type::cannot_create_file;
		}

		// If no allocation requested, position at start and return.
		if (initial_size == 0)
		{
			output_stream.seekp(0);
			if (output_stream.fail())
			{
				return error_id_type::cannot_seek;
			}

			// 0 size file created successfully, cancel the scoped deleter.
			scoped_file_deleter.cancel();

			return error_id_type::none;
		}

		// Need to allocate file to a specific size. Close stream first (avoid platform locks).
		output_stream.close();

		// Resize using filesystem API (non-throwing variant).
		const std::uintmax_t target_size = static_cast<std::uintmax_t>(initial_size);
		std::filesystem::resize_file(file_path, target_size, ec);
		if (ec)
		{
			return error_id_type::cannot_resize;
		}

		// Reopen without truncation so we can seek/write header/footer.
		output_stream.open(file_path, std::ios::binary | std::ios::out | std::ios::app);
		if (!output_stream.is_open())
		{
			return error_id_type::cannot_create_file;
		}

		// Ensure we can seek to start.
		output_stream.seekp(0);
		if (output_stream.fail())
		{
			return error_id_type::cannot_seek;
		}

		scoped_file_deleter.cancel();

		return error_id_type::none;
	}

	basic_disk_image_file_creator::error_id_type basic_disk_image_file_creator::write_elements(
		std::ostream& output_stream,
		layout_type& layout,
		const geometry_type& geometry)
	{
		// Basic sanity checks on layout
		if (layout.file_size == 0)
		{
			return error_id_type::file_size_mismatch;
		}

		// Ensure header/footer sizes/offsets fit inside the file
		if (layout.header_offset + layout.header_size > layout.file_size ||
			layout.footer_offset + layout.footer_size > layout.file_size)
		{
			return error_id_type::file_size_mismatch;
		}

		// Ensure header and footer do not overlap
		if (layout.header_size > 0 && layout.footer_size > 0 &&
			(layout.header_offset + layout.header_size) > layout.footer_offset)
		{
			return error_id_type::file_size_mismatch;
		}

		// Write header if present
		if (layout.header_size > 0)
		{
			output_stream.clear();
			output_stream.seekp(layout.header_offset, std::ios::beg);
			if (output_stream.fail())
			{
				return error_id_type::cannot_seek;
			}

			if (const auto return_error = write_image_header(output_stream, geometry);
				return_error != error_id_type::none)
			{
				return return_error;
			}

			// Check stream state after header write
			if (output_stream.fail() || output_stream.bad())
			{
				return error_id_type::cannot_write;
			}
		}

		// Write footer if present
		if (layout.footer_size > 0)
		{
			output_stream.clear();
			output_stream.seekp(layout.footer_offset, std::ios::beg);
			if (output_stream.fail())
			{
				return error_id_type::cannot_seek;
			}

			if (const auto return_error = write_image_footer(output_stream, geometry);
				return_error != error_id_type::none)
			{
				return return_error;
			}

			// Check stream state after footer write
			if (output_stream.fail() || output_stream.bad())
			{
				return error_id_type::cannot_write;
			}
		}

		return error_id_type::none;
	}

	basic_disk_image_file_creator::error_id_type basic_disk_image_file_creator::write_image_header(
		std::ostream& output,
		const geometry_type& geometry)
	{
		return error_id_type::none;
	}

	basic_disk_image_file_creator::error_id_type basic_disk_image_file_creator::write_image_footer(
		std::ostream& output,
		const geometry_type& geometry)
	{
		return error_id_type::none;
	}

	basic_disk_image_file_creator::error_id_type basic_disk_image_file_creator::validate_image_size(
		const path_type& file_path,
		layout_type& layout) const noexcept
	{
		std::error_code ec;

		// Ensure the file exists
		if (!std::filesystem::exists(file_path, ec))
		{
			// If an error_code was set, consider this a validation failure.
			return error_id_type::cannot_validate_size;
		}

		// Ensure it's a regular file (not a directory, special file, etc.)
		const auto st = std::filesystem::status(file_path, ec);
		if (ec)
		{
			return error_id_type::cannot_validate_size;
		}
		if (!std::filesystem::is_regular_file(st))
		{
			return error_id_type::cannot_validate_size;
		}

		// Query the file size (returns std::uintmax_t)
		const std::uintmax_t actual_size_raw = std::filesystem::file_size(file_path, ec);
		if (ec)
		{
			return error_id_type::cannot_validate_size;
		}

		// Compare using the same type returned by filesystem to avoid narrowing issues.
		// layout.file_size is the expected size (file_size_type). Compare by promoting
		// the expected value to std::uintmax_t.
		if (actual_size_raw != static_cast<std::uintmax_t>(layout.file_size))
		{
			return error_id_type::file_size_mismatch;
		}

		return error_id_type::none;
	}

}