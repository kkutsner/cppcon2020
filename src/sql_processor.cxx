// Author: Konstantin Kutsner
//

#include "sql_processor.hxx"

void sql_processor::execute(const std::string& sql)
{
  m_context = sql;
  m_array.emplace_back(sql);
}