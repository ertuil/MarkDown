//
//  languageCheck.hpp
//  MarkDown
//
//  Created by 陈鲁同 on 2017/12/8.
//  Copyright © 2017年 陈鲁同. All rights reserved.
//

#ifndef languageCheck_hpp
#define languageCheck_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <regex>
#include <time.h>

class languageCheck{
public:
    languageCheck();
    void outputMarkdown(std::vector<std::string> & dests);
    void setTitle(std::string title);
    
    int run(std::vector<std::string>& content);
    int getContent(std::vector<std::string>& content);
    void printContent();
    int translate();
private:
    std::string ContentTitle;
    
    enum All_Statue{
        INCODE,
        NORMAL,
        INBLOCK,
        INTABLE,
        TABLE_FMT
    } status;
    
    std::string preReplace(std::string s);
    std::string getTime();
    
    std::vector<std::string> content;
    size_t contentLength;
    std::vector<std::string> output;
    
    int isToc(std::string s);
    std::string TocContent;
    struct Toc{int deep; std::string name;};
    bool needToc = 0;
    int TocNumber = 0;
    std::vector<struct Toc> Tocs;
    std::vector<std::string> TocString;
    void handleToc();
    
    bool needMathjax = 0;
    
    int isTitle(std::string s);
    int isHorizon(std::string s);
    
    std::string handleTitle(std::string s,int titleLevel);
    std::string handleHorizon();
    
    void handleUnList();
    int isUnList(std::string s);
    
    void handleList();
    int isList(std::string s);
    
    int BlockNumber = 0;
    int isBlock(std::string s);
    std::string handleBlock(std::string s,int number);
    int handleBlock_S(std::string s);
    
    int isBr(std::string s);
    
    int isCode(std::string s);
    void handletoken();
    
    int handleCode(std::string s);
    
    int TableNumber = 0;
    std::vector<std::string> aligns;
    int handleTable(std::string s);
    std::vector<std::string> getTableElem(std::string s);
    
    void handleFlow();
};




#endif /* languageCheck_hpp */
