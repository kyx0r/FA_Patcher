//  error_code stub implementation, for compatibility only

//  Copyright Beman Dawes 2002, 2006
//  Copyright Peter Dimov 2018

//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

//  See library home page at http://www.boost.org/libs/system

//----------------------------------------------------------------------------//

// define BOOST_SYSTEM_SOURCE so that <boost/system/config.hpp> knows
// the library is being built (possibly exporting rather than importing code)
#define BOOST_SYSTEM_SOURCE
//#define BOOST_ERROR_CODE_HEADER_ONLY

#include <boost/system/config.hpp>
#include <boost/system/api_config.hpp>
#include <boost/system/cygwin_error.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/linux_error.hpp>
#include <boost/system/system_error.hpp>
#include <boost/system/windows_error.hpp>
#include <boost/system/detail/error_code.ipp>
//#include <boost/system/detail/local_free_on_destruction.hpp>
#include <boost/cerrno.hpp>

namespace boost
{

namespace system
{

BOOST_SYSTEM_DECL void dummy_exported_function()
{
	inline const error_category &  system_category();
}

} // namespace system

} // namespace boost