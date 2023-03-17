#pragma once

#include <wolv/types.hpp>
#include <wolv/io/fs.hpp>

#include <cstdio>
#include <optional>
#include <string>
#include <vector>

#include <sys/stat.h>

#if defined(OS_MACOS)

    #include <unistd.h>
    #include <sys/fcntl.h>

    #define off64_t     off_t
    #define fopen64     fopen
    #define fseeko64    fseek
    #define ftello64    ftell
    #define ftruncate64 ftruncate

#elif defined(OS_LINUX)

    #include <unistd.h>
    #include <fcntl.h>

#endif

namespace wolv::io {

    class File {
    public:
        enum class Mode
        {
            Read,
            Write,
            Create
        };

        explicit File(const std::fs::path &path, Mode mode) noexcept;
        File() noexcept;
        File(const File &) = delete;
        File(File &&other) noexcept;

        ~File();

        File &operator=(File &&other) noexcept;


        [[nodiscard]] bool isValid() const {
            return this->m_file != nullptr;
        }

        void seek(u64 offset);
        void close();

        size_t readBuffer(u8 *buffer, size_t size);
        std::vector<u8> readBytes(size_t numBytes = 0);
        std::string readString(size_t numBytes = 0);
        std::u8string readU8String(size_t numBytes = 0);

        void write(const u8 *buffer, size_t size);
        void write(const std::vector<u8> &bytes);
        void write(const std::string &string);
        void write(const std::u8string &string);

        [[nodiscard]] size_t getSize() const;
        void setSize(u64 size);

        void flush();
        bool remove();

        auto getHandle() { return this->m_file; }
        const std::fs::path &getPath() { return this->m_path; }

        void disableBuffering();

        std::optional<struct stat> getFileInfo();

    private:
        FILE *m_file;
        std::fs::path m_path;
    };

}