
#ifndef DATACONTAINER_HPP
#define DATACONTAINER_HPP

#include <stdexcept>
#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <variant>
#include "../main.hpp"

/**
 * The DataContainer class will define and store the actual data from the file internally.
 * It is important to note that this library is not a generic ASCII file reader, and only
 * needs to support specific rockstar versions. Therefore, unfortunately, the column types
 * and locations will be hard-coded here, with user-specify columns for reading.
 *
 * In the Rockstar version I use, there is the following header:
 * ID DescID Mvir Vmax Vrms Rvir Rs Np X Y Z VX VY VZ JX JY JZ Spin rs_klypin Mvir_all 
 * M200b M200c M500c M2500c Xoff Voff spin_bullock b_to_a c_to_a A[x] A[y] A[z] b_to_a(500c) 
 * c_to_a(500c) A[x](500c) A[y](500c) A[z](500c) T/|U| M_pe_Behroozi M_pe_Diemer Type SM Gas 
 * BH_Mass
 *
 * For consistent-trees, I have the following header:
 * Phantom MMP Suspicious? PID UPID Tracked Tracked_Single_MMP Num_MMP_Phantoms Original_ID 
 * Last_mm Tidal_Force Tidal_ID
 */
template <typename DataFileFormat>
class DataContainer {
private:
    // key -> column index
    static std::map<std::string, size_t> real_keys_;
    static std::map<std::string, size_t> int_keys_;

    std::unordered_map<size_t, std::string> keys_int_to_str_;
    std::unordered_map<std::string, size_t> keys_str_to_int_;

    std::vector<bool> data_is_real_mask_;
public:
    std::vector<std::shared_ptr<std::vector<std::variant<double, float, int64_t>>>> data_;

    DataContainer();

    std::string get_key(const size_t &column_index) const;
    size_t get_key(const std::string &column_name) const;
    size_t get_total_keys(void) const;
    bool is_column_real(const size_t column_index) const;

    // C++ doesn't allow specialization of member functions in templated
    // classes unless the entire class is specialized. That can lead to a
    // huge number of specializations so we are just going to use
    // overloading.
    void data_at(int64_t &value, const size_t row, const size_t column) const;
    void data_at(real &value, const size_t row, const size_t column) const;
};

template class DataContainer<RockstarData>;
template class DataContainer<ConsistentTreesData>;

#endif


