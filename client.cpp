#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <pthread.h>

using namespace std;

void * sendwhilelis(void * arg)
{
    char buf [1000];
    int* fd;
    fd=(int *) arg;
    int sockfd=*fd;
    
    string s;
    bzero(buf,1024);
    while(cin>>s)
    {
        
        strcpy(buf,s.c_str());
        
        
        write(sockfd,buf,strlen(buf));
        //cout<<buf;
        bzero(buf,1024);
        if(s=="Exit")
            break;
    }
    return 0;
}

void * Listen(void * arg)
{
    struct sockaddr_in server_addr,clien_addr;
    int listenfd,connectfd;
    socklen_t length;
    char test[1024];
    bzero(test,1023);
    int* portno;
    portno= (int*)arg;
    int pt=*portno;
    
    listenfd=socket(AF_INET,SOCK_STREAM,0);
    bzero((char*)&server_addr, sizeof(server_addr));
    //int portno;
    //cin>>portno;
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=INADDR_ANY;
    server_addr.sin_port=htons(pt+1);
    //cout<<pt;
    //bind(listenfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    //int r;
    if(::bind(listenfd, (struct sockaddr *)&server_addr, sizeof(server_addr))==0)
    {
        cout<<"Bind complete!"<<endl;
        
    }
    
    listen(listenfd,5);
    //cout<<"Listening"<<endl;
    length=sizeof (clien_addr);
    
    
    if((connectfd=accept(listenfd,(struct sockaddr *)&clien_addr,&length)))
    {
        //cout<<"fuck";
        if(connectfd==0)
        {
            cout<<"good"<<endl;
            
        }
        //cout<<connectfd<<endl;
        //bool loop=false;
        bzero(test,1024);
        pthread_t thread;
        pthread_create(&thread,NULL,&sendwhilelis,&connectfd);
        while(recv(connectfd,test,1024,0))
        {
            string resultt;
            resultt.assign(test);
            cout<<"Receive: "<<resultt<<endl;
            
            if (resultt=="Exit")
            {
                break;
            }
            bzero(test,1024);
            //cout<<"dd";
            
        }
        pthread_
    }
    //cout<<"dd";
    return 0;
}

void * liswhilesend(void * arg)
{
    char buf [1024];
    int* fd;
    fd=(int *)arg;
    int sockfd=*fd;
    while(recv(sockfd,buf,1024,0))
    {
        string resultt;
        resultt.assign(buf);
        cout<<"Receive: "<<resultt<<endl;
        
        if (resultt=="Bye")
        {
            break;
        }
        bzero(buf,1024);
        //cout<<"dd";
    }
    return 0;
}


void chatwithserv(int sockfd,string s,char * buffer)
{
    strcpy(buffer,s.c_str());
    write(sockfd,buffer,strlen(buffer));
    
    /* Receive message from the server and print to screen */
    bzero(buffer, 1024);
    read(sockfd,buffer,1023);
    
    string result;
    result.assign(buffer);
    
    cout<<result<<endl<<endl;
    
    if(result=="Bye\r\n")  //if receive Bye then end the connection
    {
        //break;
    }
    
};

void sendmsg(int sockfd,string s,char * buffer)
{
    strcpy(buffer,s.c_str());
    
    
    write(sockfd,buffer,strlen(buffer));
    //cout<<buffer;
    bzero(buffer,1024);
    
    /* Receive message from the server and print to screen */
    //bzero(buffer, 1024);
    //read(sockfd,buffer,1023);
    
    //string result;
    //result.assign(buffer);
    
    //cout<<result<<endl<<endl;
    
    //cout<< "afdsadfkjajdksfl;ads;f";
};

int getlocalhost(int sockfd,sockaddr_in serv)
{
    int local_port;
    socklen_t addrlen = sizeof(serv);
    if(getsockname(sockfd, (struct sockaddr *)&serv, &addrlen) == 0 &&serv.sin_family == AF_INET &&addrlen == sizeof(serv))
    {
        local_port = ntohs(serv.sin_port);
    }
    //cout<<local_port<<"here"<<endl;
    return local_port;
    
}


void connecting(int port)
{
    int sockfd;
    struct sockaddr_in con;
    struct hostent *conn;
    char sendbuf [1024];
    char readbuf [1024];
    //int * portno;
    //portno=(int *)arg;
    //int port=*portno;
    
    
    sockfd= socket(AF_INET,SOCK_STREAM,0);
    bzero(&con,sizeof(con));
    
    conn=gethostbyname("localhost");
    if(conn==NULL){
        cout<<"Fail"<<endl;
    }
    con.sin_family=AF_INET;
    bcopy((char *)conn->h_addr,(char *)&con.sin_addr.s_addr,conn->h_length);
    con.sin_port = htons(port);
    
    if(connect(sockfd, (struct sockaddr*)&con, sizeof(con))<0)
        
    {
        cout<<"Connection Failed!"	<<endl;
        
    }
    else
        cout<<"Connection Success!"<<endl;
    string s;
    
    //cout<<"ssssssssssssss!"<<endl;
    pthread_t thread;
    pthread_create(&thread,NULL,&liswhilesend,&sockfd);
    while(cin>>s)
    {
        //cout<<s;
        sendmsg(sockfd,s,sendbuf);
        //cout<<"send!"<<endl;
        
        if(s=="Exit")
            break;
        
    }
    
}


int main ()
{
    int sockfd,n/*,listenfd,connfd*/;
    struct sockaddr_in serv/*,server_addr,clien_addr*/;
    struct hostent *server;
    char buffer[1024];
    string tmp,s;
    pthread_t lis;
    //thread newwww;
    /* create socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    
    /* initialize value in dest */
    bzero(&serv, sizeof(serv));
    int portnum;
    //cout<<"input port: ";
    //cin>>portnum;
    server=gethostbyname("140.112.107.39");
    if(server==NULL)
    {
        cout<<"NO serv!";
    }
    
    serv.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv.sin_addr.s_addr,server->h_length);
    serv.sin_port = htons(5900);
    
    
    /* Connecting to server */
    if(connect(sockfd, (struct sockaddr*)&serv, sizeof(serv))<0)
        
    {
        cout<<"Connection Failed!"	<<endl;
        return 0;
    }
    else
        cout<<"Connection Success!"<<endl;
    
    
    
    
    
    //cout<<"thread"<<endl;
    cout<<"Please enter user name: ";
    cin>>tmp;
    s="REGISTER#"+tmp;
    
    bzero(buffer,1024);
    strcpy(buffer,s.c_str());
    write(sockfd,buffer,strlen(buffer));
    bzero(buffer, 1024);
    read(sockfd,buffer,1023);
    
    string result;
    result.assign(buffer);
    
    cout<<result<<endl<<endl;
    s="List";
    chatwithserv(sockfd,s,buffer);
    
    while(cin)
    {
        string t;
        cout<<"Do?"<<endl;
        cin>>t;
        //cout<<"What to do: ";
        if(t=="wait")
        {
            int pt;
            cin>>pt;
            pthread_t thread;
            pthread_create(&thread,NULL,&Listen,&pt);
            pthread_join(thread, NULL);
            
            
        }
        else if(t=="conn")
        {
            
            pthread_t thread;
            int i;
            //cin>>t;
            cin>>i;
            connecting(i+1);
            
            //cout<<"End chat"<<endl;
            
        }
        else if (t=="Exit")
        {
        	chatwithserv(sockfd,t,buffer);
        	break;
        }
    }	
    
    
    
    
    
    
    
    
    close(sockfd);
    
    
    
    
    
    //return 0;
}
