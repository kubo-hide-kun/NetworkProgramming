#include <stdio.h>              /* printf(),fprintf()に必要 */
#include <stdlib.h>             /* perror(),atoi()に必要 */
#include <sys/socket.h>         /* socket(),bind(),accept()に必要 */
#include <arpa/inet.h>          /* sockaddr_in,inet_addr()に必要 */
#include <string.h>             /* strlen()に必要 */
#include <unistd.h>             /* close()に必要 */
#define BUF_SIZE 256            /* マジックナンバーで256の定義 */

void DieWithError(char *errorMessage){
    perror(errorMessage);                                                               /* 標準エラー出力にエラーメッセージを返す */
    exit(1);                                                                            /* 引数は「終了ステータス」→ エラーの時は１以上を返す */
}

void commun(int sock){
    char buf[BUF_SIZE];                                                                 /* エコー文字列用のバッファ */
    int len_r;                                                                          /* 受信文字数 */
    char *message = "Accept!";

    if((len_r=recv(sock,buf,BUF_SIZE,0))<=0)                                            /* 受信データをバッファに格納 */
        DieWithError("recv()failed");                                                   /* 受信時エラー(文字量違反) */
    
    buf[len_r] = '\0';                                                                  /* 文末EOSの追加 */
    printf("%s\n",buf);                                                                 /* 受信データを出力 */
    if(send(sock, message, strlen(message), 0) != strlen(message))                                     /* クライアントに受け取ったデータを返却 */
        DieWithError("send()sent a message of unexpected bytes");                       /* 送信時エラー(データの不一致) */
}

int main(int argc, char **argv) {
    printf("start!\n");
    int servSock = socket(PF_INET,SOCK_STREAM,0);                                       /* 待ち受け用ソケットの作成(PF_INET=IPv4,SOCKET_STREAM=TCP,0=残りはお任せ) */

    int cliSock;                                                                        /* 通信用のソケット */
    struct sockaddr_in clientAddress;                                                   /* クライアントの「IPアドレス&ポート番号」のふたつを格納 */
    unsigned int szClientAddr;                                                          /* クライアントの「IPアドレス+ポート番号」のサイズ */

    struct sockaddr_in servAddress;                                                     /* サーバーの情報を格納する構造体 */
    servAddress.sin_family = PF_INET;                                                   /* インターネットアドレスファミリ */
    servAddress.sin_port = (in_port_t)htons(81);                                        /* 受け取るポート番号 */
    servAddress.sin_addr.s_addr = htonl(INADDR_ANY);                                    /* サーバーのIPアドレス(ANYは任意の場所が可能であることを示す) */

    bind(servSock,(struct sockaddr *)&servAddress,sizeof(servAddress));                 /* サーバーの設定をservSockに結びつける */
    listen(servSock,5);                                                                 /* 順番待ち(第二引数が順番待ちしても良いクライアント数) */
    while(1){
        szClientAddr = sizeof(clientAddress);                                           /* 受信データの形式のメモリサイズを取得 */
        cliSock = accept(servSock,(struct sockaddr*)&clientAddress,&szClientAddr);      /*  */
        printf("commun!\n");
        commun(cliSock);                                                                /* ユーザー定義 */
        close(cliSock);                                                                /* serverをクローズする */
    }
    close(servSock);                                                                    /* serverをクローズする */
    return 0;
}