# 手写笔记的精炼
## 首先对于这个代码思路
先用<pcap.h>内一系列函数，捕获数据包。然后通过回调函数分析捕获数据包长度，源MAC，目的MAC等信息。之后若以太网帧头部结构中的ether_type==ETHERTYPE_IP，则为IPv4协议。再分析IPv4头部，输出源IP，目的IP等信息。再通过IPv4头部结构体的protocol得到协议号，判断是哪个协议，ICMP，TCP，UDP亦或是其他协议。

## 其次对于代码中的结构体和函数
这些结构体和函数都是在我做这个项目之前几乎没有接触过的，pcap_findalldevs(),pcap_open_live(),pcap_loop(),pcap_close(),pcap_freealldevs()这一系列函数，把他们的传参搞懂后，就没太大问题了。pcap_t,pcap_if_t,struct ether_header,struct iphdr,struct tcphdr,struct udphdr,struct pcap_pkthdr这一些结构体，把他们所代表的东西，以及他们的内部结构搞懂后就可以了。

## 最后是对于一些要注意的点
在这个函数中，绝大部分的结构体定义都是用的指针形式定义的，因为要避免拷贝数据，方便做内存偏移和缓冲区const保护等原因。还有ntohs()这个16位数据的网络字节序转主机字节序的函数，也是这个代码精髓。还有pcap_close()和pcap_freealldevs()这两个函数的释放顺序一定不可以搞反了，一定要先停止网卡捕获，释放抓包内存，关闭捕获句柄之后才可以释放网卡链表占用的内存，也就是pcap_close()要写在pcap_freealldevs()前面。
