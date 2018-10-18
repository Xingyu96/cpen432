//
// main.c
//
#include <uspienv.h>
#include <uspi.h>
#include <uspios.h>
#include <uspienv/util.h>
#include <uspienv/macros.h>
#include <uspienv/types.h>
#include <uart.h>
#include <printk.h>

#define	OWN_IP_ADDRESS		{192, 168, 1, 250}	// must be a valid IP address on your LAN

#define MAC_ADDRESS_SIZE	6
#define IP_ADDRESS_SIZE		4

void debug_foo(void);

typedef struct TEthernetHeader
{
	u8	MACReceiver[MAC_ADDRESS_SIZE];
	u8	MACSender[MAC_ADDRESS_SIZE];
	u16	nProtocolType;
#define ETH_PROT_ARP		0x806
}
PACKED TEthernetHeader;

typedef struct TARPPacket
{
	u16		nHWAddressSpace;
#define HW_ADDR_ETHER		1
	u16		nProtocolAddressSpace;
#define PROT_ADDR_IP		0x800
	u8		nHWAddressLength;
	u8		nProtocolAddressLength;
	u16		nOPCode;
#define ARP_REQUEST		1
#define ARP_REPLY		2
	u8		HWAddressSender[MAC_ADDRESS_SIZE];
	u8		ProtocolAddressSender[IP_ADDRESS_SIZE];
	u8		HWAddressTarget[MAC_ADDRESS_SIZE];
	u8		ProtocolAddressTarget[IP_ADDRESS_SIZE];
}
PACKED TARPPacket;

typedef struct TMyPacket
{
	u8 Command;
}
PACKED TMyPacket;

typedef struct TARPFrame
{
	TEthernetHeader Ethernet;
	TARPPacket	ARP;
}
PACKED TARPFrame;

typedef struct TMyFrame
{
	TEthernetHeader Ethernet;
	TMyPacket Packet;
}
PACKED TMyFrame;

static const u8 OwnIPAddress[] = OWN_IP_ADDRESS;

static const char FromSample[] = "sample";

int main (void)
{
	uart_init();
	printk("own ip address is %u.%u.%u.%u \n",
			(unsigned) OwnIPAddress[0],
			(unsigned) OwnIPAddress[1],
			(unsigned) OwnIPAddress[2],
			(unsigned) OwnIPAddress[3]);
	
	if (!USPiEnvInitialize ())
	{
		printk("Cannot initialize USPi env\n");
		return EXIT_HALT;
	}
	
	if (!USPiInitialize ())
	{
		LogWrite (FromSample, LOG_ERROR, "Cannot initialize USPi");
		printk("Cannot initialize USPi\n");
		
		USPiEnvClose ();

		return EXIT_HALT;
	}

	if (!USPiEthernetAvailable ())
	{
		LogWrite (FromSample, LOG_ERROR, "Ethernet device not found");
		printk("Ethernet device not found\n");
		
		USPiEnvClose ();

		return EXIT_HALT;
	}

	u8 OwnMACAddress[MAC_ADDRESS_SIZE];
	USPiGetMACAddress (OwnMACAddress);
	printk("own mac address is %x.%x.%x.%x.%x.%x \n",
			(unsigned) OwnMACAddress[0],
			(unsigned) OwnMACAddress[1],
			(unsigned) OwnMACAddress[2],
			(unsigned) OwnMACAddress[3],
			(unsigned) OwnMACAddress[4],
			(unsigned) OwnMACAddress[5]);

	u8 TheirMACAddress[MAC_ADDRESS_SIZE];
	TheirMACAddress[0] = 184;
	TheirMACAddress[1] = 39;
	TheirMACAddress[2] = 235;
	TheirMACAddress[3] = 44;
	TheirMACAddress[4] = 185;
	TheirMACAddress[5] = 253;
	printk("Their mac address is %x.%x.%x.%x.%x.%x \n",
			(unsigned) TheirMACAddress[0],
			(unsigned) TheirMACAddress[1],
			(unsigned) TheirMACAddress[2],
			(unsigned) TheirMACAddress[3],
			(unsigned) TheirMACAddress[4],
			(unsigned) TheirMACAddress[5]);


	while (1)
	{
		u8 Buffer[USPI_FRAME_BUFFER_SIZE];
		TMyFrame *pMyFrame = (TMyFrame *) Buffer;
		uint16_t protocol_type = BE (0x800);
		uint8_t command = 80;

		memcpy ((u8 *)pMyFrame->Ethernet.MACReceiver, TheirMACAddress, MAC_ADDRESS_SIZE);
		memcpy ((u8 *)pMyFrame->Ethernet.MACSender, OwnMACAddress, MAC_ADDRESS_SIZE);
		memcpy ((u16 *)&pMyFrame->Ethernet.nProtocolType, &protocol_type, sizeof(u16));

		memcpy ((u8 *)&pMyFrame->Packet.Command, &command, sizeof(uint8_t));

		if (!USPiSendFrame (pMyFrame, sizeof *pMyFrame))
		{
			LogWrite (FromSample, LOG_ERROR, "USPiSendFrame failed");
			printk("USPiSendFrame failed\n");

			break;
		}


	}

	USPiEnvClose ();

	return EXIT_HALT;
}
