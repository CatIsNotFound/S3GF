#pragma once
#ifndef MYENGINE_UTILS_FILESYSTEM_H
#define MYENGINE_UTILS_FILESYSTEM_H

#include "Logger.h"

namespace MyEngine {
    /**
     * @class FileSystem
     * \if EN
     * @brief File System (File Manager)
     * @details Easy to manage files or directories in the system. (Based on `std::filesystem`)
     * @note This is a static class, there is no need to obtain a global singleton.
     * \endif
     */
    class FileSystem {
    public:
        /**
         * @enum Filter
         * \if EN
         * @brief File types filter, tell the filter how to filter
         * \endif
         */
        enum Filter : uint8_t {
            All,
            FilesOnly,
            DirectoriesOnly
        };

        /**
         * \if EN
         * @brief Data unit, refers to the basic measurement or processing unit of data
         * \endif
         */
        enum DataUnit : uint8_t {
            B,
            KB,
            MB,
            GB,
            TB
        };

        /**
         * \if EN
         * @brief Get the data size of a specified unit after conversion
         * @param size Original data size
         * @param data_size Specified unit for conversion
         * @return Floating-point value, representing the converted data size
         * @details Data conversion: 1GB = 1024MB, 1MB = 1024KB, 1KB = 1024B
         * \endif
         */
        static float translateSize(size_t size, DataUnit data_size);
        /**
         * \if EN
         * @brief Get the data size of a specified unit after conversion
         * @param size Original data size
         * @param data_size Specified unit for conversion
         * @return Integer value, representing the converted data size
         * @details Data conversion: 1GB = 1024MB, 1MB = 1024KB, 1KB = 1024B
         * \endif
         */
        static size_t translateSize(float size, DataUnit data_size);

        /**
         * \if EN
         * @brief Set the current global directory, defining the full path for `.` in the relative path
         * @param main_directory Set the global directory
         * @return Returns a boolean value indicating whether the operation was successful
         * @see currentPath
         * \endif
         */
        static bool setCurrentPath(const std::string& main_directory);
        /**
         * \if EN
         * @brief Get the current global directory.
         * @return Return the full path of `.` in the relative path
         * @see setCurrentPath
         * \endif
         */
        static const std::string& currentPath();

        /**
         * \if EN
         * @brief Get the directory of the current file through the file path
         * @param file_path Specify a file path
         * @return Returns the full path of the currently specified file
         * @see getFileSize
         * @see getAbsolutePath
         * @see getShortFileName
         * \endif
         */
        static std::string getDirectoryFromFile(const std::string& file_path);
        /**
         * \if EN
         * @brief Check if the specified file exists
         * @param path Path of the specified file
         * @note If a relative directory is specified, it will traverse from the main directory!
         * @return Returns a boolean value indicating whether the specified path is a file
         * @see isDir
         * \endif
         */
        static bool isFile(const std::string& path);
        /**
         * \if EN
         * @brief Check whether a specified directory exists.
         * @param path The specified directory
         * @note If a relative directory is specified, it will be traversed from the main directory!
         * @return Returns a boolean value indicating whether the specified path is a directory.
         * @see isFile
         * \endif
         */
        static bool isDir(const std::string& path);
        /**
         * \if EN
         * @brief Create an empty directory
         * @param path Specify the new empty directory
         * @param ignore_error Ignore errors if the directory already exists
         * @param recursive_create Whether to create directories recursively
         * @return Returns a boolean value indicating whether the operation was successful
         * @see rmDir
         * @see isDir
         * \endif
         */
        static bool mkDir(const std::string& path, bool ignore_error = false, bool recursive_create = false);
        /**
         * \if EN
         * @brief Delete the specified directory
         * @param path The specified directory
         * @param ignore_error Ignore errors if the directory exists
         * @param recursive_remove Recursively delete all files and directories under this directory
         * @warning Do not use the `recursive_remove` parameter carelessly. If the specified directory contains important files, they will also be deleted when using this parameter!
         * @return Returns a boolean value indicating whether the operation was successful
         * @see mkDir
         * @see isDir
         * \endif
         */
        static bool rmDir(const std::string& path, bool ignore_error = false, bool recursive_remove = false);
        /**
         * \if EN
         * @brief Create an empty file
         * @param path The specified file path
         * @param auto_create_directory Whether to automatically create directories (recursively if they do not exist)
         * @param ignore_error Ignore existing errors
         * @return Returns a bool value indicating whether the operation was successful
         * @see rmFile
         * @see isFile
         * \endif
         */
        static bool mkFile(const std::string &path, bool auto_create_directory = false, bool ignore_error = false);
        /**
         * \if EN
         * @brief Delete the specified file
         * @param path The path of the specified file
         * @param ignore_error Ignore errors if the file exists
         * @return Returns a bool value indicating whether the operation was successful
         * @see mkFile
         * @see isFile
         * \endif
         */
        static bool rmFile(const std::string& path, bool ignore_error = false);
        /**
         * \if EN
         * @brief Write content to a specified file
         * @param context The content to write
         * @param path The path to the specified file
         * @param append_mode Whether to use append mode
         * @param ignore_error Ignore any existing errors
         * @return Returns a bool value indicating whether the operation was successful
         * @see writeBinaryFile
         * @see readFile
         * \endif
         */
        static bool writeFile(const std::string_view &context, const std::string &path,
                              bool append_mode = false, bool ignore_error = false);

        static bool writeFile(const std::string &path, const std::function<void(std::ofstream&)>& how2WriteFile,
                               bool append_mode, bool ignore_error);

        /**
         * \if EN
         * @brief Reads content from the specified file
         * @param path The path of the specified file
         * @param ignore_error Ignore any existing errors
         * @param ok Used to check whether there is currently an error
         * @return Returns all the contents read from the file
         * @see writeBinaryFile
         * @see writeFile
         * \endif
         */
        static std::string readFile(const std::string &path, bool ignore_error = false, bool *ok = nullptr);

        static bool readFile(const std::string &path, const std::function<void(std::ifstream&)>& how2ReadFile,
                             bool ignore_error = false);

        /**
         * \if EN
         * @brief Write binary content to a specified file
         * @param path The specified output file path
         * @param append_mode Whether to use append mode for writing
         * @param how2WriteFile Specifies the method for writing the file
         * @param ignore_error Ignore any existing errors
         * @return Returns whether the file was successfully written
         * @see readBinaryFile
         * @see writeFile
         * \endif
         */
        static bool writeBinaryFile(const std::string& path, bool append_mode,
                                    const std::function<void(std::ofstream& file)>& how2WriteFile,
                                    bool ignore_error = false);
        /**
         * \if EN
         * @brief Write binary content to a specified file
         * @param binaries The binary content to be stored
         * @param path The output path of the file
         * @param append_mode Specifies whether to use append mode
         * @param ignore_error Ignore existing errors
         * @return Returns whether the file was successfully written
         * @see readBinaryFile
         * @see writeFile
         * \endif
         */
        static bool writeBinaryFile(const std::string_view& binaries, const std::string& path,
                                    bool append_mode, bool ignore_error = false);

        /**
         * \if EN
         * @brief Read binary content from the specified file
         * @param path The specified file path
         * @param how2ReadFile The method to read the file
         * @param ignore_error Ignore existing errors
         * @return Returns whether reading the binary content was successful
         * @see writeBinaryFile
         * \endif
         */
        static bool readBinaryFile(const std::string& path,
                                   const std::function<void(std::ifstream& file)>& how2ReadFile,
                                   bool ignore_error = false);
        /**
         * \if EN
         * @brief Read binary content from the specified file
         * @param path The specified file path
         * @param ignore_error Ignore existing errors
         * @param ok Indicates whether there were any issues during execution
         * @return Returns the complete binary content
         * @see writeBinaryFile
         * \endif
         */
        static std::vector<uint8_t> readBinaryFile(const std::string& path, bool ignore_error = false, bool* ok = nullptr);
        /**
         * \if EN
         * @brief Get the size of the specified file
         * @param file_path Path of the specified file
         * @return Get the size of the file in bytes
         * @see readableSize
         * \endif
         */
        static size_t getFileSize(const std::string& file_path);
        /**
         * \if EN
         * @brief Get the readable data size of a specified file
         * @param file_path The path of the specified file
         * @param data_size The specified data unit
         * @return Returns a floating-point value representing the readable file size
         * @see getFileSize
         * \endif
         */
        static float readableSize(const std::string& file_path, DataUnit data_size = MB);

        /**
         * \if EN
         * @brief Get the absolute path
         * @param path Specified path
         * @return Returns the complete absolute path
         * @see getFileSize
         * @see getShortFileName
         * @see getDirectoryFromFile
         * \endif
         */
        static std::string getAbsolutePath(const std::string& path);
        /**
         * \if EN
         * @brief Get the file name
         * @param path The specified full path
         * @param cut_file_extension Whether to remove the file extension
         * @return Returns the file name
         * @see getFileSize
         * @see getAbsolutePath
         * @see getDirectoryFromFile
         * \endif
         */
        static std::string getShortFileName(const std::string& path, bool cut_file_extension = false);

        /**
         * \if EN
         * @brief Get all files or directories under the specified path
         * @param path The specified path
         * @param filter The specified filter type
         * @param file_extension_filter List of file extensions to filter
         * @param ignore_error Ignore any existing errors
         * @return Returns a list of file paths
         * @see listFiles
         * @see listFilesRecursively
         * \endif
         */
        static std::vector<std::string> listFilePaths(const std::string& path, Filter filter = All,
                                                      const std::vector<std::string>& file_ext_list = {},
                                                      bool ignore_error = false);

        /**
         * \if EN
         * @brief Fully recursive listing of all files in a specified directory
         * @param path The specified path
         * @param file_ext_list File filter, specifying which file extensions to filter
         * @param ignore_error Ignore any existing errors
         * @return Returns a list of file paths
         * @see listFiles
         * @see listFilePaths
         * \endif
         */
        static std::vector<std::string> listFilesRecursively(const std::string& path, 
                                                            const std::vector<std::string>& file_ext_list = {},
                                                            bool ignore_error = false);

        /**
         * \if EN
         * @brief Get all files or directories under the specified path
         * @param path The specified path
         * @param filter The specified filter type
         * @param file_extension_filter List of file extensions to filter
         * @param ignore_error Ignore existing errors
         * @return Returns a list of file names
         * @see listFilePaths
         * @see listFilesRecursively
         * \endif
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

#endif //MYENGINE_UTILS_FILESYSTEM_H
