#include <vector>
#include <string>
#include <fstream>
#include "../main.hpp"
#include "RockstarData.hpp"

#ifndef ROCKSTAR_HPP
#define ROCKSTAR_HPP

class RockstarIO {
private:
    std::string file_path_;
    std::vector<std::string> header_;
    std::vector<uint32_t> column_bit_mask_;

public:
    RockstarIO(std::string file_path = "") {
        file_path_ = file_path;
    }

    void set_file_path(std::string file_path_from_user);
    std::string get_file_path(void);

    void set_header(std::vector<std::string> header_from_file);
    std::vector<std::string> get_header(void);

    void set_column_bit_mask(std::vector<uint32_t> column_bit_mask);
    std::vector<uint32_t> get_column_bit_mask(void);

    std::vector<std::string> read_header(std::string file_path);
    std::vector<std::string> read_header(void);

    std::vector<real> read_cosmology_from_header(std::vector<std::string> header);
    std::vector<real> read_cosmology_from_header(void);

    real read_scale_factor_from_header(std::vector<std::string> header);
    real read_scale_factor_from_header(void);

    real read_box_size_from_header(std::vector<std::string> header);
    real read_box_size_from_header(void);

    void process_file_line(std::string line, std::vector<uint32_t> column_bit_mask, std::shared_ptr<RockstarData> rockstar_data);
    uint64_t read_data_from_file(std::shared_ptr<RockstarData> rockstar_data, std::string file_path, std::vector<uint32_t> column_bit_mask);
    uint64_t read_data_from_file(std::shared_ptr<RockstarData> rockstar_data, std::vector<uint32_t> column_bit_mask);
    uint64_t read_data_from_file(std::shared_ptr<RockstarData> rockstar_data, std::string file_path);
    uint64_t read_data_from_file(std::shared_ptr<RockstarData> rockstar_data);
};

#endif
