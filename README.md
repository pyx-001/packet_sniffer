## 基于libpcap的数据包嗅探器
使用C语言开发，基于libpcap库实现的网络抓包工具，运行在WSL2 Ubuntu。

## 功能
捕获网卡数据包，解析以太网帧、IP头、TCP/UDP/ICMP头部，输出：
1.数据包长度
2.源MAC、目的MAC
3.源IP、目的IP
4.协议类型
5.TCP/UDP源端口、目的端口

## 环境依赖
Ubuntu(WSL2)，libpcap

## 编译命令
gcc sniffer.c -o sniffer -lpcap

## 运行
sudo ./sniffer

## 原理说明
1. libpcap库打开网卡设备，注册回调函数，收到包触发回调；
2. 手动解析各层协议头部；
3. 网络字节序（大端）转本机字节序（小端）。
