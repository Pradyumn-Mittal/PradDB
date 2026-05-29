#include <vector>
#include <string>

class CatalogManager {
public:
  bool table_exists(const std::string& name);

  void register_table(const std::string& name);

  void remove_table(const std::string& name);

  const std::vector<std::string>& get_all_tables();
};