#include <stdio.h>              /* printf(),fprintf()に必要 */
#include <stdlib.h>             /* perror(),atoi()に必要 */
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
void DieWithError(char *errorMessage){
    perror(errorMessage);                                           /* 標準エラー出力にエラーメッセージを返す */
    exit(1);                                                        /* 引数は「終了ステータス」→ エラーの時は１以上を返す */
}

void commun(int sock){
    char buf[BUF_SIZE];                                              /* エコー文字列用のバッファ */
    char *message = "404";                                          /* 送信するメッセージ　*/

    if(send(sock,message,strlen(message),0)!=strlen(message))       /* サーバーにメッセージの送信 */
        DieWithError("Send() sent a message of unexpected");        /* 送信時エラーの判定 */

    if((recv(sock,buf,BUF_SIZE,0))<=0)                               /* 受信データをバッファに格納 */
        DieWithError("recv()failed");                               /* 受信時エラー */

    printf("%s\n",buf);                                             /* 受信データを出力 */
}

int main(int argc, char **argv){
    if(argc != 3)DieWithError("arguments is not available");        /* 実行時引数の個数が正常であることを確認する */
    char *server_idaddr = argv[1];                                  /* サーバーのIPアドレスを実行時引数から取得 */
    int server_port = atoi(argv[2]);                                /* サーバーのポート番号を実行時引数から取得 */

    int sock = socket(PF_INET,SOCK_STREAM,0);                       /* TCPソケットを作成する */
    if(sock<0)DieWithError("socket()failed");                       /* 生成時エラーの判定(0以上:成功, -1:エラー) */

    struct sockaddr_in target;                                      /* サーバーのアドレス */
    target.sin_family = AF_INET;                                    /* インターネットアドレスファミリ */
    target.sin_addr.s_addr = inet_addr(server_idaddr);              /* サーバーのIPアドレス */
    target.sin_port = htons(server_port);                           /* サーバーのポート番号 */

    if(connect(sock,(struct sockaddr *)&target,sizeof(target))<0)   /* サーバーへの接続を確立する */
        DieWithError("connect()failed");                            /* 接続時エラーの判定 */
    
    commun(sock);                                                   /* 関数内の処理でサーバーと各種通信を行う */
    close(sock);                                                    /* サーバーとの接続をクローズする */
    return 0;
}