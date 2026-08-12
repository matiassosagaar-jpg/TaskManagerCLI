#pragma once
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <algorithm>
#include <ranges>
namespace Persistence::FileHandler 
{

void array_to_files(const nlohmann::json& field, const std::filesystem::path& output_path);
void save(const nlohmann::json& full_report);

void read_json_dir(nlohmann::json& target, const std::filesystem::path& dir_path);
nlohmann::json load();
} // namespace Persistence::FileHandler 