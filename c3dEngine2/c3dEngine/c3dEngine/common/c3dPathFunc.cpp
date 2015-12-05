//
//  c3dPathFunc.cpp
//  HelloOpenGL
//
//  Created by wantnon (yang chao) on 12-11-13.
//
//


#include "c3dPathFunc.h"
string getFileNameFromFilePath(string filePath){
    string clipPath,fileName;
    vector<string> rs=splitStrInTwoByLastBar(filePath);
    clipPath=rs[0];
    fileName=rs[1];
    return fileName;
    
}
vector<string> splitStrInTwoByLastDotBehindLastBar(const string&str)
//part1 and part2 do not contain the split-dot
//if there is no dot in str, then part1=str and part2=""
//example:
//  "abc/def.txt" -> "abc/def"+"txt"
//  "abc/def" -> "abc/def"+""
//  "abc2.0/def" -> "abc2.0/def"+""  (not "abc2"+"0/def")
{
    string part1="";
    string part2="";
	//find the last "." location
    int i_lastDot=-1;
    int len=(int)str.size();
    for(int i=len-1;i>=0;i--){
        if(str[i]=='.'){
            i_lastDot=i;
            break;
        }
        if(str[i]=='/'||str[i]=='\\'){
            break;
        }
    }
    if(i_lastDot==-1){//not found
        part1=str;
        part2="";
        vector<string> rs;
        rs.push_back(part1);
        rs.push_back(part2);
        return rs;
    }
    //found
    //use i_lastDot to split str
    for(int i=0;i<len;i++){
        if(i<i_lastDot){
            part1+=str[i];
        }else if(i>i_lastDot){
            part2+=str[i];
        }//i==i_lastDot no action
    }//got part1 and part2
    vector<string> rs;
    rs.push_back(part1);
    rs.push_back(part2);
    return rs;
    
    
}

vector<string> splitStrInTwoByLastBar(const string&str)
//part1 and prat2 do not contain the spliat-'/' or '\\' 
//if there is no '/' or '\\' in str, then part1=str and part2=""
{
    string part1="";
    string part2="";
    //find the last "/" location
    int i_lastBar=-1;
    int len=(int)str.size();
    for(int i=len-1;i>=0;i--){
        if(str[i]=='/'||str[i]=='\\'){
            i_lastBar=i;
            break;
        }
    }
    if(i_lastBar==-1){//not found
        part1=str;
        part2="";
        vector<string> rs;
        rs.push_back(part1);
        rs.push_back(part2);
        return rs;
    }
    //found 
    //use i_lastBar to split str
    for(int i=0;i<len;i++){
        if(i<i_lastBar){
            part1+=str[i];
        }else if(i>i_lastBar){
            part2+=str[i];
        }//i==i_lastBar, no action
    }//got part1 and part2
    vector<string> rs;
    rs.push_back(part1);
    rs.push_back(part2);
    return rs;
    
}


