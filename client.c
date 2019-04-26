#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

/**
struct in_addr{
    unsigned long s_addr;       //IPアドレス
};

struct sockaddr_in {
    unsigned short sin_family;  //アドレスファミリ (TCP or IP)
    unsigned short sin_port;    //アドレスポート (ポート番号)
    struct in_addr sin_addr;    //IPアドレス
    char sin_zero[8];           // 不使用領域
};
**/

void commun(int sock){
    char buf[256];                                              /* エコー文字列用のバッファ */
    char *message = "404";                                      /* 送信するメッセージ　*/

    send(sock,message,strlen(message),0);                       /* サーバーにメッセージの送信 */
    recv(sock,buf,256,0);                                       /* 受信データをバッファに格納 */

    printf("%s\n",buf);                                         /* 受信データを出力 */
}

int main(int argc, char **argv){
    int sock = socket(PF_INET,SOCK_STREAM,0);                   /* TCPソケットを作成する */

    struct sockaddr_in target;                                  /* サーバーのアドレス */
    target.sin_family = AF_INET;                                /* インターネットアドレスファミリ */
    target.sin_addr.s_addr = inet_addr("10.13.64.20");          /* サーバーのIPアドレス */
    target.sin_port = htons(10001);                             /* サーバーのポート番号 */

    connect(sock,(struct sockaddr *)&target,sizeof(target));    /* サーバーへの接続を確立する */
    commun(sock);                                               /* 関数内の処理でサーバーと各種通信を行う */
    close(sock);                                                /* サーバーとの接続をクローズする */
    return 0;
}