#!/bin/sh

function copyfile()
{
	file1=$path1/$2
	file2=$path2/$2
	
	needcopyfile=0
	if [ ! -f "$file1" ];then
		needcopyfile=0
	elif [ ! -f "$file2" ];then
		needcopyfile=1	
	elif [ $1 == "1" ];then
		filetime1=`stat -c %Y $file1`
        	filetime2=`stat -c %Y $file2`

        	if [ $filetime1 -gt $filetime2 ];then
                	needcopyfile=1
        	fi
	fi
	
	if [ $needcopyfile == 1 ];then
		echo "copy $file1 $file2"
		cp -f $file1 $file2
	fi
}

binpath=../../_bin/linux
respath=../../../Resource
settingpath=../../_bin/setting
deploypath=/data/logcluster

mkdir -p $deploypath
mkdir -p $deploypath/setting
mkdir -p $deploypath/plugin
mkdir -p $deploypath/config

#setting
path1=$settingpath
path2=$deploypath/setting
copyfile 1 ip.address
copyfile 1 bus.relation
copyfile 1 server.network
copyfile 1 initapp.log4cxx
copyfile 1 templateapp.log4cxx
copyfile 1 common.startup
copyfile 1 cluster.setting

path1=$settingpath/log
path2=$deploypath/setting
copyfile 1 master.startup
copyfile 1 proxy.startup
copyfile 1 shard.startup

#config
path1=$respath/config
path2=$deploypath/config

#plugin
path1=$binpath
path2=$deploypath
copyfile 1 KFStartup
copyfile 1 KFStartupd

cp -f $deploypath/KFStartup $deploypath/logserver
cp -f $deploypath/KFStartupd $deploypath/logserverd

path2=$deploypath/plugin
copyfile 1 KFBus.so
copyfile 1 KFBusd.so
copyfile 1 KFIpAddress.so
copyfile 1 KFIpAddressd.so
copyfile 1 KFHttpClient.so
copyfile 1 KFHttpClientd.so
copyfile 1 KFTcpServer.so
copyfile 1 KFTcpServerd.so
copyfile 1 KFTcpClient.so
copyfile 1 KFTcpClientd.so
copyfile 1 KFMessage.so
copyfile 1 KFMessaged.so
copyfile 1 KFConfig.so
copyfile 1 KFConfigd.so
copyfile 1 KFClusterServer.so
copyfile 1 KFClusterServerd.so
copyfile 1 KFClusterProxy.so
copyfile 1 KFClusterProxyd.so
copyfile 1 KFTimer.so
copyfile 1 KFTimerd.so
copyfile 1 KFClusterShard.so
copyfile 1 KFClusterShardd.so
copyfile 1 KFWorker.so
copyfile 1 KFWorkerd.so
copyfile 1 KFLogProxy.so
copyfile 1 KFLogProxyd.so
copyfile 1 KFLogShard.so
copyfile 1 KFLogShardd.so
copyfile 1 KFRedis.so
copyfile 1 KFRedisd.so
copyfile 1 KFOptiond.so
copyfile 1 KFOption.so
copyfile 1 KFScheduled.so
copyfile 1 KFSchedule.so
copyfile 1 KFDeployClient.so
copyfile 1 KFDeployClientd.so
copyfile 1 KFDeployCommand.so
copyfile 1 KFDeployCommandd.so