// Author: Konstantin Kutsner
//
#pragma once

#include <mylib/placeholder.hxx>

// This file contains necessary declarations and definitions to decouple private implementation
// details from the public interface definitions. This allows to not leak private header files
// of the library to the public header files.

class sql_processor;
using sql_processor_placeholder =
  placeholder<sql_processor, details::x86_windows_size{44}, details::x64_windows_size{72}>;