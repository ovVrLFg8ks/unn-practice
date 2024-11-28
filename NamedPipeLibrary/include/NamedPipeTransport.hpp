#ifndef NAMED_PIPE_TRANSPORT_HPP
#define NAMED_PIPE_TRANSPORT_HPP

#include <string>
#include <vector>

class NamedPipeTransport {
public:
    NamedPipeTransport(const std::string& read_pipe, const std::string& write_pipe);
    ~NamedPipeTransport();

    void write(const std::vector<uint8_t>& data);
    std::vector<uint8_t> read();

private:
    std::string read_pipe_;
    std::string write_pipe_;
};

#endif 