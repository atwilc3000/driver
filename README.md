##### 1. Including WILC1000 driver source in kernel tree.  
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

##### 2. Setting basic configuration and options.  
At this step, to include WILC1000 Driver, you have to modify default configuration using the “`menuconfig`” parameter.  

    $ make menuconfig
    
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

##### 3. Modify WILC1000 driver as user target system.  
   * Related files
      - Linux_wlan_sdio.c  
         : The SDIO WILC1000 related protocol is implemented in the file `wilc1000/wilc_sdio.c` while the linux related SDIO API is implemented in the file `wilc1000/linux_wlan_sdio.c`  

           Just as the SPI, only the definition MODALIAS defined in the file `wilc1000/linux_wlan_common.h` need to be changed to match the target platform name.  

      - Linux_wlan_spi.c  
         : The SPI WILC1000 related protocol is implemented in the file `wilc1000/wilc_spi.c` while the linux related SPI API is implemented in the file `wilc1000/linux_wlan_spi.c`  

           When the module is inserted, the module calls the function `linux_spi_init` which in turn calls the function `spi_register_driver(&wilc_bus);`  

           Where the wilc_bus is defined as follow
      ```  
      struct spi_driver wilc_bus __refdata = {  
      		.driver = {  
      		.name = MODALIAS,  
      		},  
      		.probe =  wilc_bus_probe,  
      		.remove = __devexit_p(wilc_bus_remove),  
      };  
      ```  
	  The only thing that needs to be changed is the definition of the MODALIAS, it should changed to match the target platform name.  
          This value is defined at the file `wilc1000/linux_wlan_common.h`  

      - Linux_wlan.c  
      : When the module initialized it registers a GPIO (in case of SPI or SDIO with external interrupt) to act as an interrupt, the driver uses the definition GPIO_NUM, this definition should be defined in the file `wilc1000/linux_wlan_common.h`  

   > You need to add the information that matches the Define value of the Makefile.
