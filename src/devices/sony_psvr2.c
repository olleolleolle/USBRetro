// sony_psvr2.c
#include "sony_psvr2.h"
#include "globals.h"

// check if device is PlayStation Classic controller
bool is_sony_psvr2(uint16_t vid, uint16_t pid) {
  return vid == 0x054c && (pid == 0x0e45 || pid == 0x0e46); // Sony PSVR2 Sense controllers
}

// check if 2 reports are different enough
bool diff_report_psvr2(sony_psvr2_report_t const* rpt1, sony_psvr2_report_t const* rpt2) {
  bool result;

  // x1, y1, x2, y2, rx, ry must different than 2 to be counted
  result = diff_than_n(rpt1->x1, rpt2->x1, 2) || diff_than_n(rpt1->y1, rpt2->y1, 2) ||
           diff_than_n(rpt1->x2, rpt2->x2, 2) || diff_than_n(rpt1->y2, rpt2->y2, 2) ||
           diff_than_n(rpt1->rx, rpt2->rx, 2) || diff_than_n(rpt1->ry, rpt2->ry, 2);

  // check the reset with mem compare
  result |= memcmp(&rpt1->rz + 1, &rpt2->rz + 1, sizeof(sony_psvr2_report_t)-7);

  return result;
}

// process usb hid input reports
void process_sony_psvr2(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len) {
  // previous report used to compare for changes
  static sony_psvr2_report_t prev_report[5] = { 0 };

  uint8_t const report_id = report[0];
  report++;
  len--;

  // all buttons state is stored in ID 1
  if (report_id == 1)
  {
    sony_psvr2_report_t psvr2_report;
    memcpy(&psvr2_report, report, sizeof(psvr2_report));

    // counter is +1, assign to make it easier to compare 2 report
    prev_report[dev_addr-1].counter = psvr2_report.counter;

    if ( diff_report_psvr2(&prev_report[dev_addr-1], &psvr2_report) )
    {
      TU_LOG1("(x1, y1, x2, y2, rx, ry, rz) = (%u, %u, %u, %u, %u, %u, %u)\r\n", psvr2_report.x1, psvr2_report.y1, psvr2_report.x2, psvr2_report.y2, psvr2_report.rx, psvr2_report.ry, psvr2_report.rz);
      if (psvr2_report.square    ) TU_LOG1("Square ");
      if (psvr2_report.cross     ) TU_LOG1("Cross ");
      if (psvr2_report.circle    ) TU_LOG1("Circle ");
      if (psvr2_report.triangle  ) TU_LOG1("Triangle ");

      if (psvr2_report.l1        ) TU_LOG1("L1 ");
      if (psvr2_report.r1        ) TU_LOG1("R1 ");
      if (psvr2_report.l2        ) TU_LOG1("L2 ");
      if (psvr2_report.r2        ) TU_LOG1("R2 ");
      if (psvr2_report.l3        ) TU_LOG1("L3 ");
      if (psvr2_report.r3        ) TU_LOG1("R3 ");

      if (psvr2_report.share     ) TU_LOG1("Share ");
      if (psvr2_report.option    ) TU_LOG1("Option ");
      if (psvr2_report.ps        ) TU_LOG1("PS ");
      if (psvr2_report.t_l2      ) TU_LOG1("Touch(L2) ");
      if (psvr2_report.t_triangle) TU_LOG1("Touch(Triangle) ");
      if (psvr2_report.t_square  ) TU_LOG1("Touch(Square) ");
      if (psvr2_report.t_l3      ) TU_LOG1("Touch(L3) ");
      if (psvr2_report.t_l1      ) TU_LOG1("Touch(L1) ");

      TU_LOG1("\r\n");

      int threshold = 28;
      bool dpad_up    = false;//(psvr2_report.dpad == 0 || psvr2_report.dpad == 1 || psvr2_report.dpad == 7);
      bool dpad_right = false;//((psvr2_report.dpad >= 1 && psvr2_report.dpad <= 3));
      bool dpad_down  = false;//((psvr2_report.dpad >= 3 && psvr2_report.dpad <= 5));
      bool dpad_left  = false;//((psvr2_report.dpad >= 5 && psvr2_report.dpad <= 7));
      bool has_6btns = true;

      buttons = (((psvr2_report.r3)       ? 0x00 : 0x20000) |
                 ((psvr2_report.l3)       ? 0x00 : 0x10000) |
                 ((psvr2_report.r1)       ? 0x00 : 0x08000) |
                 ((psvr2_report.l1)       ? 0x00 : 0x04000) |
                 ((psvr2_report.square)   ? 0x00 : 0x02000) |
                 ((psvr2_report.triangle) ? 0x00 : 0x01000) |
                 ((has_6btns)             ? 0x00 : 0x00800) |
                 ((psvr2_report.ps)       ? 0x00 : 0x00400) |
                 ((psvr2_report.r2)       ? 0x00 : 0x00200) |
                 ((psvr2_report.l2)       ? 0x00 : 0x00100) |
                 ((dpad_left)             ? 0x00 : 0x00008) |
                 ((dpad_down)             ? 0x00 : 0x00004) |
                 ((dpad_right)            ? 0x00 : 0x00002) |
                 ((dpad_up)               ? 0x00 : 0x00001) |
                 ((psvr2_report.option)   ? 0x00 : 0x00080) |
                 ((psvr2_report.share)      ? 0x00 : 0x00040) |
                 ((psvr2_report.cross)    ? 0x00 : 0x00020) |
                 ((psvr2_report.circle)   ? 0x00 : 0x00010));

      uint8_t analog_1x = psvr2_report.x1+1;
      uint8_t analog_1y = psvr2_report.y1+1;
      uint8_t analog_2x = psvr2_report.x2+1;
      uint8_t analog_2y = psvr2_report.y2+1;
      uint8_t analog_l = psvr2_report.l2;
      uint8_t analog_r = psvr2_report.r2;

      // keep analog within range [1-255]
      ensureAllNonZero(&analog_1x, &analog_1y, &analog_2x, &analog_2y);

      // add to accumulator and post to the state machine
      // if a scan from the host machine is ongoing, wait
      post_globals(
        dev_addr,
        instance,
        buttons,
        analog_1x, // Left Analog X
        analog_1y, // Left Analog Y
        analog_2x, // Right Analog X
        analog_2y, // Right Analog Y
        analog_l,  // Left Trigger
        analog_r,  // Right Trigger
        0,
        0          // Spinner Quad X
      );
    }

    prev_report[dev_addr-1] = psvr2_report;
  }
}

DeviceInterface sony_psvr2_interface = {
  .name = "Sony PlayStation Classic",
  .is_device = is_sony_psvr2,
  .process = process_sony_psvr2,
  .task = NULL,
  .init = NULL
};
