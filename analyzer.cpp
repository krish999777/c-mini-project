/*
g++ analyzer.cpp -o app \
-std=c++17 \
-I/opt/homebrew/include \
-L/opt/homebrew/lib \
-lraylib \
-framework OpenGL \
-framework Cocoa \
-framework IOKit \
-framework CoreVideo \
-lcurl
*/
#include<iostream>
#include<string>
#include <curl/curl.h>
#include "json.hpp"
#include<iomanip>
#include <raylib.h>
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
    string getUsername(){
        return username;
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
void showBarChart(int n, Student s[]) {
    InitWindow(800, 600, "Bar Chart");

    int maxi = 0;
    for(int i = 0; i < n; i++)
        maxi = max(maxi, s[i].getRepos());

    while (!WindowShouldClose()) {
        BeginDrawing();
        DrawLine(80, 500, 750, 500, BLACK);
        DrawLine(80, 100, 80, 500, BLACK);
        ClearBackground(RAYWHITE);
        int divisions = 5;

        for(int i = 0; i <= divisions; i++) {
            int y = 500 - (i * 400 / divisions);
            int value = (maxi * i) / divisions;

            DrawLine(75, y, 80, y, BLACK);

            DrawText(to_string(value).c_str(), 30, y - 5, 10, BLACK);
        }

        int width = 600 / n;

        for(int i = 0; i < n; i++) {
            int height = (s[i].getRepos() * 400) / maxi;

            DrawRectangle(100 + i * width, 500 - height,
                          width - 10, height, BLUE);
            DrawText(s[i].getUsername().c_str(),
            105 + i * width,
            510,
            10,
            BLACK);
        }

        DrawText("Repos Chart", 300, 20, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();
}
int maximumFollowers(int n,Student s[]){
    int maximum=0;
    for(int i=0;i<n;i++){
        int followers=s[i].getFollowers();
        if(followers>s[maximum].getFollowers()){
            maximum=i;
        }
    }
    return maximum;
}
int minimumFollowers(int n,Student s[]){
    int minimum=0;
    for(int i=0;i<n;i++){
        int followers=s[i].getFollowers();
        if(followers<s[minimum].getFollowers()){
            minimum=i;
        }
    }
    return minimum;
}
int maximumFollowing(int n,Student s[]){
    int maximum=0;
    for(int i=0;i<n;i++){
        int following=s[i].getFollowing();
        if(following>s[maximum].getFollowing()){
            maximum=i;
        }
    }
    return maximum;
}
int minimumFollowing(int n,Student s[]){
    int minimum=0;
    for(int i=0;i<n;i++){
        int following=s[i].getFollowing();
        if(following<s[minimum].getFollowing()){
            minimum=i;
        }
    }
    return minimum;
}
int maximumRepos(int n,Student s[]){
    int maximum=0;
    for(int i=0;i<n;i++){
        int repos=s[i].getRepos();
        if(repos>s[maximum].getRepos()){
            maximum=i;
        }
    }
    return maximum;
}
int minimumRepos(int n,Student s[]){
    int minimum=0;
    for(int i=0;i<n;i++){
        int repos=s[i].getRepos();
        if(repos<s[minimum].getRepos()){
            minimum=i;
        }
    }
    return minimum;
}
float averageFollowers(int n,Student s[]){
    int total=0;
    for(int i=0;i<n;i++){
        total+=s[i].getFollowers();
    }
    return (float)total/n;
}
float averageFollowing(int n,Student s[]){
    int total=0;
    for(int i=0;i<n;i++){
        total+=s[i].getFollowing();
    }
    return (float)total/n;
}
float averageRepos(int n,Student s[]){
    int total=0;
    for(int i=0;i<n;i++){
        total+=s[i].getRepos();
    }
    return (float)total/n;
}
int findUser(int n,Student s[],string username){
    for(int i=0;i<n;i++){
        if(s[i].getUsername()==username){
            return i;
        }
    }
    return -1;
}
void compareUsers(Student a, Student b){
    cout<<endl<<"----- COMPARISON -----"<<endl;
    cout<<endl<<"Followers:"<<endl;
    if(a.getFollowers() > b.getFollowers())
        cout<<a.getUsername()<<" is higher"<<endl;
    else if(a.getFollowers() < b.getFollowers())
        cout<<b.getUsername()<<" is higher"<<endl;
    else
        cout<<"Equal"<<endl;

    cout<<"Following:"<<endl;
    if(a.getFollowing() > b.getFollowing())
        cout<<a.getUsername()<<" is higher"<<endl;
    else if(a.getFollowing() < b.getFollowing())
        cout<<b.getUsername()<<" is higher"<<endl;
    else
        cout<<"Equal"<<endl;

    cout<<"Repos:"<<endl;
    if(a.getRepos() > b.getRepos())
        cout<<a.getUsername()<<" is higher"<<endl;
    else if(a.getRepos() < b.getRepos())
        cout<<b.getUsername()<<" is higher"<<endl;
    else
        cout<<"Equal"<<endl;
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
        cout<<setw(10)<<2<<"\t Maximum and Minimum followers,following,repos"<<endl; 
        cout<<setw(10)<<3<<"\t Average followers,following,repos"<<endl; 
        cout<<setw(10)<<4<<"\t Search for user"<<endl; 
        cout<<setw(10)<<5<<"\t Compare 2 users"<<endl;
        cout<<setw(10)<<6<<"\t Show bar chart"<<endl; 
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
            cout<<setw(20)<<"----------Maximum Followers---------"<<endl;
            s[maxfr].getStudent();
            cout<<setw(20)<<"----------Minimum Followers----------"<<endl;
            s[minfr].getStudent();
            cout<<setw(20)<<"----------Maximum Following----------"<<endl;
            s[maxfl].getStudent();
            cout<<setw(20)<<"----------Minimum Following----------"<<endl;
            s[minfl].getStudent();
            cout<<setw(20)<<"----------Maximum Repos----------"<<endl;
            s[maxr].getStudent();
            cout<<setw(20)<<"----------Minimum Repos----------"<<endl;
            s[minr].getStudent();
            cout<<endl;
        }else if(choice==3){
            float avgfr=averageFollowers(n,s);
            float avgfg=averageFollowing(n,s);
            float avgr=averageRepos(n,s);
            cout<<endl;
            cout<<setw(20)<<"Average Followers"<<"\t"<<avgfr<<endl;
            cout<<setw(20)<<"Average Following"<<"\t"<<avgfg<<endl;
            cout<<setw(20)<<"Average Repos"<<"\t"<<avgr<<endl;
            cout<<endl;
        }else if(choice==4){
            cout<<endl;
            cout<<"Enter username of student to search"<<endl;
            string c;
            cin>>c;
            int index=findUser(n,s,c);
            if(index==-1){
                cout<<"Student not found"<<endl;
            }else{
                s[index].getStudent();
            }

        }else if(choice==5){
            string u1, u2;
            cout<<"Enter first username:\n";
            cin>>u1;
            cout<<"Enter second username:\n";
            cin>>u2;

            int i1 = findUser(n, s, u1);
            int i2 = findUser(n, s, u2);

            if(i1 == -1 || i2 == -1){
                cout<<"One or both users not found\n";
            } else {
                compareUsers(s[i1], s[i2]);
            }
        }else if(choice==6){
            showBarChart(n, s);
        }
        else{
            cout<<"Invalid choice"<<endl;
        }
    }

    curl_easy_cleanup(curl);
}