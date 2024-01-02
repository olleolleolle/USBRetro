// sony_jog.c
#include "sony_jog.h"
#include "globals.h"

// check if device is Sony USB Jog Controller (PCVA-JC1)
bool is_sony_jog(uint16_t vid, uint16_t pid) {
  return ((vid == 0x054c && pid == 0x00c4)); // Sony USB Jog Controller (PCVA-JC1)
}

// check if 2 reports are different enough
bool diff_report_jog(sony_jog_report_t const* rpt1, sony_jog_report_t const* rpt2) {
  return memcmp(&rpt1, &rpt2, 3);
}

// process usb hid input reports
void process_sony_jog(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) {
  // previous report used to compare for changes
  static sony_jog_report_t prev_report[5] = { 0 };

  sony_jog_report_t out_report;
  memcpy(&out_report, report, sizeof(out_report));

  if (diff_report_jog(&prev_report[dev_addr-1], &out_report))
  {
    TU_LOG1("(jog) = (");
    if (out_report.turned) {
      if (out_report.turnedLeft) {
        TU_LOG1("-1 ");
      } else {
        TU_LOG1(" 1 ");
      }
    } else {
      TU_LOG1(" 0 ");
    }
    TU_LOG1(") DPad = ");
    if (out_report.up    ) TU_LOG1("Up ");
    if (out_report.down  ) TU_LOG1("Down ");
    if (out_report.left  ) TU_LOG1("Left ");
    if (out_report.right ) TU_LOG1("Right ");
    if (out_report.center) TU_LOG1("Center ");

    TU_LOG1("Buttons = ");
    if (out_report.a) TU_LOG1("A ");
    if (out_report.b) TU_LOG1("B ");
    if (out_report.c) TU_LOG1("C ");
    if (out_report.d) TU_LOG1("D ");
    if (out_report.e) TU_LOG1("E ");
    if (out_report.f) TU_LOG1("F ");
    TU_LOG1("\r\n");

    bool has_6btns = true;
    bool bttn_l = out_report.e && !out_report.center;
    bool bttn_r = out_report.f && !out_report.center;
    bool bttn_start = out_report.center && !out_report.e;
    bool bttn_z = out_report.center && out_report.e;

    buttons = (((bttn_r)           ? 0x00 : 0x8000) |
               ((bttn_l)           ? 0x00 : 0x4000) |
               ((out_report.c)     ? 0x00 : 0x2000) |
               ((out_report.d)     ? 0x00 : 0x1000) |
               ((has_6btns)        ? 0x00 : 0x0800) |
               ((false)            ? 0x00 : 0x0400) |
               ((false)            ? 0x00 : 0x0200) |
               ((false)            ? 0x00 : 0x0100) |
               ((out_report.left)  ? 0x00 : 0x0008) |
               ((out_report.down)  ? 0x00 : 0x0004) |
               ((out_report.right) ? 0x00 : 0x0002) |
               ((out_report.up)    ? 0x00 : 0x0001) |
               ((bttn_start)       ? 0x00 : 0x0080) |
               ((bttn_z)           ? 0x00 : 0x0040) |
               ((out_report.a)     ? 0x00 : 0x0020) |
               ((out_report.b)     ? 0x00 : 0x0010));

    uint8_t analog_1x = 128;
    uint8_t analog_1y = 128;
    uint8_t analog_2x = 128;
    uint8_t analog_2y = 128;

    if (out_report.turned) {
      if (out_report.turnedLeft) {
        analog_1x = 0xff;
      } else {
        analog_1x = 1;
      }
    }

    // add to accumulator and post to the state machine
    // if a scan from the host machine is ongoing, wait
    post_globals(dev_addr, instance, buttons, analog_1x, analog_1y, analog_2x, analog_2y, 0, 0, 0, 0);

    prev_report[dev_addr-1] = out_report;
  }
}

DeviceInterface sony_jog_interface = {
  .name = "Sony USB Jog Controller (PCVA-JC1)",
  .is_device = is_sony_jog,
  .process = process_sony_jog,
};
