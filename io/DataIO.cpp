#include <chrono>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <variant>
#include "DataIO.hpp"

template <typename Container>
void DataIO<Container>::set_file_name(const std::string &file_name) {
    file_name_ = file_name;
}

template <typename Container>
std::string DataIO<Container>::get_file_name(void) const {
    return file_name_;
}

template <typename Container>
void DataIO<Container>::set_header(const std::vector<std::string> &header) {
    header_ = header;
}

template <typename Container>
std::vector<std::string> DataIO<Container>::get_header(void) const {
    return header_;
}

template <typename Container>
std::vector<std::string> DataIO<Container>::read_header(const std::string &file_name) {
    std::vector<std::string> lines;
    std::string line;
    std::ifstream file(file_name);

    if (file.is_open()) {
        while (getline(file, line, '\n')) {
            // all of the header fields start with a # character, so stop when we don't find one
            if (line.find("#") == std::string::npos) {
                break;
            }

            lines.push_back(line);
        }
    }
    else {
        throw std::runtime_error("Can not open the provided file!\n" + file_name);
    }

    file.close();

    // only set the file path if the read was successful.
    set_file_name(file_name);
    set_header(lines);

    return lines;
}

template <typename Container>
std::vector<std::string> DataIO<Container>::read_header(void) {
    return read_header(file_name_);
}

template <typename Container>
std::vector<double> DataIO<Container>::read_cosmology_from_header(const std::vector<std::string> &header) const {
    std::vector<double> cosmological_parameters;

    for (auto line : header) {
        if (line.find("#Om") != std::string::npos) {
            size_t pos1 = line.find(";");
            size_t pos2 = line.find(";", pos1 + 1);

            cosmological_parameters.push_back(
                std::stof(
                    line.substr(
                        line.find("=") + 2,
                        pos1 - line.find("=") - 2
                    )
                )
            );
            cosmological_parameters.push_back(
                std::stof(
                    line.substr(
                        line.find("=", pos1) + 2,
                        pos2 - line.find("=", pos1) - 2
                    )
                )
            );
            cosmological_parameters.push_back(
                std::stof(
                    line.substr(
                        line.find("=", pos2) + 2
                    )
                )
            );
        }
    }

    if (cosmological_parameters.empty()) {
        throw std::runtime_error("There is no cosmology in the header! Please check the standard format."); 
    }

    return cosmological_parameters;
}

template <typename Container>
std::vector<double> DataIO<Container>::read_cosmology_from_header(void) const {
    return read_cosmology_from_header(header_);
}

template <typename Container>
double DataIO<Container>::read_scale_factor_from_header(const std::vector<std::string> &header) const {
    double scale_factor = -1.;
    for (auto line : header) {
        if (line.find("#a") != std::string::npos) {
            size_t pos = line.find("=");
            scale_factor = std::stof(
                line.substr(pos + 1)
            );
        }
    }

    if (scale_factor < 0) {
        std::cout << "Scale factor not found in file, setting to -1.\n";
    }

    // return -1 if there is no scale factor (e.g. consistent trees)
    return scale_factor;
}

template <typename Container>
double DataIO<Container>::read_scale_factor_from_header(void) const {
    return read_scale_factor_from_header(header_);
}

template <typename Container>
double DataIO<Container>::read_box_size_from_header(const std::vector<std::string> &header) const {
    double box_size = -1.;
    for (auto line : header) {
        if (line.find("#Box") != std::string::npos) {
            size_t pos1 = line.find(" ", line.find(" ") + 1);
            size_t pos2 = line.find(" ", pos1 + 1);
            box_size = std::stof(
                line.substr(
                    pos1 + 1,
                    pos2 - pos1 - 1
                )
            );
        }
    }

    if (box_size < 0) {
        throw std::runtime_error("There is no box size in the header! Please check the standard format.");
    }

    return box_size;
}

template <typename Container>
double DataIO<Container>::read_box_size_from_header(void) const {
    return read_box_size_from_header(header_);
}

template <typename Container>
bool DataIO<Container>::process_line_from_file(const std::string &line, Container &container) const {
    std::string field;
    std::stringstream line_stream(line);

    // sometimes data files have rows with only a single number
    // so ignore them
    std::stringstream single_number_stream(line);
    double test_number;
    if (single_number_stream >> test_number) {
        std::string remaining;
        if (!(single_number_stream >> remaining)) {
            return false;
        }
    }

    auto row = std::make_shared<std::vector<std::variant<double, float, int64_t>>>();

    size_t column_index = 0;
    // field contains a string representation of the actual single data point in the file
    while (getline(line_stream, field, ' ')) {
        if (field.empty()) {
            continue;
        }

        if (container.column_mask(column_index)) {
            if (container.is_column_double(column_index)) {
                row->push_back(std::stod(field));
            }
            else {
                // all non-double columns are int64_t types
                row->push_back((int64_t)std::strtol(field.c_str(), NULL, 10));
            }
        }

        column_index++;
    }

    container.data_.push_back(row);

    return true;
}

template <typename Container>
size_t DataIO<Container>::read_data_from_file(const std::string &file_name, Container &container) const {
    std::ifstream halo_catalog_file(file_name);
    size_t line_indexer = 0;
    size_t N_lines = 0;

    if (halo_catalog_file.is_open()) {
        std::string line;

        auto start_time = std::chrono::high_resolution_clock::now();
        while (getline(halo_catalog_file, line)) {
            if (line.find("#") != std::string::npos) {
                continue;
            }

            if (process_line_from_file(line, container)) {
                N_lines++;
            }
        }
        auto end_time = std::chrono::high_resolution_clock::now();

        std::chrono::duration<float> seconds_interval = end_time - start_time;
        float iterations_per_second = (float)N_lines / seconds_interval.count();
        std::cout << "Duration was " << seconds_interval.count() << " s\n";
        std::cout << "The speed was " << iterations_per_second << " lines per second\n";
    }
    else {
        throw std::runtime_error("Could not open the provided file!\n" + file_name);
    }

    return N_lines;
}

template <typename Container>
size_t DataIO<Container>::read_data_from_file(Container &container) const {
    return read_data_from_file(file_name_, container);
}


