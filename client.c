#include <stdio.h>              /* printf(),fprintf()に必要 */
#include <sys/socket.h>         /* socket(),connect()に必要 */
#include <arpa/inet.h>          /* sockaddr_in,inet_addr()に必要 */
#include <string.h>             /* strlen()に必要 */
#include <unistd.h>             /* close()に必要 */
#define BUF_SIZE 256            /* マジックナンバーで256の定義 */

/**
struct in_addr{
    unsigned long s_addr;       //IPアドレス
};

struct sockaddr_in {
    unsigned short sin_family;  // アドレスファミリ (TCP or IP)
    unsigned short sin_port;    // アドレスポート (ポート番号)
    struct in_addr sin_addr;    // IPアドレス
    char sin_zero[8];           // 不使用領域
};
**/

void commun(int sock){
    char buf[BUF_SIZE];                                         /* エコー文字列用のバッファ */
    char *message = "404";                                      /* 送信するメッセージ　*/

    send(sock,message,strlen(message),0);                       /* サーバーにメッセージの送信 */
    recv(sock,buf,BUF_SIZE,0);                                  /* 受信データをバッファに格納 */

    printf("%s\n",buf);                                         /* 受信データを出力 */
}

int main(int argc, char **argv){
    char *server_idaddr = "10.13.64.20";                        /* マジックナンバーの定義 */
    int server_port = 10001;                                    /* マジックナンバーの定義 */
    int sock = socket(PF_INET,SOCK_STREAM,0);                   /* TCPソケットを作成する */

    struct sockaddr_in target;                                  /* サーバーのアドレス */
    target.sin_family = AF_INET;                                /* インターネットアドレスファミリ */
    target.sin_addr.s_addr = inet_addr(server_idaddr);          /* サーバーのIPアドレス */
    target.sin_port = htons(server_port);                             /* サーバーのポート番号 */

    connect(sock,(struct sockaddr *)&target,sizeof(target));    /* サーバーへの接続を確立する */
    commun(sock);                                               /* 関数内の処理でサーバーと各種通信を行う */
    close(sock);                                                /* サーバーとの接続をクローズする */
    return 0;
}