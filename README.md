##### 1. Including WILC1000 driver source in kernel tree.  
Overwrite onto the linux-at91 kernel directory already we have.  
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
<br><br>
##### 2. Setting basic configuration and options.  
At this step, to include WILC1000 Driver, you have to modify default configuration using the “`menuconfig`” parameter.  

    $ make menuconfig
    
Choose the Atmel SmartConnect menu, “`Device Driver -> Network device support -> Wireless LAN -> Atmel SmartConnect Wireless cards Driver`”. The option WILC1000 driver can be set to either “`M`”or “`*`” as depicted in the below.  
![](https://github.com/atmchrispark/Image/blob/master/kernel_smartconnect.jpg)  
<br>
If Bus type is SPI, The option WILC1000 driver can be set as below.
![](https://github.com/atmchrispark/Image/blob/master/Choose_SPI.jpg)

   * WILC1000 support  : Wi-Fi device (wilc1000)  
   * Bus Type          : Choose either SDIO(Secure Digital Input Output) or SPI(Serial Peripheral Bus).  
   * Memory Allocation : Heap area shortage in the system may affect wifi operate.  
Therefore, according to the resource status user can choose one of the three below.  
      * When bootup system.  
      * When driver loading.  
      * Driver running fluidly.  
   * Use out of band interrupt : If you want to use an external pin interrupt instead of internal SDIO IRQ in case of SDIO bus type. Choose this option.  
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
<br>
If Bus type is SPI, the “`.config`” file in the kernel directory can be simply editable like the below.
```
CONFIG_WILC1000_SPI=y
```  


<br><br>
##### 3. Modify WILC1000 driver as user bus type.

   ▶ CUSTOMER_PLATFORM, WINC_SPI and WILC_SDIO_IRQ_GPIO feature will help you suitably to modify wifi driver in the source.


   * **SDIO**  
      : The SDIO WILC1000 related protocol is implemented in the file `wilc1000/wilc_sdio.c` while the linux related SDIO API is implemented in the file `wilc1000/linux_wlan_sdio.c`  
  
      When wifi drvier is loaded and unload on kernel, if user want to add special commands, write it in linux_sdio_init and linux_sdio_deinit function of file `linux_wlan_sdio.c`  

   * **SPI**  
      1. Registering SPI driver  
         The SPI NMI related protocol is implemented in the file `src/driver/nmi_spi.c` while the linux related SPI API is implemented in the file `src/diver/linux/source/linux_wlan_spi.c`  

         When the module is inserted, the module calls the function `linux_spi_init` which in turn calls the function `spi_register_driver(&wilc_bus);`  
         
         Where the wilc_bus is defined as follow
      ```  
      struct spi_driver wilc_bus __refdata = {  
      		.driver = {  
      		.name = MODALIAS,  
      		},  
      		.probe =  wilc_bus_probe,  
      		.remove = __exit_p(wilc_bus_remove),  
      };  
      ```  
	 The only thing that needs to be changed is the definition of the MODALIAS, it should changed to match the target platform name.  
         This value is defined at the file `src/driver/include/Linux_wlan_common.h`   
  
      2. Setting SPI Clock  
         SPI clock can be changed to improve the communication with host processor during working.  
         So `MIN_SPEED` and `MAX_SPEED` value have to defined depand on the clock which platform support.  
      
         look at it in `linux_wlan_spi.c` file  
      
         ```
            #elif defined(CUSTOMER_PLATFORM)
            /*
               DOTO : define Clock speed under 48M.
            */
            #else
               #define MIN_SPEED 24000000
               #define MAX_SPEED 48000000
            #endif
         ```  
      
   * **Interrupt**  
      
      When WILC1000 has SPI interface, External Pin Interrupt has to be used to transfer the data to host processor.  
      
      When the module initialized it registers a GPIO (in case of SPI or SDIO with external interrupt) to act as an interrupt, the driver uses the definition GPIO_NUM, this definition should be defined in the file `linux_wlan_common.h`  
      
      Check and modify it like below  
      
      - define gpio number (GPIO_NUM) which is physically mapped with WILC1000 in file `linux_wlan_common.h`.
      - save the registerd irq number to the private wlic context in function `init_irq" of file "linux_wlan.c`.  Fix and add  'linux_wlan_enable_irq' and 'linux_wlan_disable_irq' in same file according to user platform.
      
