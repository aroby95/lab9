#include <stdio.h>
#include <sys/mman.h> 
#include <fcntl.h> 
#include <unistd.h>
#define _BSD_SOURCE

#define GPIO_DIP_OFFSET 2
#define GPIO_LED_OFFSET 0 
#define DIPS_AND_LEDS_BASEADDR 0x41200000
#define RGB_LED_BASEADDR 0x41210000

// the below code uses a device called /dev/mem to get a pointer to a physical
// address.  We will use this pointer to read/write the custom peripheral
volatile unsigned int * get_a_pointer(unsigned int phys_addr)
{

	int mem_fd = open("/dev/mem", O_RDWR | O_SYNC); 
	void *map_base = mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, mem_fd, phys_addr); 
	volatile unsigned int *radio_base = (volatile unsigned int *)map_base; 
	return (radio_base);
}

int main()
{

// first, get a pointer to the peripheral base address using /dev/mem and the function mmap
    volatile unsigned int *dipsandleds_ptr = get_a_pointer(DIPS_AND_LEDS_BASEADDR);	
    volatile unsigned int *rgb_leds = get_a_pointer(RGB_LED_BASEADDR);
    *rgb_leds = 0b000000;
    volatile unsigned int count = 0;
    printf("\r\n\r\n\r\nLab 6 Example Program\n\r");
    while (1)
    {
	    // *(dipsandleds_ptr+GPIO_LED_OFFSET) = 0xf;
	    // usleep(500000);
	    // *(dipsandleds_ptr+GPIO_LED_OFFSET) = 0x0;	
		// usleep(500000);
	    // // for demonstration purposes, print the value of the DIPS
	    // printf("Current Switches = %d\r\n",*(dipsandleds_ptr+GPIO_DIP_OFFSET));
        unsigned int current_dips3 = *(dipsandleds_ptr+GPIO_DIP_OFFSET) & 0b111;
        *rgb_leds = current_dips3 | (current_dips3 << 3);
        *(dipsandleds_ptr+GPIO_LED_OFFSET) = count; 
        usleep(500000);
        count++;
        count = count % (0b10000);
    }
    return 0;
}
