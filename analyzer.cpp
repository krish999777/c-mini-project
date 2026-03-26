#include<iostream>
#include<string>
#include <curl/curl.h>
#include "json.hpp"
#include<iomanip>
using json = nlohmann::json;
using namespace std;
class Student{
    char username[100];
    char name[50];
    char description[160];
    int followers;
    int following;
    int repos;
public:
    void setStudent(char u[100],char n[50],char d[160],int fr,int fg,int r){
        strcpy(username,u);
        strcpy(name,n);
        strcpy(description,d);
        followers=fr;
        following=fg;
        repos=r;
    }
    void getStudent(){
        cout<<"Username="<<username<<endl;
        cout<<"Name="<<name<<endl;
        cout<<"Description="<<description<<endl;
        cout<<"Followers="<<followers<<endl;
        cout<<"Following="<<following<<endl;
        cout<<"Repos="<<repos<<endl;
    }
    int getFollowers(){
        return followers;
    }
    int getFollowing(){
        return following;
    }
    int getRepos(){
        return repos;
    }
};
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
int maximumFollowers(int n,Student s[]){
    int maximum=0;
    for(int i=0;i<n;i++){
        int followers=s[i].getFollowers();
        if(followers>maximum){
            maximum=followers;
        }
    }
    return maximum;
}
int minimumFollowers(int n,Student s[]){
    int minimum=9999999;
    for(int i=0;i<n;i++){
        int followers=s[i].getFollowers();
        if(followers<minimum){
            minimum=followers;
        }
    }
    return minimum;
}
int maximumFollowing(int n,Student s[]){
    int maximum=0;
    for(int i=0;i<n;i++){
        int following=s[i].getFollowing();
        if(following>maximum){
            maximum=following;
        }
    }
    return maximum;
}
int minimumFollowing(int n,Student s[]){
    int minimum=9999999;
    for(int i=0;i<n;i++){
        int following=s[i].getFollowing();
        if(following<minimum){
            minimum=following;
        }
    }
    return minimum;
}
int maximumRepos(int n,Student s[]){
    int maximum=0;
    for(int i=0;i<n;i++){
        int repos=s[i].getRepos();
        if(repos>maximum){
            maximum=repos;
        }
    }
    return maximum;
}
int minimumRepos(int n,Student s[]){
    int minimum=9999999;
    for(int i=0;i<n;i++){
        int repos=s[i].getRepos();
        if(repos<minimum){
            minimum=repos;
        }
    }
    return minimum;
}
int main(){
    CURL *curl;
    CURLcode result;
    string response;

    curl=curl_easy_init();
    if(curl==NULL){
        cout<<"failed"<<endl;
        return 0;
    }
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "C++ App");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    int n;
    cout<<"Enter number of students"<<endl;
    cin>>n;
    Student s[60];
    for(int i=0;i<n;i++){
        response.clear();
        cout<<"Enter username for student "<<i+1<<endl;
        string username;
        cin >> username;
        string url = "https://api.github.com/users/" + username;
        curl_easy_setopt(curl,CURLOPT_URL,url.c_str());
        result=curl_easy_perform(curl);
        if(result!=CURLE_OK){
            cout<<"failed"<<endl;
            i--;
            continue;
        }
        json data = json::parse(response);
        if(data["message"]=="Not Found"){
            cout<<"Not found username"<<endl;;
            i--;
            continue;
        }
        char login[100];
        char name_c[50];
        char bio_c[160];
        strcpy(login,data["login"].is_null()?"":data["login"].get<string>().c_str());
        strcpy(name_c,data["name"].is_null()?"":data["name"].get<string>().c_str());
        strcpy(bio_c,data["bio"].is_null()?"":data["bio"].get<string>().c_str());
        s[i].setStudent(
            login,
            name_c,
            bio_c,
            data["followers"],
            data["following"],
            data["public_repos"]);
    }
    cout<<"Enter what details you want to view"<<endl;
    while(1){
        int choice;
        cout<<"------------------CHOOSE YOUR CHOICE------------------"<<endl;
        cout<<setw(10)<<1<<"\t View details of all users"<<endl; 
        cout<<setw(10)<<2<<"\t Maximum and Minimum Followers,following,repos"<<endl; 
        cout<<setw(10)<<0<<"\t Exit program"<<endl; 
        cin>>choice;
        if(choice==0){
            break;
        }else if(choice==1){
            cout<<endl;
            for(int i=0;i<n;i++){
                cout<<endl;
                s[i].getStudent();
                cout<<endl;
            }
        }else if(choice==2){
            int maxfr=maximumFollowers(n,s);
            int minfr=minimumFollowers(n,s);
            int maxfl=maximumFollowing(n,s);
            int minfl=minimumFollowing(n,s);
            int maxr=maximumRepos(n,s);
            int minr=minimumRepos(n,s);
            cout<<endl;
            cout<<setw(20)<<"Maximum Followers"<<"\t"<<maxfr<<endl;
            cout<<setw(20)<<"Minimum Followers"<<"\t"<<minfr<<endl;
            cout<<setw(20)<<"Maximum Following"<<"\t"<<maxfl<<endl;
            cout<<setw(20)<<"Minimum Following"<<"\t"<<minfl<<endl;
            cout<<setw(20)<<"Maximum Repos"<<"\t"<<maxr<<endl;
            cout<<setw(20)<<"Minimum Repos"<<"\t"<<minr<<endl;
            cout<<endl;
        }else{
            cout<<"Invalid choice"<<endl;
        }
    }

    curl_easy_cleanup(curl);
}