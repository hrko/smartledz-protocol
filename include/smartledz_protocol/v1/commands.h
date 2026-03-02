/**
 * @file
 * @brief Smart LEDZ v1 mesh command builders and opcode constants.
 */

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

namespace smartledz_protocol {
namespace v1 {

/**
 * @brief Smart LEDZ protocol v1 command and notify helpers.
 */

/// @name Mesh command opcodes.
/// @{
constexpr uint8_t kOpcodeOnOff = 0xD0;
constexpr uint8_t kOpcodeBrightness = 0xD2;
constexpr uint8_t kOpcodeColor = 0xE2;
constexpr uint8_t kOpcodeStatusQuery = 0xDA;
constexpr uint8_t kOpcodeDimmingQuery = 0xF0;
/// @}

/// @name Notify opcodes emitted by devices.
/// @{
constexpr uint8_t kNotifyOpcodeOnlineStatus = 0xDC;
constexpr uint8_t kNotifyOpcodeStatus = 0xDB;
constexpr uint8_t kNotifyOpcodeExtended = 0xEA;
/// @}

/**
 * @brief Mesh command frame used by the session transport layer.
 */
struct MeshCommand {
  /// Mesh opcode (for example `kOpcodeOnOff`, `kOpcodeColor`).
  uint8_t opcode{0};
  /// Command payload bytes; only the first `payload_len` bytes are used.
  std::array<uint8_t, 10> payload{};
  /// Number of valid bytes in `payload`.
  uint8_t payload_len{0};
};

/**
 * @brief Builds an on/off command.
 * @param on `true` sends `0x01`, `false` sends `0x00`.
 * @return Command with opcode `kOpcodeOnOff` and payload length 1.
 */
MeshCommand make_on_off(bool on);

/**
 * @brief Builds a brightness command.
 * @param brightness_pct Brightness percentage byte expected by the device.
 * @return Command with opcode `kOpcodeBrightness` and payload length 1.
 * @note The value is forwarded as-is and is not clamped.
 */
MeshCommand make_brightness(uint8_t brightness_pct);

/**
 * @brief Builds an RGB color command.
 * @param red Red channel (0..255).
 * @param green Green channel (0..255).
 * @param blue Blue channel (0..255).
 * @return Command with opcode `kOpcodeColor` and payload `[0x04, R, G, B]`.
 */
MeshCommand make_rgb(uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief Builds a color-temperature command using the raw protocol unit.
 * @param ct_raw Raw CT byte from 18 (warmest) to 65 (coolest).
 * @return Command with opcode `kOpcodeColor` and payload `[0x05, CT]`.
 * @note Values are clamped to the supported range `[18, 65]`.
 */
MeshCommand make_ct_raw(uint8_t ct_raw);

/**
 * @brief Builds a status query command.
 * @return Command with opcode `kOpcodeStatusQuery` and payload `[0x10]`.
 */
MeshCommand make_status_query();

/**
 * @brief Builds a dimming query command (`0xF1` request wrapper).
 * @return Command with opcode `kOpcodeDimmingQuery` and fixed 5-byte payload.
 */
MeshCommand make_dimming_query();

}  // namespace v1
}  // namespace smartledz_protocol
