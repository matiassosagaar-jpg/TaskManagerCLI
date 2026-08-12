#include "file_handler.hpp"

namespace Persistence::FileHandler 
{
const auto project_path = std::filesystem::current_path();
namespace fs = std::filesystem;
using json = nlohmann::json;
void array_to_files(const json& field, const fs::path& output_path) {
    fs::create_directories(output_path);
    for (const auto& item : field) {
        std::string filename =
            std::to_string(item.at("id").get<uint64_t>()) + ".json";
        std::ofstream output_file(output_path / filename);
        
        if (!(output_file << item))
            throw std::runtime_error(
                "Failed to open file: " + (output_path / filename).string()
            );
    }
}

void save(const json& full_report) {

    fs::path temp_tree = project_path / "root.temp";
    fs::remove_all(temp_tree); // cleaning previous trash
    // ---------------------Sections---------------------
    fs::path sections_path = temp_tree / "section_reports";
    array_to_files(full_report.at("section_reports"), sections_path);
    // -----------------------Tasks-----------------------
    fs::path tasks_path = temp_tree / "task_reports";
    array_to_files(full_report.at("task_reports"), tasks_path);
    // --------------------Generators--------------------
    fs::path generators_path = temp_tree / "generators";
    fs::create_directories(generators_path);
    std::ofstream section_id(generators_path / "sections.json");
    if (!section_id.is_open()) {
        throw std::runtime_error(
            "Failed to write file: " + (generators_path / "sections.json").string()
        );
    }
    section_id <<  full_report.at("last_section_id");

    std::ofstream task_id(generators_path / "tasks.json");
    if (!task_id.is_open()) {
        throw std::runtime_error(
            "Failed to open file: " + (generators_path / "tasks.json").string()
        );
    }
    task_id <<  full_report.at("last_task_id");
    
    fs::path root_path = project_path / "root";
    if (fs::exists(root_path)) {
        fs::rename(root_path, project_path / "root.old");
    }

    fs::rename(temp_tree, root_path);

    if (fs::exists(project_path / "root.old")) {
        fs::remove_all(project_path / "root.old");
}
}

void read_json_dir(json& target, const fs::path& dir_path) {
    target = json::array();
    std::vector<fs::path> files;

    for (const auto& entry : fs::directory_iterator(dir_path)) { // vector of file-paths from the directory
        files.push_back(entry.path());
    }

    std::ranges::sort(files, {}, [](const fs::path& path) {
    return std::stoull(path.stem().string());
    });
    for (const auto& path : files) {
        std::ifstream input_file(path);

        if (!input_file) {
            throw std::runtime_error(
                "Failed to open file: " + path.string()
            );
        }
        json report;
        input_file >> report;
        target.push_back(report);
    }
}
json load() {
    json output;
    fs::path root_path = project_path / "root";
    fs::path sections_path = root_path / "section_reports";
    read_json_dir(output["section_reports"], sections_path);

    fs::path tasks_path = root_path / "task_reports";
    read_json_dir(output["task_reports"], tasks_path);

    fs::path generators_path = root_path / "generators";
    fs::path last_section_path = generators_path / "sections.json";
    std::ifstream last_section_json(last_section_path);
    if (!last_section_json) {
        throw std::runtime_error(
            "Failed to open file: " + last_section_path.string()
        );
    }
    last_section_json >> output["last_section_id"];
    
    fs::path last_task_path = generators_path / "tasks.json";
    std::ifstream last_task_json(last_task_path);
    if (!last_task_json) {
        throw std::runtime_error(
            "Failed to open file: " + last_task_path.string()
        );
    }
    last_task_json >> output["last_task_id"];
    
    return output;
}
} // namespace Persistence::FileHandler