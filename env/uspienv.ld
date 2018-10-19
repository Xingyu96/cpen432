/*
 * uspienv.ld
 */

ENTRY(_start)

SECTIONS
{
	. = 0x8000;
	
	.text : {
		*(.text*)
	}

	_etext = .;

	.rodata : {
		*(.rodata*)
	}

	.data : {
		*(.data*)
	}

	__bss_start = .;

	.bss : {
		*(.bss*)
	}

  __bss_end = .;

  . = ALIGN(8);
  __heap_low = .; /* for _sbrk */
  . = . + 0x100000; /* 1mB of heap memory */
  __heap_top = .; /* for _sbrk */
  . = . + 0x10000; /* 64kB of svc stack memory */
  __svc_stack_top = .;
  . = . + 0x10000; /* 64kB of user stack memory */
  __user_stack_top = .;
  . = . + 0x1000; /* 4kB of irq stack memory */
  __irq_stack_top = .;
  __end = .;
	_end = .;

}
