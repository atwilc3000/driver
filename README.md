#### Patching WILC1000 Driver source  
Overwrite onto the linux-at91 kernel directory already we have.  
> * Use diff-and-merge technic to see what will be changed after all.  
> * In the Linux Wireless driver directory, “`/driver/net/wireless`” directory,  
>          WILC1000 Linux driver are newly added as “`atmel`” directory. 

![](https://github.com/atmchrispark/Image/blob/master/driver_image.jpg)  

To include the WILC1000 driver in kernel build, relevant Kconfig and Makefile are also changed.  
   * linux-at91/drivers/wireless/Kconfig  
```
config AT76C50X_USB  
... skip  
source "drivers/net/wireless/atmel/Kconfig"  <-- Add this line  
```

   * linux-at91/drivers/net/wireless/Makefile  
```
obj-$(CONFIG_AT76C50X_USB)      += at76c50x-usb.o  
obj-$(CONFIG_ATMEL_SMARTCONNECT) += atmel/  <-- Add this line  
obj-$(CONFIG_PRISM54)		+= prism54/  
```

   * linux-at91/arch/arm/configs/sama5d3_xplained_defconfig  

##### Kernel Configuration
Because user have to configure the Linux kernel according to hardware, input the following command.  

    $ make ARCH=arm sama5d3_xplained_defconfig  
    
At this step, to include WILC1000 Driver, you have to modify default configuration using the “`menuconfig`” parameter.  

    $ make ARCH=arm menuconfig
    
Choose the Atmel SmartConnect menu, “`Device Driver -> Network device support -> Wireless LAN -> Atmel SmartConnect Wireless cards Driver`”. The option WILC1000 driver can be set to either “`M`”or “`*`” as depicted in the below.  
![](https://github.com/atmchrispark/Image/blob/master/kernel_smartconnect.jpg)  

   * WILC1000 support  : Wi-Fi device (wilc1000)  
   * Bus Type          : Can choose one of the two Interface SDIO SPI.  
   * Memory Allocation : Heap area shortage in the system may affect wifi operate.  
Therefore, according to the resource status user can choose one of the three below.  
      * When bootup system.
      * When driver loading.
      * Driver running fluidly.
   * Use out of band interrupt : If you would want to use an external interrupt instead of SDIO interrupt. Can select this option.  
   * Export defugfs for WILC1000 status : If you would want to know WILC1000 status.  

  
Alternatively, the “`.config`” file in the kernel directory can be simply editable like the below.  
```
CONFIG_WIRELESS=y  
CONFIG_CFG80211=y  
...  
CONFIG_ATMEL_SMARTCONNECT=y  
CONFIG_WILC1000=m  
CONFIG_WILC1000_SDIO=y  
CONFIG_WILC1000_PREALLOCATE_DURING_SYSTEM_BOOT=y  
```
