#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "main.hpp"
#include "io/DataContainer.hpp"
#include "io/DataIO.hpp"

int main(int argc, char* argv[]) {
    DataIO<DataContainer<RockstarData>> data_io("../rockstar_data.list");

    std::vector<std::string> column_mask;
    column_mask.push_back("virial_mass");
    column_mask.push_back("id");

    DataContainer<RockstarData> rockstar_data(column_mask);
    data_io.read_data_from_file(rockstar_data);

    // What is the mvir key?
    std::cout << "mvir key is " << rockstar_data.get_internal_key("virial_mass") << std::endl;

    std::cout << "mvir at 0 is " << std::get<real>(rockstar_data.data_.at(0)->at(rockstar_data.get_internal_key("virial_mass"))) << std::endl;

    real mvir;
    size_t mvir_key = rockstar_data.get_internal_key("virial_mass");
    // print the first 10 mvir values
    for (size_t i = 0; i < 10; i++) {
        rockstar_data.data_at(mvir, i, mvir_key);
        std::cout << "mvir #" << i << " = " << mvir << " Msun" << std::endl;
    }

    int64_t id;
    size_t id_key = rockstar_data.get_internal_key("id");
    // print the first 10 id values
    for (size_t i = 0; i < 10; i++) {
        rockstar_data.data_at(id, i, id_key);
        std::cout << "id #" << i << " = " << id << std::endl;
    }

    return 0;
}