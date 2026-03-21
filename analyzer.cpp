#include<iostream>
#include<string>
#include <curl/curl.h>
#include "json.hpp"
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
        cout<<"Description="<<description<<endl;
        cout<<"Followers="<<followers<<endl;
        cout<<"Following="<<following<<endl;
        cout<<"Repos="<<repos<<endl;
    }
};
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}
int main(){
    CURL *curl;
    CURLcode result;
    string response;

    curl=curl_easy_init();
    if(curl==NULL){
        cout<<"failed";
        return 0;
    }
    curl_easy_setopt(curl,CURLOPT_URL,"https://api.github.com/users/krish999777");
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "C++ App");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    result=curl_easy_perform(curl);
    if(result!=CURLE_OK){
        cout<<"failed";
        return 0;
    }

    cout<<endl<<endl<<endl;
    json data = json::parse(response);

    char login[100];
    char name_c[50];
    char bio_c[160];
    strcpy(login,data["login"].get<string>().c_str());
    strcpy(name_c,data["name"].get<string>().c_str());
    strcpy(bio_c,data["bio"].get<string>().c_str());
    Student s;
    s.setStudent(login,name_c,bio_c,data["followers"],data["following"],data["public_repos"]);
    s.getStudent();
    curl_easy_cleanup(curl);
    return 0;
}