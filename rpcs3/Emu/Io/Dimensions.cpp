#include "stdafx.h"
#include "Dimensions.h"
#include "Emu/Cell/lv2/sys_usbd.h"

#include "util/asm.hpp"

#include <locale>

LOG_CHANNEL(dimensions_log, "dimensions");

usb_device_dimensions::usb_device_dimensions(const std::array<u8, 7>& location)
	: usb_device_emulated(location)
{
	device = UsbDescriptorNode(USB_DESCRIPTOR_DEVICE, UsbDeviceDescriptor{0x200, 0x0, 0x0, 0x0, 0x20, 0x0E6F, 0x0241, 0x200, 0x1, 0x2, 0x3, 0x1});
	auto& config0 = device.add_node(UsbDescriptorNode(USB_DESCRIPTOR_CONFIG, UsbDeviceConfiguration{0x29, 0x1, 0x1, 0x0, 0x80, 0xFA}));
	config0.add_node(UsbDescriptorNode(USB_DESCRIPTOR_INTERFACE, UsbDeviceInterface{0x0, 0x0, 0x2, 0x3, 0x0, 0x0, 0x0}));
	config0.add_node(UsbDescriptorNode(USB_DESCRIPTOR_HID, UsbDeviceHID{0x0111, 0x00, 0x01, 0x22, 0x001d}));
	config0.add_node(UsbDescriptorNode(USB_DESCRIPTOR_ENDPOINT, UsbDeviceEndpoint{0x81, 0x3, 0x20, 0x1}));
	config0.add_node(UsbDescriptorNode(USB_DESCRIPTOR_ENDPOINT, UsbDeviceEndpoint{0x1, 0x3, 0x20, 0x1}));
}

usb_device_dimensions::~usb_device_dimensions()
{
}

void usb_device_dimensions::control_transfer(u8 bmRequestType, u8 bRequest, u16 wValue, u16 wIndex, u16 wLength, u32 buf_size, u8* buf, UsbTransfer* transfer)
{
	dimensions_log.notice("Control Transfer: %s", HexDump(buf, buf_size));
	dimensions_log.notice("Length: %02x", buf_size);
    usb_device_emulated::control_transfer(bmRequestType, bRequest, wValue, wIndex, wLength, buf_size, buf, transfer);
}

void usb_device_dimensions::interrupt_transfer(u32 buf_size, u8* buf, u32 endpoint, UsbTransfer* transfer)
{
    ensure(buf_size == 0x20);

    dimensions_log.notice("Interrupt Transfer: %s", HexDump(buf, buf_size));
	transfer->fake = true;
	transfer->expected_count = buf_size;
	transfer->expected_result = HC_CC_NOERR;
    transfer->expected_time = get_timestamp() + 20000;
}

std::string HexDump(const u8* data, size_t size)
{
	constexpr size_t BYTES_PER_LINE = 16;

	std::string out;
	for (size_t row_start = 0; row_start < size; row_start += BYTES_PER_LINE)
	{
		out += fmt::format("%06x: ", row_start);
		for (size_t i = 0; i < BYTES_PER_LINE; ++i)
		{
			if (row_start + i < size)
			{
				out += fmt::format("%02x ", data[row_start + i]);
			}
			else
			{
				out += "   ";
			}
		}
		out += " ";
		for (size_t i = 0; i < BYTES_PER_LINE; ++i)
		{
			if (row_start + i < size)
			{
				char c = static_cast<char>(data[row_start + i]);
				out += std::isprint(c, std::locale::classic()) ? c : '.';
			}
		}
		out += "\n";
	}
	return out;
}
