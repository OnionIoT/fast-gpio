#!/bin/sh
## define the remote server and package
server="rajiv@build.onion.io"
remotePath="/home/rajiv/OpenWRT-Buildroot/openwrt/dl"
package="fast-gpio"
localPath="/cygdrive/c/Users/Rajiv/Desktop/onion/fast-gpio"
## upload project to remote server
cmd="rsync -va --progress $localPath $server:$remotePath"
echo "rsync command: $cmd"
eval "$cmd"
## create a tar from the file, run the compile
cmd="ssh $server \"cd $remotePath && tar -zcvf $package.tar.gz $package && cd .. && make package/feeds/onion/$package/compile V=99\""
echo "$cmd"
eval "$cmd"

ssh rajiv@build.onion.io "cd /home/rajiv/OpenWRT-Buildroot/openwrt/dl && tar -zcvf fast-gpio.tar.gz fast-gpio/ && cd .. && make package/feeds/onion/fast-gpio/compile V ==99"