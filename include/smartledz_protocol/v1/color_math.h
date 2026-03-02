/**
 * @file
 * @brief Color-temperature and RGB conversion helpers for Smart LEDZ v1.
 */

#pragma once

#include <array>
#include <cstdint>

namespace smartledz_protocol {
namespace v1 {

/**
 * @brief Converts correlated color temperature and Duv into RGB.
 * @param kelvin Correlated color temperature in kelvin.
 * @param duv Duv offset used by the fitted model.
 * @return RGB tuple in 8-bit channels.
 * @note `kelvin` is clamped to `[1800, 12000]` and `duv` to `[-6.0, 6.0]`.
 */
std::array<uint8_t, 3> lookup_ct_rgb(uint16_t kelvin, float duv);

/**
 * @brief Estimates correlated color temperature from an RGB sample.
 * @param target_red Red channel of the target sample.
 * @param target_green Green channel of the target sample.
 * @param target_blue Blue channel of the target sample.
 * @param duv Duv offset used by the fitted model.
 * @return Estimated kelvin value in the modeled range.
 * @note Uses coarse (100K) then fine (1K) search over `[1800, 12000]`.
 */
uint16_t estimate_cct_from_rgb(uint8_t target_red, uint8_t target_green, uint8_t target_blue, float duv);

}  // namespace v1
}  // namespace smartledz_protocol
