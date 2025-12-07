#pragma once
#ifndef MYENGINE_FILESYSTEM_H
#define MYENGINE_FILESYSTEM_H

#include "../Utils/Logger.h"

namespace MyEngine {
    /**
     * @class FileSystem
     * @brief 文件系统
     *
     */
    class FileSystem {
    public:
        /**
         * @enum Filter
         * @brief 文件类型筛选
         */
        enum Filter {
            All,
            FilesOnly,
            DirectoriesOnly
        };

        enum DataSize {
            B,
            KB,
            MB,
            GB,
            TB
        };

        /**
         * @brief 设置当前的目录
         * @param main_directory 设定目录
         */
        static bool setCurrentPath(const std::string& main_directory);
        /**
         * @brief 获取当前的目录
         *
         */
        static const std::string& currentPath();
        /**
         * @brief 指定文件是否存在
         * @param path 指定文件路径
         * @note 若指定相对目录，则会从主目录下遍历！
         */
        static bool isFile(const std::string& path);
        /**
         * @brief 指定一个目录是否存在？
         * @param path 指定目录
         * @note 若指定相对目录，则会从主目录下遍历！
         */
        static bool isDir(const std::string& path);
        /**
         * @brief 创建一个空目录
         * @param path 指定新的空目录
         * @param ignore_error 忽略目录存在的错误
         * @param recursive_create 是否递归创建目录
         * @return 返回是否成功创建一个空目录
         */
        static bool mkDir(const std::string& path, bool ignore_error = false, bool recursive_create = false);
        /**
         * @brief 删除指定的目录
         * @param path             指定目录
         * @param ignore_error     忽略目录存在的错误
         * @param recursive_remove 递归删除此目录下的所有文件及目录
         * @warning 请不要随意使用 recursive_remove 参数，如指定目录下包含重要文件，使用此参数后将一同删除！
         */
        static bool rmDir(const std::string& path, bool ignore_error = false, bool recursive_remove = false);
        /**
         * @brief 创建一个空白文件
         * @param path                   指定文件路径
         * @param auto_create_directory  是否自动创建目录（当目录不存在时递归创建）
         * @param ignore_error           忽略存在的错误
         *
         */
        static bool mkFile(const std::string &path, bool auto_create_directory = false, bool ignore_error = false);
        /**
         * @brief 删除指定的文件
         * @param path          指定文件路径
         * @param ignore_error  忽略文件存在的错误
         */
        static bool rmFile(const std::string& path, bool ignore_error = false);
        /**
         * @brief 写入内容到指定文件
         * @param context       指定写入内容
         * @param path          指定文件路径
         * @param append_mode   是否使用追加写入
         * @param ignore_error  忽略存在的错误
         */
        static bool writeFile(const std::string &context, const std::string &path,
                              bool append_mode, bool ignore_error = false);
        /**
         * @brief 从指定文件中读取内容
         * @param path          指定文件路径
         * @param ignore_error  忽略存在的错误
         * @param ok            用于检查当前是否存在错误
         * @return 返回读取到的文件的所有内容
         */
        static std::string readFile(const std::string &path, bool ignore_error = false, bool *ok = nullptr);

        /**
         * @brief 写入二进制内容到指定文件
         * @param path              指定输出文件路径
         * @param append_mode       是否使用追加写入模式
         * @param how2WriteFile     指定写入文件的方法
         * @param ignore_error      忽略存在的错误
         * @return 返回是否成功写入文件
         */
        static bool writeBinaryFile(const std::string& path, bool append_mode,
                                    const std::function<void(std::ofstream& file)>& how2WriteFile,
                                    bool ignore_error = false);
        /**
         * @brief 写入二进制内容到指定文件中
         * @param binaries      指定存储的二进制内容
         * @param path          指定文件输出路径
         * @param append_mode   指定是否使用追加写入模式
         * @param ignore_error  忽略存在的错误
         * @return 返回是否成功写入文件
         */
        static bool writeBinaryFile(const std::vector<uint8_t>& binaries, const std::string& path,
                                    bool append_mode, bool ignore_error = false);

        /**
         * @brief 从指定文件中读取二进制内容
         * @param path          指定文件路径
         * @param how2ReadFile  指定读取文件的方法
         * @param ignore_error  忽略存在的错误
         * @return 返回是否成功读取二进制内容
         */
        static bool readBinaryFile(const std::string& path,
                                   const std::function<void(std::ifstream& file)>& how2ReadFile,
                                   bool ignore_error = false);
        /**
         * @brief 从指定文件中读取二进制内容
         * @param path          指定文件路径
         * @param ignore_error  忽略存在的错误
         * @param ok            执行是否存在问题
         * @return 返回完整的二进制内容
         */
        static std::vector<uint8_t> readBinaryFile(const std::string& path, bool ignore_error = false, bool* ok = nullptr);

        static size_t getFileSize(const std::string& file_path);

        static float readableSize(const std::string& file_path, DataSize data_size = MB);

        /**
         * @brief 获取绝对路径
         * @param path 指定路径
         * @return 将返回完整的绝对路径
         */
        static std::string getAbsolutePath(const std::string& path);
        /**
         * @brief 获取文件名
         * @param path 指定完整的路径
         * @param cut_file_extension 是否截断文件后缀名
         * @return 返回文件名
         */
        static std::string getShortFileName(const std::string& path, bool cut_file_extension = false);

        /**
         * @brief 获取指定路径下的所有文件或目录
         * @param path 指定路径
         * @param filter 指定筛选类型
         * @param file_extension_filter 筛选文件后缀名列表
         * @param ignore_error 忽略存在的错误
         * @return 返回一个文件路径列表
         * @see listFiles
         */
        static std::vector<std::string> listFilePaths(const std::string& path, Filter filter = All,
                                                      const std::vector<std::string>& file_ext_list = {},
                                                      bool ignore_error = false);

        static std::vector<std::string> listFilesRecursively(const std::string& path, 
                                                            const std::vector<std::string>& file_ext_list = {},
                                                            bool ignore_error = false);

        /**
         * @brief 获取指定路径下的所有文件或目录
         * @param path 指定路径
         * @param filter 指定筛选类型
         * @param file_extension_filter 筛选文件后缀名列表
         * @param ignore_error 忽略存在的错误
         * @return 返回一个文件名列表
         * @see listFilePaths
         */
        static std::vector<std::string> listFiles(const std::string& path, Filter filter = All,
                                                  const std::vector<std::string>& file_ext_list = {},
                                                  bool ignore_error = false);

        FileSystem() = delete;
        FileSystem(FileSystem&) = delete;
        FileSystem& operator=(const FileSystem&) = delete;
        ~FileSystem() = delete;
    private:
        static std::deque<std::string> getPathUntilExist(const std::string& path);
        static std::deque<std::string> getPathUntilNotExist(const std::string &path);
        static std::string _main_path;
    };
}

#endif //MYENGINE_FILESYSTEM_H
