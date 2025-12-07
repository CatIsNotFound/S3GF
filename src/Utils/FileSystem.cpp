
#include "FileSystem.h"

namespace MyEngine {
    std::string FileSystem::_main_path = std::filesystem::absolute(".").string();
    bool FileSystem::setCurrentPath(const std::string &main_directory) {
        if (std::filesystem::is_directory(main_directory)) {
            _main_path = std::filesystem::absolute(main_directory).string();
            return true;
        }
        Logger::log(std::format("FileSystem: Path '{}' is not found!", main_directory), Logger::ERROR);
        return false;
    }

    const std::string& FileSystem::currentPath() {
        return _main_path;
    }

    bool FileSystem::isFile(const std::string &path) {
        if (path.front() != '.') {
            return std::filesystem::is_regular_file(path);
        } else {
            auto real_path = std::format("{}{}", _main_path, path.substr(path.find_first_of('/')));
            return std::filesystem::is_regular_file(real_path);
        }
    }

    bool FileSystem::isDir(const std::string &path) {
        if (path.front() != '.') {
            return std::filesystem::is_directory(path);
        } else {
            auto real_path = std::format("{}{}", _main_path, path.substr(path.find_first_of('/')));
            return std::filesystem::is_directory(real_path);
        }
    }

    bool FileSystem::mkDir(const std::string &path, bool ignore_error, bool recursive_create) {
        std::filesystem::path temp = getAbsolutePath(path);
        if (std::filesystem::is_directory(temp)) {
            if (ignore_error) {
                Logger::log(std::format("FileSystem: Directory '{}' is exist!", temp.string()),
                            Logger::ERROR);
            }
            return false;
        } else {
            if (recursive_create) {
                auto mk_path = getPathUntilExist(temp.string());
                for (auto& m_path : mk_path) {
                    std::filesystem::create_directory(m_path);
                }
            } else {
                if (!std::filesystem::is_directory(temp)) {
                    try {
                        std::filesystem::create_directory(temp);
                    } catch (const std::exception& e) {
                        if (ignore_error) return false;
                        Logger::log(std::format("FileSystem: Directory '{}' can not be created!\n"
                                     "Exception: {}", temp.string(), e.what()));
                        return false;
                    }
                }
                else {
                    if (ignore_error) {
                        Logger::log(std::format("FileSystem: Directory '{}' is exist!", path));
                    }
                    return false;
                }
            }
        }
        return true;
    }

    bool FileSystem::rmDir(const std::string &path, bool ignore_error, bool recursive_remove) {
        std::filesystem::path temp = getAbsolutePath(path);
        if (!std::filesystem::is_directory(temp)) {
            if (!ignore_error) {
                Logger::log(std::format("FileSystem: Directory '{}' is not exist!", temp.string()));
            }
            return false;
        } else {
            if (recursive_remove) {
                auto rm_paths = getPathUntilNotExist(temp.string());
                if (rm_paths.empty()) {
                    if (ignore_error) {
                        Logger::log(std::format("FileSystem: Directory '{}' can not be removed!", temp.string()),
                                    Logger::ERROR);
                    }
                    return false;
                }
                for (int64_t idx = (int64_t)(rm_paths.size()) - 1; idx >= 0; idx--) {
                    Logger::log(std::format("Delete: {}", rm_paths[idx]), Logger::ERROR);
                    std::filesystem::remove(rm_paths[idx]);
                }
            } else {
                if (std::filesystem::is_directory(temp)) {
                    try {
                        std::filesystem::remove(temp);
                    } catch (const std::exception &e) {
                        if (!ignore_error)
                            Logger::log(std::format("FileSystem: Directory '{}' can not be removed!\n"
                                         "Exception: {}", temp.string(), e.what()), Logger::ERROR);
                        return false;
                    }
                } else {
                    if (!ignore_error) { 
                        Logger::log(std::format("FileSystem: Directory '{}' is not exist!", temp.string()),
                                    Logger::ERROR);
                    }
                    return false;
                }
            }
        }
        return true;
    }

    bool FileSystem::mkFile(const std::string &path, bool auto_create_directory, bool ignore_error) {
        std::filesystem::path temp = getAbsolutePath(path);
        if (auto_create_directory) {
            auto dir = temp.string().substr(0, temp.string().find_last_of('/'));
            if (!std::filesystem::is_directory(dir)) {
                mkDir(dir, ignore_error, true);
            }
        }
        std::ofstream file(temp.string());
        if (!file.is_open()) {
            if (!ignore_error) Logger::log(std::format("FileSystem: Can't create file '{}'!", temp.string()),
                                           Logger::ERROR);
        }
        file << "";
        file.close();
        return true;
    }

    bool FileSystem::rmFile(const std::string &path, bool ignore_error) {
        std::filesystem::path temp = getAbsolutePath(path);
        if (std::filesystem::is_regular_file(temp)) {
            std::filesystem::remove(temp);
            return true;
        }
        if (!ignore_error) Logger::log(std::format("FileSystem: Can't remove the file '{}'", temp.string()),
                                       Logger::ERROR);
        return false;
    }

    bool FileSystem::writeFile(const std::string &context, const std::string &path, bool append_mode, bool ignore_error) {
        std::filesystem::path temp = getAbsolutePath(path);
        std::ofstream file(temp.string(),((append_mode ? (std::ios::in | std::ios::app) : std::ios::in)));
        if (!file.is_open()) {
            if (!ignore_error) Logger::log(std::format("FileSystem: Can't create file '{}'!", temp.string()),
                                           Logger::ERROR);
            return false;
        }
        file << context;
        file.close();
        return true;
    }

    std::string FileSystem::readFile(const std::string &path, bool ignore_error, bool *ok) {
        std::filesystem::path temp = getAbsolutePath(path);
        std::ifstream file(temp.string(), std::ios::in);
        if (!file.is_open()) {
            if (!ignore_error) Logger::log(std::format("FileSystem: File '{}' is not found!", temp.string()),
                                           Logger::ERROR);
            if (ok) *ok = false;
            return "";
        }
        std::string output;
        size_t line = 0;
        try {
            char buf[1024] = {'\0'};
            while (true) {
                line += 1;
                file.getline(buf, 1024);
                output += buf;
                if (!file.eof()) output += '\n'; else break;
            }
        } catch (const std::exception &exception) {
            Logger::log(std::format("FileSystem: Read file '{}' failed at line {}!",
                                    temp.string(), line), Logger::ERROR);
        }
        file.close();
        if (ok) *ok = true;
        return output;
    }

    bool FileSystem::writeBinaryFile(const std::string &path, bool append_mode,
                                     const std::function<void(std::ofstream &)> &how2WriteFile, bool ignore_error) {
        std::filesystem::path temp = getAbsolutePath(path);
        std::ofstream file(temp.string(),((append_mode ? (std::ios::in | std::ios::app | std::ios::binary) :
                                           std::ios::in | std::ios::binary)));
        if (!file.is_open()) {
            if (!ignore_error) Logger::log(std::format("FileSystem: Can't create file '{}'!",
                                                       temp.string()), Logger::ERROR);
            return false;
        }
        if (how2WriteFile) how2WriteFile(file);
        file.close();
        return true;
    }

    bool FileSystem::writeBinaryFile(const std::vector<uint8_t> &binaries, const std::string &path, bool append_mode,
                                     bool ignore_error) {
        std::filesystem::path temp = getAbsolutePath(path);
        std::ofstream file(temp.string(),((append_mode ? (std::ios::in | std::ios::app | std::ios::binary) :
                                           std::ios::in | std::ios::binary)));
        if (!file.is_open()) {
            if (!ignore_error) Logger::log(std::format("FileSystem: Can't create file '{}'!",
                                                       temp.string()), Logger::ERROR);
            return false;
        }
        for (auto& bin : binaries) {
            file.write(reinterpret_cast<char*>(bin), sizeof(uint8_t));
        }
        file.close();
        return true;
    }

    bool FileSystem::readBinaryFile(const std::string &path, const std::function<void(std::ifstream &)> &how2ReadFile,
                                    bool ignore_error) {
        std::filesystem::path temp = getAbsolutePath(path);
        std::ifstream file(temp.string(), std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            if (!ignore_error) Logger::log(std::format("FileSystem: File '{}' is not found!",
                                                       temp.string()), Logger::ERROR);
            return false;
        }
        if (how2ReadFile) how2ReadFile(file);
        file.close();
        return true;
    }

    std::vector<uint8_t> FileSystem::readBinaryFile(const std::string &path, bool ignore_error, bool *ok) {
        std::filesystem::path temp = getAbsolutePath(path);
        std::ifstream file(temp.string(), std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            if (!ignore_error) Logger::log(std::format("FileSystem: File '{}' is not found!",
                                                       temp.string()), Logger::ERROR);
            if (ok) *ok = false;
            return {};
        }
        std::vector<uint8_t> ret(1024);
        char temp_buf;
        while (!file.eof()) {
            file.get(temp_buf);
            ret.emplace_back(static_cast<uint8_t>(temp_buf));
        }
        file.close();
        if (ok) *ok = true;
        return ret;
    }

    size_t FileSystem::getFileSize(const std::string &file_path) {
        if (std::filesystem::exists(file_path)) {
            return std::filesystem::file_size(file_path);
        } else {
            Logger::log(std::format("FileSystem: Can't open file '{}'!", file_path), Logger::WARN);
            return 0;
        }
    }

    float FileSystem::readableSize(const std::string &file_path, MyEngine::FileSystem::DataSize data_size) {
        auto size = getFileSize(file_path);
        switch (data_size) {
            case KB:
                return static_cast<float>(size) / 1024.0f;
            case MB:
                return static_cast<float>(size) / 1024.0f / 1024.f;
            case GB:
                return static_cast<float>(size) / 1024.0f / 1024.f / 1024.f;
            case TB:
                return static_cast<float>(size) / 1024.0f / 1024.f / 1024.f / 1024.f;
            default:
                return static_cast<float>(size);
        }
    }

    std::string FileSystem::getAbsolutePath(const std::string &path) {
        if (path[1] == ':' || path[0] == '~' || path[0] == '/') return path;
        if (path.front() == '.') {
            auto pos = path.find_first_of('/');
            if (pos != std::string::npos)
                return std::format("{}{}", _main_path, path.substr(pos));
            else
                return std::format("{}", _main_path);
        }
        else return std::format("{}/{}", _main_path, path);
    }

    std::string FileSystem::getShortFileName(const std::string &path, bool cut_file_extension) {
        auto pos = path.find_last_of("/\\");
        if (pos == std::string::npos) return path;
        if (!cut_file_extension) return path.substr(pos + 1);
        auto file_name = path.substr(pos + 1);
        pos = file_name.find_last_of('.');
        /// To avoid the hidden files (e.g: .bashrc) in Linux!
        if (pos == std::string::npos || pos == 0) return file_name;
        return file_name.substr(0, pos);
    }

    std::deque<std::string> FileSystem::getPathUntilExist(const std::string &path) {
        std::deque<std::string> paths;
        std::string temp_path = path;
        do {
            paths.push_front(temp_path);
            temp_path = temp_path.substr(0, temp_path.find_last_of('/'));
        } while (!std::filesystem::is_directory(temp_path));
        return paths;
    }

    std::deque<std::string> FileSystem::getPathUntilNotExist(const std::string &path) {
        std::deque<std::string> paths;
        const std::string& temp_path = path;
        if (std::filesystem::is_directory(temp_path)) {
            paths.push_back(temp_path);
            auto files = std::filesystem::directory_iterator(temp_path);
            for (auto& f : files) {
                if (f.is_directory()) {
                    auto new_paths = getPathUntilNotExist(f.path().string());
                    for (auto& sub_path : new_paths) {
                        paths.push_back(sub_path);
                    }
                } else {
                    paths.push_back(f.path().string());
                }
            }
        }
        return paths;
    }

    std::vector<std::string> FileSystem::listFilePaths(const std::string &path, MyEngine::FileSystem::Filter filter,
                                                       const std::vector<std::string> &file_ext_list,
                                                       bool ignore_error) {
        auto real_path = getAbsolutePath(path);
        std::vector<std::string> out;
        if (!std::filesystem::is_directory(real_path)) {
            if (!ignore_error) Logger::log(std::format("FileSystem: Path '{}' is not found! ",
                                                       real_path), Logger::ERROR);
            return {};
        }
        for (const auto& file : std::filesystem::directory_iterator(real_path)) {
            try {
                auto file_path = file.path().string();
                if (filter == DirectoriesOnly) {
                    if (!std::filesystem::is_directory(file_path)) continue;
                    else {
                        out.emplace_back(file_path);
                        continue;
                    }
                } else if (filter == FilesOnly) {
                    if (std::filesystem::is_directory(file_path)) continue;
                }
                if (file_ext_list.empty()) out.emplace_back(file_path);
                auto filename = getShortFileName(file_path);
                std::string ext_name;
                auto pos = filename.find_last_of('.');
                if (pos != std::string::npos && pos > 0) ext_name = filename.substr(pos);
                for (auto &ext: file_ext_list) {
                    if (ext_name == ext) {
                        out.emplace_back(file_path);
                        break;
                    }
                }
            } catch (const std::filesystem::filesystem_error& e) {
                if (!ignore_error) {
                    Logger::log(std::format("FileSystem: Access file '{}' error! Exception: {}",
                                            real_path, e.what()), Logger::ERROR);
                }
            }
        }
        return out;
    }

    std::vector<std::string> FileSystem::listFilesRecursively(const std::string &path,
                                                            const std::vector<std::string> &file_ext_list,
                                                            bool ignore_error) {
        auto real_path = getAbsolutePath(path);
        std::vector<std::string> out;
        if (!std::filesystem::is_directory(real_path)) {
            if (!ignore_error) Logger::log(std::format("FileSystem: Path '{}' is not found! ",
                                                       real_path), Logger::ERROR);
            return {};
        }
        for (const auto& file : std::filesystem::recursive_directory_iterator(real_path)) {
            try {
                auto file_path = file.path().string();
                if (std::filesystem::is_directory(file_path)) {
                    auto sub_res = listFilesRecursively(file_path, file_ext_list, ignore_error);
                    out.insert(out.end(), sub_res.begin(), sub_res.end());
                    continue;
                }
                if (file_ext_list.empty()) {
                    out.emplace_back(file_path);
                    continue;
                }
                auto filename = getShortFileName(file_path);
                std::string ext_name;
                auto pos = filename.find_last_of('.');
                if (pos != std::string::npos && pos > 0) ext_name = filename.substr(pos);
                for (auto &ext: file_ext_list) {
                    if (ext_name == ext) {
                        out.emplace_back(file_path);
                        break;
                    }
                }
            } catch (const std::filesystem::filesystem_error& e) {
                if (!ignore_error) {
                    Logger::log(std::format("FileSystem: Access file '{}' error! Exception: {}",
                                            real_path, e.what()), Logger::ERROR);
                }
            }
        }
        return out;
    }

    std::vector<std::string> FileSystem::listFiles(const std::string &path, FileSystem::Filter filter,
                                                   const std::vector<std::string>& file_ext_list, bool ignore_error) {
        auto real_path = getAbsolutePath(path);
        std::vector<std::string> out;
        if (!std::filesystem::is_directory(real_path)) {
            if (!ignore_error) Logger::log(std::format("FileSystem: Path '{}' is not found! ",
                                                       real_path), Logger::ERROR);
            return {};
        }
        for (const auto& file : std::filesystem::directory_iterator(real_path)) {
            try {
                auto file_path = file.path().string();
                if (filter == DirectoriesOnly) {
                    if (!std::filesystem::is_directory(file_path)) continue;
                    else {
                        out.emplace_back(getShortFileName(file_path));
                        continue;
                    }
                } else if (filter == FilesOnly) {
                    if (std::filesystem::is_directory(file_path)) continue;
                }
                auto filename = getShortFileName(file_path);
                if (file_ext_list.empty()) out.emplace_back(filename);
                std::string ext_name;
                auto pos = filename.find_last_of('.');
                if (pos != std::string::npos && pos > 0) ext_name = filename.substr(pos + 1);
                for (auto &ext: file_ext_list) {
                    if (ext_name == ext) {
                        out.emplace_back(filename);
                        break;
                    }
                }
            } catch (const std::filesystem::filesystem_error& e) {
                if (!ignore_error) {
                    Logger::log(std::format("FileSystem: Access file '{}' error! Exception: {}",
                                            real_path, e.what()), Logger::ERROR);
                }
            }
        }
        return out;
    }
}

