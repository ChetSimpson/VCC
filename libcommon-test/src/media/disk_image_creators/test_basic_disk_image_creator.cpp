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
#include "gtest/gtest.h"
#include "vcc/media/disk_image_file_creators/basic_disk_image_file_creator.h"
#include "vcc/media/geometry/generic_disk_geometry.h"

#include <filesystem>
#include <chrono>
#include <string>

using namespace vcc::media;
namespace creator_ns = ::vcc::media::disk_image_file_creators;
using geometry_ns = geometry::generic_disk_geometry;

TEST(test_basic_disk_image_file_creator, constructor_zero_sectors_throws)
{
    // Zero sectors per track is not allowed.
    EXPECT_THROW(
        creator_ns::basic_disk_image_file_creator(0u, 256u),
        std::invalid_argument);
}

TEST(test_basic_disk_image_file_creator, constructor_zero_bytes_per_sector_throws)
{
    // Zero bytes per sector is not allowed.
    EXPECT_THROW(
        creator_ns::basic_disk_image_file_creator(18u, 0u),
        std::invalid_argument);
}

TEST(test_basic_disk_image_file_creator, create_empty_path_throws)
{
    // Construct a creator with sensible defaults.
    creator_ns::basic_disk_image_file_creator creator(18u, 256u);

    // Use a generic geometry (head/track values are not used by the basic
    // creator's default track-size calculation, but a valid geometry must be passed).
    geometry_ns geom(2u, 10u, 18u, 256u);

    // An empty path should raise std::invalid_argument per implementation.
    std::filesystem::path empty_path;
    EXPECT_THROW(creator.create(empty_path, geom), std::invalid_argument);
}

TEST(test_basic_disk_image_file_creator, create_creates_file_with_expected_size)
{
    // Prepare geometry and creator such that the basic creator's defaults match
    // the geometry's sector layout. This makes the expected file size easy to compute.
    const auto heads = 2u;
    const auto tracks = 4u;
    const auto sectors_per_track = 16u;
    const auto bytes_per_sector = 128u;

    geometry_ns geom(heads, tracks, sectors_per_track, bytes_per_sector);

    // Use the same defaults as the geometry so calculate_track_size produces
    // the expected per-track size.
    creator_ns::basic_disk_image_file_creator creator(sectors_per_track, bytes_per_sector);

    // Create a unique path in the temporary directory.
    const auto now = std::chrono::steady_clock::now().time_since_epoch().count();
    const std::string filename = "vcc_basic_disk_image_test_" + std::to_string(now) + ".img";
    const auto tmp = std::filesystem::temp_directory_path();
    const auto file_path = tmp / filename;

    // Ensure cleanup even if ASSERT/EXPECT fail.
    std::error_code ec;
    std::filesystem::remove(file_path, ec);

    // Call create and expect success.
    const auto result = creator.create(file_path, geom);
    EXPECT_EQ(result, creator_ns::basic_disk_image_file_creator::error_id_type::none);

    // Validate file exists and size is expected.
    ASSERT_TRUE(std::filesystem::exists(file_path));
    const auto actual_size = std::filesystem::file_size(file_path);
    const std::uintmax_t expected_size =
        static_cast<std::uintmax_t>(heads) *
        static_cast<std::uintmax_t>(tracks) *
        static_cast<std::uintmax_t>(sectors_per_track) *
        static_cast<std::uintmax_t>(bytes_per_sector);

    EXPECT_EQ(actual_size, expected_size);

    // Cleanup
    std::filesystem::remove(file_path, ec);
}
