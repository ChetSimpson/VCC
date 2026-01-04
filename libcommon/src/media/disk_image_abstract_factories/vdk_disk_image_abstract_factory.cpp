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
#include "vcc/media/disk_image_abstract_factories/vdk_disk_image_abstract_factory.h"
#include "vcc/media/detail/vdk_disk_image_header_layout.h"
#include "vcc/media/disk_images/generic_disk_image.h"
#include <type_traits>
#include <stdexcept>


namespace vcc::media::disk_image_abstract_factories
{
	namespace 
	{
		// Generic helper to read a 16-bit little-endian word from a byte buffer.
		// Performs a bounds check and works with any container that exposes
		// operator[] and size() (e.g. std::vector, std::array).
		template<class BufferType_>
		inline std::uint16_t get_little_endian_word(const BufferType_& buffer, std::size_t offset)
		{
			static_assert(
				std::is_unsigned_v<std::remove_cv_t<std::remove_reference_t<decltype(buffer[0])>>>,
				"Buffer element type must be an unsigned integral type.");

			// Ensure we have at least two bytes available at the requested offset.
			if (offset + 1 >= buffer.size())
			{
				throw std::out_of_range("get_little_endian_word: buffer too small for offset");
			}

			using elem_unsigned_t = std::make_unsigned_t<std::remove_cv_t<std::remove_reference_t<decltype(buffer[0])>>>;

			const auto lo = static_cast<std::uint16_t>(buffer[offset]) & 0xffu;
			const auto hi = (static_cast<std::uint16_t>(buffer[offset + 1]) & 0xffu) << 8u;

			return static_cast<std::uint16_t>(lo | hi);
		}
	}

	std::unique_ptr<vdk_disk_image_abstract_factory::disk_image_type> vdk_disk_image_abstract_factory::create(
		[[maybe_unused]] stream_ptr_type& stream,
		const stream_size_type& stream_size,
		const header_buffer_type& header_buffer,
		bool write_protected,
		error_id_type& error_condition) const
	{
		using created_disk_image_type = ::vcc::media::disk_images::generic_disk_image;
		using header_layout = ::vcc::media::detail::vdk_disk_image_header_layout;
		using field_offsets = header_layout::field_offsets;

		// Delegate validation to check(). If the header buffer is invalid we'll return nullptr.
		if(const auto detection_result = check(header_buffer, stream_size);
			detection_result != error_id_type::none)
		{
			error_condition = detection_result;
			return {};
		}

		// At this point 'check' guarantees the header buffer is large enough for these reads.
		const auto header_size = get_little_endian_word(header_buffer, field_offsets::header_size);
		const auto head_count = header_buffer[field_offsets::number_of_head];
		const auto track_count = header_buffer[field_offsets::number_of_tracks];

		// Construct a generic disk image with validated geometry and header information.
		// Note: stream is moved into the image. Callers should not use the stream after this.
		return std::make_unique<created_disk_image_type>(
			std::move(stream),
			created_disk_image_type::geometry_type{ head_count, track_count, sector_count_, sector_size_ },
			header_size,
			first_valid_sector_id_,
			write_protected);
	}

	vdk_disk_image_abstract_factory::error_id_type vdk_disk_image_abstract_factory::check(
		const header_buffer_type& header_buffer,
		const stream_size_type& stream_size) const
	{
		using header_layout = ::vcc::media::detail::vdk_disk_image_header_layout;
		using field_offsets = header_layout::field_offsets;
		using field_values = header_layout::values;
		using name_and_compression_masks = header_layout::name_and_compression;

		// Quick sanity: header_buffer must contain at least the fixed base header.
		static_assert (
			header_buffer_type().size() >= header_layout::base_size,
			"Header buffer size is too small");

		// Check the signature
		if (header_buffer[field_offsets::signature_msb] != field_values::signature_msb ||
			header_buffer[field_offsets::signature_lsb] != field_values::signature_lsb)
		{
			return error_id_type::invalid_signature;
		}

		// We only support the current version at this time.
		if(header_buffer[field_offsets::version] != field_values::current_version)
		{
			return error_id_type::unsupported_version;
		}

		const auto specified_header_size = get_little_endian_word(
			header_buffer,
			field_offsets::header_size);

		// Validate the specified header size is within acceptable bounds.
		if(specified_header_size < header_layout::base_size
		   || specified_header_size > stream_size)
		{
			return error_id_type::invalid_header_size;
		}

		// Extract the name length from the flags2 field and determine the total length
		// of the header we should expect.
		const auto name_length =
			(header_buffer[field_offsets::name_and_compression] & name_and_compression_masks::name_length_mask)
			>> name_and_compression_masks::name_length_shift;
		const auto expected_header_size = header_layout::base_size + name_length;

		// The header does not specify any additional data beyond the name so the specified
		// header size must match the expected header size.
		if(specified_header_size != expected_header_size)
		{
			return error_id_type::header_size_mismatch;
		}

		// Defensive: sector_size_ must be non-zero but return an error rather than crash.
		static_assert (sector_size_ != 0, "Sector size cannot be 0");

		// Since there is no footer and no additional data should be present after the image data
		// the specified header size must match the expected header size.
		if (const auto possible_header_size = stream_size % sector_size_;
			expected_header_size != possible_header_size)
		{
			return error_id_type::header_size_mismatch;
		}

		return error_id_type::none;
	}

}
