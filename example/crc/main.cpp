#include "hal.hpp"

using CRC16_MODBUS = HAL::gd32f4::CRC16_MODBUS;

extern "C"
{
int main(void)
{
    const uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
    uint16_t crc = CRC16_MODBUS::calculate(data, sizeof(data));
    (void)crc;

    while (1)
        ;
}
}
