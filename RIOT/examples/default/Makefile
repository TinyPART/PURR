# name of your application
APPLICATION = default

# If no BOARD is found in the environment, use this default:
BOARD ?= native

# This has to be the absolute path to the RIOT base directory:
RIOTBASE ?= $(CURDIR)/../..

# Uncomment these lines if you want to use platform support from external
# repositories:
#RIOTCPU ?= $(CURDIR)/../../RIOT/thirdparty_cpu
#EXTERNAL_BOARD_DIRS ?= $(CURDIR)/../../RIOT/thirdparty_boards

# Uncomment this to enable scheduler statistics for ps:
#USEMODULE += schedstatistics

# If you want to use native with valgrind, you should recompile native
# with the target all-valgrind instead of all:
# make -B clean all-valgrind

# Comment this out to disable code in RIOT that does safety checking
# which is not needed in a production environment but helps in the
# development process:
DEVELHELP ?= 1

# Change this to 0 show compiler invocation lines by default:
QUIET ?= 1

# Modules to include:
USEMODULE += shell
USEMODULE += shell_cmds_default
USEMODULE += shell_cmds_xipfs
USEMODULE += ps
# include and auto-initialize all available sensors
USEMODULE += saul_default

USEMODULE += auto_init_gnrc_netif
# Specify the mandatory networking modules for IPv6 and UDP
USEMODULE += gnrc_ipv6_default
#USEMODULE += sock_udp
# Additional networking modules that can be dropped if not needed
#USEMODULE += gnrc_icmpv6_echo

# Use the default file system
USEMODULE += vfs_default
USEMODULE += suit suit_transport_coap
USEMODULE += suit_transport_vfs
USEMODULE += suit_storage_vfs

include $(RIOTBASE)/tests/Makefile.boards.netif
ifneq (,$(filter $(BOARD),$(BOARD_PROVIDES_NETIF)))
  # Use modules for networking
  # gnrc is a meta module including all required, basic gnrc networking modules
  USEMODULE += gnrc
  # use the default network interface for the board
  USEMODULE += netdev_default
  # automatically initialize the network interface
  USEMODULE += auto_init_gnrc_netif
  # shell command to send L2 packets with a simple string
  USEMODULE += gnrc_txtsnd
  # the application dumps received packets to stdout
  USEMODULE += gnrc_pktdump
endif

FEATURES_OPTIONAL += periph_rtc

ifneq (,$(filter msba2,$(BOARD)))
  USEMODULE += mci
  USEMODULE += random
endif

include $(RIOTBASE)/Makefile.include

ifneq (,$(filter $(BOARD),$(BOARD_PROVIDES_NETIF)))
  # We use only the lower layers of the GNRC network stack, hence, we can
  # reduce the size of the packet buffer a bit
  # Set GNRC_PKTBUF_SIZE via CFLAGS if not being set via Kconfig.
  ifndef CONFIG_GNRC_PKTBUF_SIZE
    CFLAGS += -DCONFIG_GNRC_PKTBUF_SIZE=512
  endif
endif

# Set a custom channel if needed
include $(RIOTMAKE)/default-radio-settings.inc.mk

all:
	echo "PIP := /Users/user/Demonstrations/pipcore-mpu/pip.elf" > /Users/user/Demonstrations/pip-mpu-separate/Makefile.config
	echo "ROOT := $(shell pwd)/bin/dwm1001_pip/default.elf" >> /Users/user/Demonstrations/pip-mpu-separate/Makefile.config
	make -C /Users/user/Demonstrations/pip-mpu-separate realclean all
	cp /Users/user/Demonstrations/pip-mpu-separate/pip+root.elf bin/dwm1001_pip/pip+root.elf
	cp /Users/user/Demonstrations/pip-mpu-separate/pip+root.bin bin/dwm1001_pip/pip+root.bin

flash:
	openocd -c 'source [find interface/jlink.cfg]' -c 'transport select swd' -f '/Users/user/Demonstrations/RIOT/boards/common/nrf52_pip/dist/openocd.cfg' -c 'tcl_port 0' -c 'telnet_port 0' -c 'gdb_port 0' -c 'init' -c 'targets' -c 'reset halt' -c 'flash write_image erase "/Users/user/Demonstrations/RIOT/examples/default/bin/dwm1001_pip/pip+root.bin" 0 ' -c 'verify_image "/Users/user/Demonstrations/RIOT/examples/default/bin/dwm1001_pip/pip+root.bin" 0' -c 'reset run' -c 'shutdown'
