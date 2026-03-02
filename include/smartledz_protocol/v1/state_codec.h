/**
 * @file
 * @brief Stateful decoders for Smart LEDZ v1 notify payloads.
 */

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <map>
#include <vector>

namespace smartledz_protocol {
namespace v1 {

/**
 * @brief Last known state fields decoded from mesh notifications.
 */
struct DeviceStateSnapshot {
  /// True after at least one payload was applied for this address.
  bool seen{false};
  /// Raw online status byte from `0xDC` online-status payloads.
  uint8_t online_status{0};
  /// True when `online_brightness` is valid.
  bool has_online_brightness{false};
  /// Brightness byte from online-status payload.
  uint8_t online_brightness{0};
  /// True when `power` has been derived from a payload.
  bool has_power{false};
  /// Derived power state (`status != 0 && brightness != 0`).
  bool power{false};
  /// True when `brightness` has been decoded from any payload type.
  bool has_brightness{false};
  /// Brightness byte from dimming/online payloads.
  uint8_t brightness{0};
  /// Device type/raw mode byte seen in online-status payloads.
  uint8_t type_raw{0};
  /// True when `ct_raw` is valid.
  bool has_ct{false};
  /// Raw CT byte (18..65 in currently known payloads).
  uint8_t ct_raw{0};
  /// True when `rgb` is valid.
  bool has_rgb{false};
  /// RGB channels decoded from status payload.
  std::array<uint8_t, 3> rgb{0, 0, 0};
  /// Caller-provided timestamp for latest decoded update.
  uint32_t last_update_ms{0};
};

/**
 * @brief Applies an online-status notify payload (`opcode 0xDC`).
 * @param payload10 Pointer to notify payload bytes.
 * @param payload_len Number of bytes in `payload10`.
 * @param now_ms Caller timestamp used to set `last_update_ms`.
 * @param states Address-to-state map to update.
 * @param updated_addresses Optional output vector appended with updated addresses.
 * @note Payload is parsed as repeated 4-byte tuples:
 *       `[addr, status, brightness, type_raw]`.
 *       Parsing stops at `addr == 0x00` or sentinel tuple `0xFF, *, 0xFF, *`.
 */
void apply_online_status_payload(const uint8_t *payload10, size_t payload_len, uint32_t now_ms,
                                 std::map<uint16_t, DeviceStateSnapshot> *states,
                                 std::vector<uint16_t> *updated_addresses);

/**
 * @brief Applies a status notify payload (`opcode 0xDB`) for one source address.
 * @param src Source mesh address from the decrypted frame.
 * @param payload10 Pointer to notify payload bytes.
 * @param payload_len Number of bytes in `payload10`.
 * @param now_ms Caller timestamp used to set `last_update_ms`.
 * @param states Address-to-state map to update.
 * @param updated_addresses Optional output vector appended with `src` when decoded.
 * @note Requires at least 4 bytes and decodes:
 *       `payload[0..2] => RGB`, `payload[3] => CT raw when in [18, 65]`.
 */
void apply_status_payload(uint16_t src, const uint8_t *payload10, size_t payload_len, uint32_t now_ms,
                          std::map<uint16_t, DeviceStateSnapshot> *states,
                          std::vector<uint16_t> *updated_addresses);

/**
 * @brief Applies an extended dimming response payload (`opcode 0xEA`, body starts with `0xF1`).
 * @param src Source mesh address from the decrypted frame.
 * @param payload10 Pointer to notify payload bytes.
 * @param payload_len Number of bytes in `payload10`.
 * @param now_ms Caller timestamp used to set `last_update_ms`.
 * @param states Address-to-state map to update.
 * @param updated_addresses Optional output vector appended with `src` when decoded.
 * @note This decoder accepts only `func=1`, `item=4`, and at least one data byte;
 *       brightness is taken from `payload[6]`.
 */
void apply_f1_response_payload(uint16_t src, const uint8_t *payload10, size_t payload_len, uint32_t now_ms,
                               std::map<uint16_t, DeviceStateSnapshot> *states,
                               std::vector<uint16_t> *updated_addresses);

}  // namespace v1
}  // namespace smartledz_protocol
