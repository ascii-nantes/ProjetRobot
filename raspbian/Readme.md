# Presentation
This part of the project is destined to be run on the Raspberry Pi on the robot. For testing purpose, all code should be tested on an environement as close as possible to a raspberry pi (or on it if possible).

To emulate this environement, I suggest using a virtual machine with a Debian wheezy. The choice of the distribution has been made in accord with the Raspberry Pi FAQ [What is Raspbian ?](http://www.raspbian.org/RaspbianFAQ#What_is_Raspbian.3F)

I (Leonard) personnally use [qemu](http://wiki.qemu.org/Main_Page) with an official [Debian image for qemu](https://people.debian.org/~aurel32/qemu/amd64/debian_wheezy_amd64_standard.qcow2)

## Running ProjetRobot in a VM

## First run

### Install qemu
 - Install qemu-headless
 ```
 host$ yaourt -Ss qemu-headless
 ```
### Setup network
 - (If not done) Install vde2
 ```
 host$ yaourt -Ss vde2
 ```
 - Modprobe tun
 ```
 host# modprobe tun
 ```
 - Add a vde interface
 ```
 host# vde_switch -tap tap0 -daemon -mod 660 -group users
 ```
 - Configure tap0
 ```
 host# ip addr add 192.168.100.254/24 dev tap0
 ```
#### Bridge through ethernet (This section need testing)
 - (If not done) Install bridge-utils
 ```
 host$ yaourt -Ss bridge-utils
 ```
 - Create a bridge
 ```
 host# sudo brctl addbr br0
 ```
 - Add tap0 and your ethernet port
 ```
 host# sudo brctl addif eth0
 host# brctl addif tap0
 ```
 - Auto configure your bridge
 ```
 host# dhcpcd bro0
 ```
### Wifi through nat
 - Set up your iptables
 ```
 host# iptables -A POSTROUTING -o wlo1 -j MASQUERADE
 host# iptables -A FORWARD -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT
 host# iptables -A FORWARD -i tap0 -o wlo1 -j ACCEPT
 host# iptables -I INPUT -p all -i tap0 -j ACCEPT
 host# ipatbles -I INPUT -p all -s 192.168.100.1/24 -j ACCEPT
 ```
 - Allow ip forwarding (Only for the current session)
 ```
 host# sudo sysctl net.ipv4.ip_forward=1
 ```

### Run the VM
 - Start Qemu
 ```
 host$ qemu-system-x86_64 -net nic -net vde -hda debian_wheezy_amd64_standard.qcow2 -curses -m 1024
 ```
 - Login (default root:root)
 - Configure internet in your VM
 ```
 vm# ip addr add dev eth0 192.168.100.1/24
 vm# ip route add default via 192.168.100.254 dev eth0
 ```
 - Change the nameserver in `/etc/resolve.conf` to `8.8.8.8`

### Verify installation
 - Update the system
 ```
 vm# apt-get update
 vm# apt-get upgrade
 ```
 - Install java and git
 ```
 vm# apt-get install default-java
 ```
 - Go make yourself a coffee
 ```
 vm# apt-get install git
 ```
 - Drink your coffee

## Utility script
 - compile.sh: Simple script which compile the project into a target folder
 - run.sh: Simple script which run the compiled project
