// Author: Konstantin Kutsner
//

#pragma once
#include <string>

class sql_processor
{
public:
    void execute(const std::string &sql);

private:
    std::string m_context;
    std::vector<std::string> m_array;
};