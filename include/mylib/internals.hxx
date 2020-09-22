// Author: Konstantin Kutsner
//
#pragma once

#include <mylib/placeholder.hxx>

// This file contains necessary declarations and definitions to decouple private implementation
// details from the public interface definitions. This allows to not leak private header files
// of the library to the public header files. However, it requires to specify the exact size
// of the private implementation depended type. That has a downside effects. It is not enough
// to specify one size that will work, because the size depends on the build configuration
// and platform. So the matrix of different sizes must be provided.

class sql_processor;

// This is an example of explicit placeholder size calculation using nested ternary operator:
using sql_processor_placeholder =
  details::basic_placeholder<sql_processor,
                             details::x86 ? (details::debug ? 44 : 36)
                                          : details::x64 ? (details::debug ? 72 : 56) : 0>;

// The alternarive format is using special helper that selects the size automatically:
// using sql_processor_placeholder =
//  placeholder<sql_processor, details::x86_windows_size{44}, details::x64_windows_size{56}>;