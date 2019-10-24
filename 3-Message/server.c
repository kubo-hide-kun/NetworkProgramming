#include <stdio.h>              /* printf(),fprintf()に必要 */
#include <stdlib.h>             /* perror(),atoi()に必要 */
#include <sys/socket.h>         /* socket(),bind(),accept()に必要 */
#include <arpa/inet.h>          /* sockaddr_in,inet_addr()に必要 */
#include <string.h>             /* strlen()に必要 */
#include <unistd.h>             /* close()に必要 */

#define BUF_SIZE 256            /* マジックナンバーで256の定義 */
#define MONYE_DIGIT_SIZE 10     /* 金額の最大桁数 */

/* プロトタイプ宣言開始(関数の場所を任意に設定可能)) */
void DiewithError(char *);
int prepare_server_socket(int);
void commun(int);
void read_nutil_delim(int, char*, char, int);
int get_current_balance();
void set_current_balance(int);
/* プロトタイプ宣言終了 */

int main(int argc, char **argv) {                                    /* 待ち受け用ソケットの作成(PF_INET=IPv4,SOCKET_STREAM=TCP,0=残りはお任せ) */

    int cliSock;                                                                        /* 通信用のソケット */
    struct sockaddr_in clientAddress;                                                   /* クライアントの「IPアドレス&ポート番号」のふたつを格納 */
    unsigned int szClientAddr;                                                          /* クライアントの「IPアドレス+ポート番号」のサイズ */

    int servSock = prepare_server_socket(10001);

    listen(servSock,5);                                                                 /* 順番待ち(第二引数が順番待ちしても良いクライアント数) */
    while(1){
        szClientAddr = sizeof(clientAddress);                                           /* 受信データの形式のメモリサイズを取得 */
        cliSock = accept(servSock,(struct sockaddr*)&clientAddress,&szClientAddr);      /*  */
        commun(cliSock);                                                                /* ユーザー定義 */
    }
    close(servSock);                                                                    /* serverをクローズする */
    return 0;
}

void DieWithError(char *errorMessage){
    perror(errorMessage);                                                               /* 標準エラー出力にエラーメッセージを返す */
    exit(1);                                                                            /* 引数は「終了ステータス」→ エラーの時は１以上を返す */
}


int prepare_server_socket(int port){
    int servSock = socket(PF_INET,SOCK_STREAM,0);   
    if(servSock<0)DieWithError("socket() failed");

    struct sockaddr_in servAddress;                                                     /* サーバーの情報を格納する構造体 */
    servAddress.sin_family = AF_INET;                                                   /* インターネットアドレスファミリ */
    servAddress.sin_addr.s_addr = htonl(INADDR_ANY);                                    /* サーバーのIPアドレス(ANYは任意の場所が可能であることを示す) */
    servAddress.sin_port = htons(port);                                                /* 受け取るポート番号 */

    bind(servSock,(struct sockaddr *)&servAddress,sizeof(servAddress));                 /* サーバーの設定をservSockに結びつける */
    
    return servSock;
}

void commun(int sock){
    char buf[BUF_SIZE];                                                                 /* エコー文字列用のバッファ */
    int len_r;                                                                          /* 受信文字数 */

    if((len_r=recv(sock,buf,BUF_SIZE,0))<=0)                                            /* 受信データをバッファに格納 */
        DieWithError("recv()failed");                                                   /* 受信時エラー(文字量違反) */
    
    buf[len_r] = '\0';                                                                  /* 文末EOSの追加 */
    printf("%s\n",buf);                                                                 /* 受信データを出力 */
    if((send(sock,buf,strlen(buf),0))!=strlen(buf))                                     /* クライアントに受け取ったデータを返却 */
        DieWithError("send()sent a message of unexpected bytes");                       /* 送信時エラー(データの不一致) */
}

void read_nutil_delim(int sock, char *buf, char delimiter, int max_length) {
    int len_r = 0;                                                                      /* 受信文字列 */
    int index_letter = 0;                                                               /* 受信文字列の合計 */
    while(index_letter < max_length -1) {
        if ((len_r = recv(sock, buf + index_letter, 1, 0)) <= 0) {                      /* エラー */
            printf("接続が切れました");
            return ;
        }
        if(buf[index_letter] == delimiter)
            break;
        else
            index_letter++;
    }
    buf[index_letter] = '\0';                                                           /* nullを末尾に追加 */
}

// 本来はデータベースから現在の預金残高を取得
int get_current_balance(){
	return 1000000;
}

// 本来は預金残高をデータベースに登録
void set_current_balance(int new_balance) {
	return;
}