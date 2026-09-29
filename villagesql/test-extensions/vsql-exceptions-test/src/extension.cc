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

#include <villagesql/vsql.h>

#include <optional>
#include <stdexcept>

using namespace vsql;

//
// Exception in simple INT function
//
void exception_add(IntArg , IntArg , IntResult ) {
  throw std::runtime_error("vsql_exceptions_test::exception_add");
}

//
// Exception in accumulater of function
//
using ExAccState = std::optional<long long>;

void ea_clear(ExAccState &s) { s = std::nullopt; }
void ea_acc(ExAccState &s, IntArg v) {
  if (!v.is_null()) s = s.value_or(0) + v.value();
  if (3 < s.value_or(0)) {
    throw std::bad_optional_access();
  }
}
void ea_result(const ExAccState &s, IntResult out) {
  if (!s.has_value()) { out.set_null(); return; }
  out.set(s.value());
}

//
// Exception in accumulator clear
//
struct ExClearState {
  std::optional<long long> accum;

};

void eclr_clear(ExClearState &s) { throw std::runtime_error("vsql_exceptions_test::ExClearState::eclr_clear"); }
void eclr_acc(ExClearState &s, IntArg v) {
  if (!v.is_null()) s.accum = s.accum.value_or(0) + v.value();
}
void eclr_result(const ExClearState &s, IntResult out) {
  if (!s.accum.has_value()) { out.set_null(); return; }
  out.set(s.accum.value());
}

//
// Exception in accumulater constuctor
//
struct ExConstrState {
  std::optional<long long> accum;

  ExConstrState() { throw std::runtime_error("vsql_exceptions_test::ExConstrState::ExConstrState"); }
};

void ec_clear(ExConstrState &s) { s.accum = std::nullopt; }
void ec_acc(ExConstrState &s, IntArg v) {
  if (!v.is_null()) s.accum = s.accum.value_or(0) + v.value();
}
void ec_result(const ExConstrState &s, IntResult out) {
  if (!s.accum.has_value()) { out.set_null(); return; }
  out.set(s.accum.value());
}

//
// Exception in accumulater destructor
//
struct ExDestructrState {
  std::optional<long long> accum;

  // Highly unlikely someone adds noexcept(false) to a destructor. The default is noexcept(true) which forces
  // C++ to signal and terminates. We do not catch a signal. This code merely tests the unlikely scenario
  // where we receive an exception instead.
  ~ExDestructrState() noexcept(false) { throw std::runtime_error("vsql_exceptions_test::ExDestructrState::~ExDestructrState"); }
};

void ed_clear(ExDestructrState &s) { s.accum = std::nullopt; }
void ed_acc(ExDestructrState &s, IntArg v) {
  if (!v.is_null()) s.accum = s.accum.value_or(0) + v.value();
}
void ed_result(const ExDestructrState &s, IntResult out) {
  if (!s.accum.has_value()) { out.set_null(); return; }
  out.set(s.accum.value());
}


VEF_GENERATE_ENTRY_POINTS(make_extension()
                          .func(make_func<&exception_add>("exception_add").returns(INT).param(INT).param(INT).build())
                          .func(make_aggregate_func<ExAccState, &ea_result>("exception_sum").returns(INT)
                               .param(INT).clear<&ea_clear>().accumulate<&ea_acc>().build()
                               )
                          .func(make_aggregate_func<ExClearState, &eclr_result>("exception_clear").returns(INT)
                               .param(INT).clear<&eclr_clear>().accumulate<&eclr_acc>().build()
                               )
                          .func(make_aggregate_func<ExConstrState, &ec_result>("exception_constr").returns(INT)
                               .param(INT).clear<&ec_clear>().accumulate<&ec_acc>().build()
                               )
                          .func(make_aggregate_func<ExDestructrState, &ed_result>("exception_destructr").returns(INT)
                               .param(INT).clear<&ed_clear>().accumulate<&ed_acc>().build()
                               )
                          )
