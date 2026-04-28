#include <unordered_map>
#include <string>
#include <cstdint>

extern const std::unordered_map<std::string, uint8_t> registerMap;
extern const std::unordered_map<std::string, uint8_t> functMap;
extern const std::unordered_map<std::string, uint8_t> opcodeMap;

extern const std::unordered_map<uint8_t, std::string> registerMap_reverse;
extern const std::unordered_map<uint8_t, std::string> functMap_reverse;
extern const std::unordered_map<uint8_t, std::string> opcodeMap_reverse;