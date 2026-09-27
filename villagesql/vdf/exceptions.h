// Copyright (c) 2026 VillageSQL Contributors
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, see <https://www.gnu.org/licenses/>.

#ifndef VILLAGESQL_SDK_EXCEPTIONS_H
#define VILLAGESQL_SDK_EXCEPTIONS_H

#define VDF_EXCEPTIONS_TRY \
  try {

#define VDF_EXCEPTIONS_CATCH(result__)          \
  } catch(const std::exception& ex) { \
    (result__).type = VEF_RESULT_ERROR;         \
    if (nullptr != (result__).error_msg) {          \
      snprintf((result__).error_msg, VEF_MAX_ERROR_LEN,   \
               "VDF threw an exception (%s)", ex.what()); \
    } \
  } catch(...) { \
    (result__).type = VEF_RESULT_ERROR;         \
    if (nullptr != (result__).error_msg) {            \
      snprintf((result__).error_msg, VEF_MAX_ERROR_LEN, \
               "VDF threw an exception");       \
    } \
  }

#endif  // VILLAGESQL_SDK_EXCEPTIONS_H
