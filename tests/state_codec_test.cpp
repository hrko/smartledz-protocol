#include <smartledz_protocol/v1/state_codec.h>
#include <cassert>
#include <iostream>
using namespace smartledz_protocol::v1;

int main() {
  std::map<uint16_t, DeviceStateSnapshot> states;
  std::vector<uint16_t> updates;
  uint8_t record[] = {1, 1, 70, 3};
  apply_online_status_payload(record, sizeof(record), 1, &states, &updates);
  assert(states[1].has_reachability && states[1].reachable);
  assert(states[1].has_power && states[1].power && states[1].brightness == 70);
  for (uint8_t stale_brightness : {0, 255}) {
    record[1] = 0;
    record[2] = stale_brightness;
    apply_online_status_payload(record, sizeof(record), 2, &states, &updates);
    assert(!states[1].reachable && states[1].online_status == 0);
    assert(states[1].has_power && states[1].power);
    assert(states[1].brightness == 70 && states[1].online_brightness == 70);
    assert(states[1].last_update_ms == 2 && updates.back() == 1);
  }
  record[0] = 2;
  apply_online_status_payload(record, sizeof(record), 3, &states, nullptr);
  assert(states[2].has_reachability && !states[2].reachable);
  assert(!states[2].has_power && !states[2].has_brightness && !states[2].has_online_brightness);
  record[0] = 1;
  record[1] = 1;
  record[2] = 0;
  apply_online_status_payload(record, sizeof(record), 4, &states, nullptr);
  assert(states[1].reachable && states[1].has_power && !states[1].power);
  const uint8_t color[] = {10, 20, 30, 40};
  apply_status_payload(2, color, sizeof(color), 5, &states, nullptr);
  assert(states[2].reachable && !states[2].has_power);
  const uint8_t dimming[] = {0xf1, 0, 1, 4, 0x11, 2, 50, 0};
  apply_f1_response_payload(3, dimming, sizeof(dimming), 6, &states, nullptr);
  assert(states[3].has_reachability && states[3].reachable && states[3].brightness == 50);
  std::cout << "State reachability tests passed\n";
}
