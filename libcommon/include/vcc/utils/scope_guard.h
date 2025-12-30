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
#include <type_traits>


namespace vcc::utils
{

	/// @brief A simple RAII scope guard that invokes a callable on destruction.
	/// 
	/// @tparam FunctionType_ Type of the callable to be invoked when the guard
	///                      goes out of scope. The callable must be invocable
	///                      with no arguments and must not throw (required by
	///                      the template constraint).
	/// 
	/// The guard stores a callable and invokes it in the destructor unless the
	/// guard is explicitly cancelled by calling `cancel()`. Copying and moving
	/// are disabled to avoid accidental multiple invocations of the callable.
	/// 
	/// Example:
	/// @code
	/// {
	///     auto guard = vcc::utils::scope_guard([&] { cleanup_resource(); });
	///     // ... do work
	///     if (some_condition) {
	///         guard.cancel(); // prevent cleanup_resource() from being called
	///     }
	/// } // if not cancelled, cleanup_resource() is called here
	/// @endcode
	template <class FunctionType_>
	requires std::is_nothrow_invocable_v<FunctionType_>
	class scope_guard
	{
	public:

		/// @brief Construct the scope guard with a callable.
		/// 
		/// @param f Callable to invoke when this guard is destroyed (unless
		/// cancelled). The callable is stored by value.
		/// 
		/// @note The template constraint requires the callable to be invocable without
		/// arguments and nothrow when invoked. However, the construction/assignment of
		/// the callable itself may throw if the callable's move/copy constructor throws.
		explicit scope_guard(FunctionType_&& f)
			: fn_(std::forward<FunctionType_>(f))
		{
		}

		/// @brief Copy construction is disabled.
		scope_guard(const scope_guard&) = delete;
		/// @brief Move construction is disabled.
		scope_guard(scope_guard&& other) = delete;

		/// @brief Copy assignment is disabled.
		scope_guard& operator=(const scope_guard&) = delete;
		/// @brief Move assignment is disabled.
		scope_guard& operator=(scope_guard&&) = delete;

		/// @brief Cancel the guard so the callable will not be invoked on destruction.
		/// 
		/// This is noexcept and simply marks the guard as inactive.
		void cancel() noexcept { active_ = false; }

		/// @brief Destructor invokes the stored callable if the guard is active.
		/// 
		/// The destructor is noexcept. The callable is only invoked when `active_` is
		/// true. Because the template requires the callable to be nothrow-invocable,
		/// invoking it in the destructor should not throw.
		~scope_guard() noexcept
		{
			if (active_)
			{
				fn_();
			}
		}


	private:

		/// @brief The stored callable that will be invoked on destruction.
		const FunctionType_ fn_;
		/// @brief Whether the guard is active (true) or has been cancelled (false).
		bool active_ = true;
	};

}
