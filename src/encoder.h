#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>

// Input: a list which contains that line of assembly code (i.e. "add", "$t1", "$t2", "$t3")
// Output: A machine instruction
uint32_t encode(const std::vector<std::string>& tokens, const std::unordered_map<std::string, int>& labelTable, int pc);
uint32_t encode(const std::string& str, const std::unordered_map<std::string, int>& labelTable, int pc);

uint32_t _split_by_type(const std::vector<std::string>& tokens, const std::unordered_map<std::string, int>& labelTable, int type, int pc);
uint32_t _encode_R(const std::vector<std::string>& tokens);
uint32_t _encode_I(const std::vector<std::string>& tokens, const std::unordered_map<std::string, int>& labelTable, int pc);
uint32_t _encode_J(const std::vector<std::string>& tokens, const std::unordered_map<std::string, int>& labelTable);

// Input: a line of assembly code
// Output: a list which contains that line of assembly code
std::vector<std::string> _tokenize(const std::string& str);