// Author: Konstantin Kutsner
// 

#pragma once

// This is a public header file of the library and it should include any
// private header files (any header file outside of "include/mylib")
#include <mylib/internals.hxx>

class data_base final
{
public:
    // All these special member functions are required to prevent leaking
    // of the private header files into the public inteface of this library
    data_base();
    ~data_base();
    data_base(const data_base&);
    data_base(data_base&&) noexcept;
    data_base& operator =(const data_base&);
    data_base& operator =(data_base&&) noexcept;

    void execute_sql(const std::string &stmt);

private:
    sql_processor_placeholder m_sql;
};
