#include <stdio.h>
#include <pcap.h>
#include <arpa/inet.h>
#include <netinet/ether.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

void packet_handler(u_char *arg, const struct pcap_pkthdr *header, const u_char *packet)
{
    struct ether_header *eth_header;
    struct iphdr *ip_header;

    eth_header = (struct ether_header *)packet;
    printf("====================================\n");
    printf("捕获数据包长度: %d 字节\n", header->len);

    if(ntohs(eth_header->ether_type) == ETHERTYPE_IP)
    {
        ip_header = (struct iphdr *)(packet + 14);
        printf("源IP: %s\n", inet_ntoa(*(struct in_addr *)&ip_header->saddr));
        printf("目的IP: %s\n", inet_ntoa(*(struct in_addr *)&ip_header->daddr));

        unsigned int ip_header_len = ip_header->ihl * 4;
        const u_char *transport = packet + 14 + ip_header_len;

        switch(ip_header->protocol)
        {
            case 1:
                printf("协议: ICMP\n");
                break;
            case 6:
            {
                struct tcphdr *tcp = (struct tcphdr *)transport;
                printf("协议: TCP | 源端口:%d  目的端口:%d\n", ntohs(tcp->source), ntohs(tcp->dest));
                break;
            }
            case 17:
            {
                struct udphdr *udp = (struct udphdr *)transport;
                printf("协议: UDP | 源端口:%d  目的端口:%d\n", ntohs(udp->source), ntohs(udp->dest));
                break;
            }
            default:
                printf("协议: 其他 %d\n", ip_header->protocol);
        }
    }
}

int main()
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;
    pcap_if_t *alldevs, *dev;

    if (pcap_findalldevs(&alldevs, errbuf) == -1)
    {
        printf("获取网卡列表失败: %s\n", errbuf);
        return -1;
    }
    dev = alldevs;
    printf("使用网卡设备: %s\n", dev->name);

    handle = pcap_open_live(dev->name, BUFSIZ, 1, 1000, errbuf);
    if(handle == NULL)
    {
        printf("打开网卡失败: %s\n", errbuf);
        pcap_freealldevs(alldevs);
        return -1;
    }

    printf("开始抓包，按 Ctrl+C 停止\n");
    pcap_loop(handle, 0, packet_handler, NULL);

    pcap_close(handle);
    pcap_freealldevs(alldevs);
    return 0;
}

