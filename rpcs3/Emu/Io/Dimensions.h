#pragma once

#include "Emu/Io/usb_device.h"
#include "Utilities/mutex.h"
#include <array>
#include <queue>

// struct infinity_figure
// {
// 	fs::file inf_file;
// 	std::array<u8, 0x14 * 0x10> data{};
// 	bool present = false;
// 	u8 order_added = 255;
// 	void save();
// };

class dimensions_toypad
{
public:
	

protected:
	

private:
	
};

extern dimensions_toypad g_dimensionstoypad;

class usb_device_dimensions : public usb_device_emulated
{
public:
	usb_device_dimensions(const std::array<u8, 7>& location);
	~usb_device_dimensions();

	void control_transfer(u8 bmRequestType, u8 bRequest, u16 wValue, u16 wIndex, u16 wLength, u32 buf_size, u8* buf, UsbTransfer* transfer) override;
	void interrupt_transfer(u32 buf_size, u8* buf, u32 endpoint, UsbTransfer* transfer) override;

protected:
	std::queue<std::array<u8, 32>> m_queries;
};

std::string HexDump(const u8* data, size_t size);
