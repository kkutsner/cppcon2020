// Author: Konstantin Kutsner

#include <mylib/data_base.hxx>

int main()
{
    data_base db;
    db.execute_sql("stmt 1");
    
    data_base db2{db};
    db2.execute_sql("stmt 2");

    data_base db3{std::move(db2)};
    db3.execute_sql("stmt 3");
    
    return 0;
}