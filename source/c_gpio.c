/*
Copyright (c) 2012-2015 Ben Croston

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is furnished to do
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include "c_gpio.h"

static volatile uint32_t *gpio_map;

uint32_t readl(uint32_t addr)
{
	uint32_t val = 0;
	uint32_t mmap_base = (addr & ~MAP_MASK);
	uint32_t mmap_seek = ((addr - mmap_base) >> 2);
	val = *(gpio_map + mmap_seek);
	if(OPiGPIODebug)
		printf("mmap_base = 0x%x\t mmap_seek = 0x%x\t gpio_map = 0x%x\t total = 0x%x\n",mmap_base,mmap_seek,gpio_map,(gpio_map + mmap_seek));

	return val;
}

void writel(uint32_t val, uint32_t addr)
{
  uint32_t mmap_base = (addr & ~MAP_MASK);
  uint32_t mmap_seek = ((addr - mmap_base) >> 2);
  *(gpio_map + mmap_seek) = val;
}

int setup(void)
{
	int mem_fd;
	uint8_t *gpio_mem;

	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0)
	{
		return SETUP_DEVMEM_FAIL;
	}

	if ((gpio_mem = malloc(BLOCK_SIZE + (PAGE_SIZE-1))) == NULL)
		return SETUP_MALLOC_FAIL;

	if ((uint32_t)gpio_mem % PAGE_SIZE)
		gpio_mem += PAGE_SIZE - ((uint32_t)gpio_mem % PAGE_SIZE);

	gpio_map = (uint32_t *)mmap( (caddr_t)gpio_mem, BLOCK_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_FIXED, mem_fd, GPIO_BASE_OPI);

	if(OPiGPIODebug)
		printf("gpio_mem = 0x%x\t gpio_map = 0x%x\n",gpio_mem,gpio_map);

	if ((uint32_t)gpio_map < 0)
		return SETUP_MMAP_FAIL;

	return SETUP_OK;
}

int gpio_function(int gpio)
{
	uint32_t regval = 0;
	int bank = gpio >> 5;
	int index = gpio - (bank << 5);
	int offset = ((index - ((index >> 3) << 3)) << 2);
	uint32_t phyaddr = SUNXI_GPIO_BASE + (bank * 36) + ((index >> 3) << 2);

	regval = readl(phyaddr);
	if (OPiGPIODebug)
		printf("read reg val: 0x%x offset:%d\n",regval,offset);

	regval >>= offset;
	regval &= 7;
	if (OPiGPIODebug)
		printf("read reg val: 0x%x\n",regval);

	return regval; // 1=input, 0=output, 4=alt0
}

void set_pullupdn(int gpio, int pud)//void sunxi_pullUpDnControl (int pin, int pud)
{
	uint32_t regval = 0;
	int bank = gpio >> 5;
	int index = gpio - (bank << 5);
	int sub = index >> 4;
	int sub_index = index - 16*sub;
	uint32_t phyaddr = SUNXI_GPIO_BASE + (bank * 36) + 0x1C + 4*sub; // +0x1c -> pullUpDn reg

	if (OPiGPIODebug)
		printf("func:%s pin:%d,bank:%d index:%d sub:%d phyaddr:0x%x\n",__func__, gpio,bank,index,sub,phyaddr);

	regval = readl(phyaddr);
	if (OPiGPIODebug)
		printf("pullUpDn reg:0x%x, pud:0x%x sub_index:%d\n", regval, pud, sub_index);
	regval &= ~(3 << (sub_index << 1));
	regval |= (pud << (sub_index << 1));
	if (OPiGPIODebug)
		printf("pullUpDn val ready to set:0x%x\n", regval);
	writel(regval, phyaddr);
	regval = readl(phyaddr);
	if (OPiGPIODebug)
		printf("pullUpDn reg after set:0x%x  addr:0x%x\n", regval, phyaddr);
}

void setup_gpio(int gpio, int direction, int pud)//void sunxi_set_gpio_mode(int pin,int mode)
{
	uint32_t regval = 0;
	int bank = gpio >> 5;
	int index = gpio - (bank << 5);
	int offset = ((index - ((index >> 3) << 3)) << 2);
	uint32_t phyaddr = SUNXI_GPIO_BASE + (bank * 36) + ((index >> 3) << 2);
	if (OPiGPIODebug)
		printf("func:%s pin:%d, direction:%d bank:%d index:%d phyaddr:0x%x\n",__func__, gpio , direction,bank,index,phyaddr);

	regval = readl(phyaddr);
	if (OPiGPIODebug)
		printf("read reg val: 0x%x offset:%d\n",regval,offset);

	set_pullupdn(gpio, pud);

	if(INPUT == direction)
	{
		regval &= ~(7 << offset);
		writel(regval, phyaddr);
		regval = readl(phyaddr);
		if (OPiGPIODebug)
			printf("Input mode set over reg val: 0x%x\n",regval);
	}
	else if(OUTPUT == direction)
	{
	   regval &= ~(7 << offset);
	   regval |=  (1 << offset);
	   if (OPiGPIODebug)
			printf("Out mode ready set val: 0x%x\n",regval);
	   writel(regval, phyaddr);
	   regval = readl(phyaddr);
	   if (OPiGPIODebug)
			printf("Out mode set over reg val: 0x%x\n",regval);
	}
	else
	{
		printf("line:%dpin number error\n",__LINE__);
	}
}

void output_gpio(int gpio, int value) //void sunxi_digitalWrite(int pin, int value)
{
	uint32_t regval = 0;
	int bank = gpio >> 5;
	int index = gpio - (bank << 5);
	uint32_t phyaddr = SUNXI_GPIO_BASE + (bank * 36) + 0x10; // +0x10 -> data reg
	if (OPiGPIODebug)
		printf("func:%s pin:%d, value:%d bank:%d index:%d phyaddr:0x%x\n",__func__, gpio , value,bank,index,phyaddr);

	regval = readl(phyaddr);
	if (OPiGPIODebug)
		printf("before write reg val: 0x%x,index:%d\n",regval,index);
	if(0 == value)
	{
		regval &= ~(1 << index);
		writel(regval, phyaddr);
		regval = readl(phyaddr);
		if (OPiGPIODebug)
			printf("LOW val set over reg val: 0x%x\n",regval);
	}
	else
	{
		regval |= (1 << index);
		writel(regval, phyaddr);
		regval = readl(phyaddr);
		if (OPiGPIODebug)
			printf("HIGH val set over reg val: 0x%x\n",regval);
	}
}

int input_gpio(int gpio)//int sunxi_digitalRead(int pin)
{
	uint32_t regval = 0;
	int bank = gpio >> 5;
	int index = gpio - (bank << 5);
	uint32_t phyaddr = SUNXI_GPIO_BASE + (bank * 36) + 0x10; // +0x10 -> data reg
	if (OPiGPIODebug)
		printf("func:%s pin:%d,bank:%d index:%d phyaddr:0x%x\n",__func__, gpio,bank,index,phyaddr);

	regval = readl(phyaddr);
	regval = regval >> index;
	regval &= 1;
	if (OPiGPIODebug)
		printf("***** read reg val: 0x%x,bank:%d,index:%d,line:%d\n",regval,bank,index,__LINE__);

	return regval;
}

void clear_event_detect(int gpio)
{
	return;
}

int eventdetected(int gpio)
{
	return 0;
}

void set_rising_event(int gpio, int enable)
{
  return;
}

void set_falling_event(int gpio, int enable)
{
	return;
}

void set_high_event(int gpio, int enable)
{
	return;
}

void set_low_event(int gpio, int enable)
{
	return;
}

void cleanup(void)
{
    munmap((caddr_t)gpio_map, BLOCK_SIZE);
}
