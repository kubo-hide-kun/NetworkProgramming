#include <stdio.h>              /* printf(),fprintf()に必要 */
#include <stdlib.h>             /* perror(),atoi()に必要 */
#include <sys/socket.h>         /* socket(),connect()に必要 */
#include <arpa/inet.h>          /* sockaddr_in,inet_addr()に必要 */
#include <string.h>             /* strlen()に必要 */
#include <unistd.h>             /* close()に必要 */

#define BUF_SIZE 256            /* マジックナンバーで256の定義 */
#define MONEY_DIGIT_SIZE 10     /* お金の最大桁数 */

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

void DieWithError(char *);
int prepare_client_socket(char*, int);
void commun(int);
void my_scanf(char*, int);
void read_nutil_delim(int, char*, char, int);


int main(int argc, char **argv){                                    /* 第一引数: コマンドライン引数の数, 第二引数: コマンドライン引数を格納した配列 */
    if(argc != 3)DieWithError("arguments is not available");        /* 実行時引数の個数が正常であることを確認する */
    char *server_idaddr = argv[1];                                  /* サーバーのIPアドレスを実行時引数から取得 */
    int server_port = atoi(argv[2]);                                /* サーバーのポート番号を実行時引数から取得 */

    int sock = prepare_client_socket(server_idaddr,server_port);
    
    commun(sock);                                                   /* 関数内の処理でサーバーと各種通信を行う */
    close(sock);                                                    /* サーバーとの接続をクローズする */
    return 0;
}

void DieWithError(char *errorMessage){
    perror(errorMessage);                                           /* 標準エラー出力にエラーメッセージを返す */
    exit(1);                                                        /* 引数は「終了ステータス」→ エラーの時は１以上を返す */
}

int prepare_client_socket(char* idaddr,int port){
    int sock = socket(PF_INET,SOCK_STREAM,0);                       /* TCPソケットを作成する */
    if(sock<0)DieWithError("socket()failed");                       /* 生成時エラーの判定(0以上:成功, -1:エラー) */

    struct sockaddr_in target;                                      /* サーバーのアドレス */
    target.sin_family = AF_INET;                                    /* インターネットアドレスファミリ */
    target.sin_addr.s_addr = inet_addr(idaddr);                     /* サーバーのIPアドレス */
    target.sin_port = htons(port);                                  /* サーバーのポート番号 */

    if(connect(sock,(struct sockaddr *)&target,sizeof(target))<0)   /* サーバーへの接続を確立する */
        DieWithError("connect()failed");                            /* 接続時エラーの判定 */

    return sock;
}

void commun(int sock){
    char cmd[2] = "";                                               /* コマンド入力用 */
    char withdraw[MONEY_DIGIT_SIZE+1];                              /* 引き出し額 */
    char deposit[MONEY_DIGIT_SIZE+1];                               /* 預け入れ額 */
    char msg[BUF_SIZE];                                             /* 送信メッセージ */
    my_scanf(cmd, 1);
    switch (cmd[0]){
        case '0':                                                   /* 引き出し処理 */
            my_scanf(withdraw,MONEY_DIGIT_SIZE);
            sprintf(msg, "0_%s_", withdraw);
            break;
        case '1':                                                   /* 預け入れ処理 */
            my_scanf(deposit,MONEY_DIGIT_SIZE);
            sprintf(msg, "%s_0_", withdraw);
            break;
        default:
            return;
    }

    if(send(sock, msg, strlen(msg), 0) != strlen(msg))                  /* 送信処理 */
        DieWithError("send() sent a message of unexpected bytes");

    read_until_delim(sock, msg, '_', BUF_SIZE);                         /* 受信処理 */
    printf("残高は%d円になりました", atoi(msg));                           /* 表示処理 */
}

void my_scanf(char *buf,int num_letter) {
    // num_letter文字だけ入力させる (scanfの脆弱性対策)
    char format[20];
    sprintf(format, "%s%d%s", "%", num_letter, "s%[^\n]");
    scanf(format, buf);
    getchar();
}

void read_nutil_delim(int sock, char *buf, char delimiter, int max_length) {
    int len_r = 0;                                                                      /* 受信文字列 */
    int index_letter = 0;                                                               /* 受信文字列の合計 */
    while(index_letter < max_length -1) {
        if ((len_r = recv(sock, buf + index_letter, 1, 0)) <= 0) {                      /* エラー */
			DieWithError("recv() failed");
        }
        if(buf[index_letter] == delimiter) break;
        else index_letter++;
    }
    buf[index_letter] = '\0';                                                           /* nullを末尾に追加 */
}