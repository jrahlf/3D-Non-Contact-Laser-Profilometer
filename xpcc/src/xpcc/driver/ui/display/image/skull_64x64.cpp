
#include <xpcc/architecture/driver/accessor.hpp>

namespace bitmap
{
	FLASH_STORAGE(uint8_t skull_64x64[]) =
	{
		64, 64,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xe0, 0x60, 0x30, 0x18, 0x08, 0x0c, 0x0c, 0x04, 0x06, 0x06, 0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x02, 0x02, 0x06, 0x06, 0x04, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x60, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x3e, 0x07, 0x01, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x03, 0x0f, 0x7c, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7f, 0xf0, 0x80, 0x03, 0x3f, 0xf0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xf0, 0x3f, 0x03, 0x80, 0xf0, 0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0xc0, 0xc0, 0x40, 0xc0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x0e, 0xfc, 0x7f, 0x00, 0x00, 0x3e, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x1f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0x3c, 0x00, 0x00, 0x7f, 0xfc, 0x0e, 0x07, 0x01, 0x00, 0x00, 0x00, 0x80, 0x80, 0xc0, 0x80, 0x80, 0x00, 0x00, 0x00,
		0xf0, 0xb8, 0x9e, 0x8f, 0x80, 0x80, 0x80, 0x83, 0x07, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x60, 0x6f, 0xfc, 0xf0, 0xe0, 0xc0, 0xc0, 0xc1, 0xc1, 0x83, 0x03, 0x01, 0x00, 0x00, 0x00, 0xfc, 0xfe, 0xff, 0x00, 0xff, 0xfe, 0xf8, 0x00, 0x00, 0x00, 0x01, 0x03, 0x83, 0x81, 0xc1, 0xc0, 0xe0, 0xe0, 0xf0, 0xfc, 0xcf, 0x40, 0x60, 0x30, 0x10, 0x1c, 0x0e, 0x03, 0x01, 0x00, 0x01, 0x1f, 0x30, 0x60, 0xc0,
		0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x03, 0x02, 0x06, 0x04, 0x0c, 0x0c, 0x18, 0x18, 0x30, 0x30, 0x63, 0xff, 0x31, 0x0d, 0xe3, 0x0f, 0x5c, 0xf0, 0x90, 0xf0, 0x11, 0xe1, 0x20, 0xe0, 0x10, 0xf1, 0x11, 0xf0, 0x90, 0x90, 0xdc, 0x0f, 0xf1, 0x0f, 0xf9, 0xff, 0xc7, 0x61, 0x61, 0x30, 0x30, 0x18, 0x18, 0x0c, 0x04, 0x06, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x01,
		0x00, 0x00, 0x00, 0x00, 0xe0, 0xb0, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x10, 0x18, 0x08, 0x0c, 0x0c, 0x86, 0xc2, 0xc3, 0x6f, 0x7e, 0x70, 0xc1, 0x83, 0x82, 0x07, 0x04, 0x07, 0x08, 0x0f, 0x09, 0x0f, 0x09, 0x0f, 0x09, 0x05, 0x04, 0x05, 0x82, 0xc3, 0xe0, 0x70, 0x7f, 0x6f, 0xc6, 0xcc, 0x8c, 0x08, 0x18, 0x10, 0x30, 0x20, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0x60, 0xc0, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x7e, 0x60, 0x40, 0x60, 0x30, 0x1c, 0x06, 0x02, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x03, 0x02, 0x02, 0x02, 0x06, 0x06, 0x06, 0x02, 0x02, 0x02, 0x03, 0x03, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x03, 0x02, 0x06, 0x1c, 0x30, 0x60, 0x40, 0x60, 0x3c, 0x06, 0x03, 0x01, 0x00, 0x00, 0x00,
	};
}

