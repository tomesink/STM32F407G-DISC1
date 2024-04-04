Following [this](https://github.com/cpq/bare-metal-programming-guide) baremetal STM32 programming guide adjusted for [STM32F407G-DISC1](https://www.st.com/en/evaluation-tools/stm32f4discovery.html) board.

The projects depends on CMSIS libraries which need to be cloned or added into Makefile like this:

```Makefile
cmsis_core: 
 	  git clone --depth 1 -b 5.9.0 https://github.com/ARM-software/CMSIS_5 $@ 

cmsis_f4: 
 	git clone --depth 1 -b v2.6.8 https://github.com/STMicroelectronics/cmsis_device_f4 $@
```
