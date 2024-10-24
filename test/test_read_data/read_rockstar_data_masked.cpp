/**
 * This file is part of HaloDataManager.
 * Copyright (c) 2024 Douglas Rennehan (douglas.rennehan@gmail.com)
 * 
 * This program is free software: you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License as published by the 
 * Free Software Foundation, either version 3 of the License, or 
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License 
 * along with HaloDataManager. If not, see <https://www.gnu.org/licenses/>.
*/

#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "../../io/DataIO.hpp"

int main(int argc, char* argv[]) {
    DataIO<DataContainer<RockstarData>> data_io("../data/out_163.list");

    std::vector<std::string> column_mask;
    column_mask.push_back("virial_mass");
    column_mask.push_back("id");

    DataContainer<RockstarData> rockstar_data(column_mask);
    data_io.read_data_from_file(rockstar_data);

    size_t mvir_key = rockstar_data.get_internal_key("virial_mass");
    // print the first 10 mvir values
    for (size_t i = 0; i < 10; i++) {
        std::cout << "mvir #" << i << " = ";
        std::cout << rockstar_data.get_data<double>(i, mvir_key);
        std::cout << " Msun" << std::endl;
    }

    size_t id_key = rockstar_data.get_internal_key("id");
    // print the first 10 id values
    for (size_t i = 0; i < 10; i++) {
        std::cout << "id #" << i << " = ";
        std::cout << rockstar_data.get_data<int64_t>(i, id_key);
        std::cout << std::endl;
    }

    return 0;
}
