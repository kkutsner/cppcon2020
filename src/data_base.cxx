// Author: Konstantin Kutsner
//

#include <mylib/data_base.hxx>

#include "sql_processor.hxx"

data_base::data_base() = default;
data_base::~data_base() = default;
data_base::data_base(const data_base&) = default;
data_base::data_base(data_base&&) noexcept = default;
data_base& data_base::operator=(const data_base&) = default;
data_base& data_base::operator=(data_base&&) noexcept = default;

void data_base::execute_sql(const std::string& stmt)
{
  m_sql->execute(stmt);
}
