//
// main.c
//
#include <uspienv.h>
#include <uspi.h>
#include <uspios.h>
#include <uspienv/util.h>
#include <uspienv/macros.h>
#include <uspienv/types.h>

#include <kstdint.h>
#include <uart.h>
#include <timer.h>
#include <printk.h>
#include <interrupts2.h>
#include <led.h>
#include <arm.h>

#define	OWN_IP_ADDRESS		{192, 168, 0, 250}	// must be a valid IP address on your LAN

#define MAC_ADDRESS_SIZE	6
#define IP_ADDRESS_SIZE		4

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

typedef struct TARPFrame
{
	TEthernetHeader Ethernet;
	TARPPacket	ARP;
}
PACKED TARPFrame;

void DelayLoop (unsigned nCount);

static const u8 OwnIPAddress[] = OWN_IP_ADDRESS;

static const char FromSample[] = "sample";

int main (void)
{
	uart_init();
		/* This sets the interrupt enable bit(s) in CPSR (FIQ, IRQ, SVC, etc) */
	  enable_interrupts();
	  printk("enabled interrupts\n");

	  /* This enables arm timer and uart interrupt */
	  enable_arm_irq();
	  printk("enabled arm irq\n");
    printk("own ip address is %u.%u.%u.%u \n",
    (unsigned) OwnIPAddress[0],
    (unsigned) OwnIPAddress[1],
    (unsigned) OwnIPAddress[2],
    (unsigned) OwnIPAddress[3]);

	  int freq = TIMER_IRQ_FREQ;
	  timer_start(freq);
	  
	  printk("The timer frequency is: %d\n\n", freq);
	  printk("The available commands are: \n");
	  printk("'r' - RESET\n");
	  printk("'1' - 0.5 SECONDS CYCLE\n");
	  printk("'2' - 2 SECONDS CYCLE\n");
	  printk("'3' - 5 SECONDS CYCLE\n");
		  
	  // init led
	  led_init();
	  printk("init led\n");
	  
	  // ethernet
	  /*if (!USPiEnvInitialize ())
		{
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

		while (1)
		{
			u8 Buffer[USPI_FRAME_BUFFER_SIZE];
			unsigned nFrameLength;
			if (!USPiReceiveFrame (Buffer, &nFrameLength))
			{
				continue;
			}

			LogWrite (FromSample, LOG_NOTICE, "Frame received (length %u)", nFrameLength);
			printk("Frame received (length %u)\n", nFrameLength);

			if (nFrameLength < sizeof (TARPFrame))
			{
				continue;
			}

			TARPFrame *pARPFrame = (TARPFrame *) Buffer;
			if (   pARPFrame->Ethernet.nProtocolType	!= BE (ETH_PROT_ARP)
				|| pARPFrame->ARP.nHWAddressSpace		!= BE (HW_ADDR_ETHER)
				|| pARPFrame->ARP.nProtocolAddressSpace	!= BE (PROT_ADDR_IP)
				|| pARPFrame->ARP.nHWAddressLength		!= MAC_ADDRESS_SIZE
				|| pARPFrame->ARP.nProtocolAddressLength	!= IP_ADDRESS_SIZE
				|| pARPFrame->ARP.nOPCode			!= BE (ARP_REQUEST))
			{
				continue;
			}

			LogWrite (FromSample, LOG_NOTICE, "Valid ARP request from %u.%u.%u.%u received",
				  (unsigned) pARPFrame->ARP.ProtocolAddressSender[0],
				  (unsigned) pARPFrame->ARP.ProtocolAddressSender[1],
				  (unsigned) pARPFrame->ARP.ProtocolAddressSender[2],
				  (unsigned) pARPFrame->ARP.ProtocolAddressSender[3]);
				  
			printk("valid arp request from %u.%u.%u.%u received\n",
					(unsigned) pARPFrame->ARP.ProtocolAddressSender[0],
				  (unsigned) pARPFrame->ARP.ProtocolAddressSender[1],
				  (unsigned) pARPFrame->ARP.ProtocolAddressSender[2],
				  (unsigned) pARPFrame->ARP.ProtocolAddressSender[3]);
			printk("target address is %u.%u.%u.%u \n",
					(unsigned) pARPFrame->ARP.ProtocolAddressTarget[0],
				  (unsigned) pARPFrame->ARP.ProtocolAddressTarget[1],
				  (unsigned) pARPFrame->ARP.ProtocolAddressTarget[2],
				  (unsigned) pARPFrame->ARP.ProtocolAddressTarget[3]);

			if (memcmp (pARPFrame->ARP.ProtocolAddressTarget, OwnIPAddress, IP_ADDRESS_SIZE) != 0)
			{
				continue;
			}

			LogWrite (FromSample, LOG_NOTICE, "ARP request is to us");
			printk("ARP request is to us\n");
	

			// prepare reply packet
			memcpy (pARPFrame->Ethernet.MACReceiver, pARPFrame->ARP.HWAddressSender, MAC_ADDRESS_SIZE);
			memcpy (pARPFrame->Ethernet.MACSender, OwnMACAddress, MAC_ADDRESS_SIZE);
			pARPFrame->ARP.nOPCode = BE (ARP_REPLY);

			memcpy (pARPFrame->ARP.HWAddressTarget, pARPFrame->ARP.HWAddressSender, MAC_ADDRESS_SIZE);
			memcpy (pARPFrame->ARP.ProtocolAddressTarget, pARPFrame->ARP.ProtocolAddressSender, IP_ADDRESS_SIZE);

			memcpy (pARPFrame->ARP.HWAddressSender, OwnMACAddress, MAC_ADDRESS_SIZE);
			memcpy (pARPFrame->ARP.ProtocolAddressSender, OwnIPAddress, IP_ADDRESS_SIZE);
	
			if (!USPiSendFrame (pARPFrame, sizeof *pARPFrame))
			{
				LogWrite (FromSample, LOG_ERROR, "USPiSendFrame failed");
				printk("USPiSendFrame failed\n");

				break;
			}

			LogWrite (FromSample, LOG_NOTICE, "ARP reply successfully sent");
			printk("ARP reply successfully sent\n");
		}

		USPiEnvClose ();*/
		
	  while (1) {
	  	DelayLoop(250000);
	  	//turn_led_on();
	  	DelayLoop(250000);
	  	//turn_led_off();
      //printk("hi\n");
      //printk("timer is pending? = %d\n", timer_is_pending());
	  }
	  return EXIT_HALT;
	
}
