//
//  languageCheck.cpp
//  MarkDown
//
//  Created by 陈鲁同 on 2017/12/8.
//  Copyright © 2017年 陈鲁同. All rights reserved.
//

#include "languageCheck.hpp"

void languageCheck::setTitle(std::string title){
    ContentTitle = title;
}

std::string languageCheck::getTime(){
    time_t timep;
    time (&timep);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S",localtime(&timep) );
    return tmp;
}

int languageCheck::run(std::vector<std::string>& content){
    content.clear();
    output.clear();
    aligns.clear();
    for(int i = 0;i < 10;++i){
        aligns.push_back("center");
    }
    status = NORMAL;
    BlockNumber = 0;
    TableNumber = 0;
    output.push_back("<!DOCTYPE html>");
    output.push_back("<html>");
    output.push_back("<head>");
    output.push_back("<title>" + ContentTitle + "</title>");
    output.push_back("<meta name=\"Application Author\" content=\"Ertuil\" />");
    output.push_back("<meta name=\"Time\" content=\""+getTime()+"\" />");
    //output.push_back("<style type=\"text/css\">div {display: block;font-family: \"Times New Roman\",Georgia,Serif}#wrapper { width: 100%;height:100%; margin: 0; padding: 0;}#left { float:left; width: 10%;  height: 100%;  }#second {   float:left;   width: 80%;height: 100%;}#right {float:left;  width: 10%;  height: 100%; }</style>");
    //output.push_back("<style type=\"text/css\">@import url(default.css);</style> ");
    std::ifstream cssfile("default.css");
    std::string buff;
    output.push_back("<style type=\"text/css\">");
    while(getline(cssfile,buff)){
        output.push_back(buff);
    }

    output.push_back("</style>");
    output.push_back("<meta charset=\"utf-8\"/>");
    output.push_back("<script type=\"text/x-mathjax-config\"> MathJax.Hub.Config({tex2jax: {inlineMath: [['$','$'], ['\\\\(','\\\\)']]}});</script><script type=\"text/javascript\" src=\"http://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML\"></script>");
    output.push_back("<script src=\"https://cdn.bootcss.com/raphael/2.2.7/raphael.js\"></script>");
    output.push_back("<script src=\"https://cdn.bootcss.com/flowchart/1.7.0/flowchart.js\"></script>");
    output.push_back("</head>");
    output.push_back("<body>");
    //output.push_back("<div id=\"wrapper\">");
    output.push_back("<div id = \"wrapper\"> <div id=\"left\"></div><div id=\"main\">");
    
    translate();
    
    output.push_back("</br></br></div>");
    output.push_back("<div id=\"right\">");
    if(needToc == true){
        handleToc();
    }
    output.push_back("<br /></div></div>");
    if(needMathjax == 0){
        std::vector<std::string>::iterator it = output.begin() + 4;
        output.erase(it);
    }
    output.push_back("</body>");
    output.push_back("</html>");
    return 0;
}

int languageCheck::getContent(std::vector<std::string>& content){
    this->content = content;
    contentLength = content.size();
    return 0;
}

void languageCheck::printContent(){
    for(auto v : output){
        std::cout<<v<<std::endl;
    }
}

void languageCheck::outputMarkdown(std::vector<std::string> & dests){
    for(auto v : output){
        dests.push_back(v);
    }
}
int languageCheck::translate(){
    handletoken();
    handleUnList();
    handleList();
    handleFlow();
    for(auto s : content){
        if(needToc == 0) isToc(s);
        auto titleLevel = isTitle(s);
        //auto blockNumber = isBlock(s);
        if(handleBlock_S(s) == 1){}
        else if(handleCode(s) == 1){}
        else if(handleTable(s) == 1){}
        else if(titleLevel != 0){
            output.push_back(handleTitle(s, titleLevel));
        }else if(isHorizon(s)){
            output.push_back(handleHorizon());
        //}else if(blockNumber != 0){
        //    output.push_back(handleBlock(s,blockNumber));
        }else if(isBr(s)){
            output.push_back("<br />");
        }else{
            output.push_back(s);
        }
    }
    return 0;
}


int languageCheck::isTitle(std::string s){
    auto titleLevel = 0;
    if(s.empty()){
        return 0;
    }
    while(s[titleLevel] == '#'){
        ++titleLevel;
        if(titleLevel > 7) break;
    }
    if(titleLevel > 7 || s[titleLevel] != ' '){
        titleLevel = 0;
    }
    return titleLevel;
}

std::string languageCheck::handleTitle(std::string s, int titleLevel){
    auto name = s.substr(titleLevel + 1);
    auto tem = "<a name=\""+ name +"\"></a>";
    tem += "<h"+std::to_string(titleLevel)+">"+  name +"</h"+std::to_string(titleLevel)+">";
    Tocs.push_back(Toc{titleLevel,name});
    return tem;
}

int languageCheck::isHorizon(std::string s){
    std::regex e1("^[-*]{3,}$");
    std::smatch sm;
    std::regex_match (s,sm,e1);
    if(sm.size()  != 0){
        return 1;
    }else{
        return 0;
    }
}



std::string languageCheck::handleHorizon(){
    return "<hr>";
}

int languageCheck::isUnList(std::string s){
    if(s.empty()){
        return 0;
    }
    auto idx = 0;
    while(s[idx] == ' ') ++idx;
    if(s[idx] == '+' || s[idx] == '-' || s[idx] == '*'){
        ++idx;
        if(idx < s.size() && s[idx] == ' '){
            return idx;
        }
    }
    return 0;
}

void languageCheck::handleUnList(){
    auto ii = 0,top = 0;
    std::stack<int> stack;
    stack.push(0);
    std::string temp;
    for(auto i = 0; i < content.size();++i){
        top = stack.top();
        std::string s = content[i];
        ii = isUnList(s);
        if(ii <= 0){
            temp = "";
            while(ii < top){
                temp += "</ul>";
                stack.pop();
                top = stack.top();
            }
            temp += s;
            content[i] = temp;
            continue;
        }
        if(ii > stack.top()){
            temp = "<ul><li>" + s.substr(ii + 1) + "</li>";
            stack.push(ii);
        }else if(ii == stack.top()){
            temp = "<li>" + s.substr(ii + 1) + "</li>";
        }else{
            temp = "";
            while(ii < top){
                temp += "</ul>";
                stack.pop();
                top = stack.top();
            }
            if(ii > stack.top()){
                temp += "<ul><li>" + s.substr(ii + 1) + "</li>";
                stack.push(ii);
            }else if(ii == stack.top()){
                temp += "<li>" + s.substr(ii + 1) + "</li>";
            }
        }
        content[i] = temp;
    }
}

void languageCheck::handleList(){
    auto ii = 0,top = 0;
    std::stack<int> stack;
    stack.push(0);
    std::string temp;
    for(auto i = 0; i < content.size(); ++i){
        top = stack.top();
        std::string s = content[i];
        ii = isList(s);
        if(ii <= 1){
            temp = "";
            while(ii < top){
                temp += "</ol>";
                stack.pop();
                top = stack.top();
            }
            temp += s;
            content[i] = temp;
            continue;
        }
        if(ii > stack.top()){
            temp = "<ol><li>" + s.substr(ii + 1) + "</li>";
            stack.push(ii);
        }else if(ii == stack.top()){
            temp = "<li>" + s.substr(ii + 1) + "</li>";
        }else{
            temp = "";
            while(ii < top){
                temp += "</ol>";
                stack.pop();
                top = stack.top();
            }
            if(ii > stack.top()){
                temp += "<ol><li>" + s.substr(ii + 1) + "</li>";
                stack.push(ii);
            }else if(ii == stack.top()){
                temp += "<li>" + s.substr(ii + 1) + "</li>";
            }
        }
        content[i] = temp;
    }
}
int languageCheck::isList(std::string s){
    std::regex e("^\\s*\\d+\\.\\s.+");
    std::smatch sm;
    std::regex_match (s,sm,e);
    if(sm.size() != 0){
        auto idx = 0;
        while(s[idx] == ' ') ++idx;
        while(isdigit(s[idx])) ++idx;
        return ++idx;
    }else{
        return 0;
    }
}


int languageCheck::isBlock(std::string s){
    std::regex e1("^>*\\s+.*");
    std::smatch sm;
    std::regex_match (s,sm,e1);
    if(sm.size()  != 0){
        auto idx = 0;
        while(s[idx] == '>') idx++;
        return idx;
    }else{
        return 0;
    }
}

std::string languageCheck::handleBlock(std::string s,int number){
    std::string temp,temp2;
    auto ii = number;
    while(ii > 0){
        temp += "<blockquote style=\"color:#8fbc8f\">";
        temp2 += "</blockquote>";
        -- ii;
    }
    return temp +  s.substr(number + 1) + temp2;
}

int languageCheck::handleBlock_S(std::string s){
    
    int CorNumber = isBlock(s);
    //std::cout<<CorNumber<<std::endl;
    std::string result = "";
    
    if(status == NORMAL && CorNumber >= 1){
        status = INBLOCK;
        while(BlockNumber < CorNumber){
            BlockNumber ++;
            result += "<blockquote>";
        }
        result += s.substr(CorNumber);
        result += "<br />";
        output.push_back(result);
        return 1;
    }else if(status == INBLOCK){
        if(CorNumber > BlockNumber){
            while(BlockNumber < CorNumber){
                BlockNumber ++;
                result += "<blockquote>";
            }
        }else if(CorNumber < BlockNumber){
            while(BlockNumber > CorNumber){
                BlockNumber --;
                result += "</blockquote>";
            }
        }
        result += s.substr(CorNumber);
        result += "<br />";
        if(BlockNumber <= 0){
            status = NORMAL;
            BlockNumber = 0;
        }
        output.push_back(result);
        return 1;
    }
    return 0;
}


void languageCheck::handletoken(){
    std::string temp,temp2;
    std::regex re_lt("<");
    std::regex re_and("&");
    std::regex re_link("\\[(.*)\\]\\((.*)\\)");
    std::regex re_sup("\\[(.*)\\]\\[(.*)\\]");
    std::regex re_img("!\\[(.*)\\]\\((.*)\\)");
    std::regex re_i("\\*([^\\*]+)\\*");
    std::regex re_S("\\~\\~([^\\~\\~]+)\\~\\~");
    std::regex re_b("\\*\\*([^\\*\\*]+)\\*\\*");
    std::regex re_m("\\$([^\\$]*)\\$");
    std::regex re_code("```(.*)```");
    std::smatch sm;
    for(int i = 0; i < content.size(); ++i){
        temp = content[i];
        temp.insert(temp.begin(), ' ');
        temp2.clear();
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),re_and,"&amp;");
        temp.clear();
        std::regex_replace(std::back_inserter(temp),temp2.begin(),temp2.end(),re_lt,"&lt;");
        temp2.clear();
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),std::regex("\\\\&amp;"),"&");
        temp.clear();
        std::regex_replace(std::back_inserter(temp),temp2.begin(),temp2.end(),std::regex("\\\\&lt;"),"<");
        temp2.clear();
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),re_img,"<img src=\"$2\" align=\"middle\">");
        temp.clear();
        std::regex_replace(std::back_inserter(temp),temp2.begin(),temp2.end(),re_sup,"$1<sup>$2</sup>");
        temp2.clear();
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),re_link,"<a href=\"$2\" target=\"_blank\">$1</a>");
        temp.clear();
        std::regex_replace(std::back_inserter(temp),temp2.begin(),temp2.end(),re_b,"<b> $1 </b>");
        temp2.clear();
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),re_S,"<s> $1 </s>");
        temp.clear();
        std::regex_replace(std::back_inserter(temp),temp2.begin(),temp2.end(),re_i,"<i> $1 </i>");
        temp2.clear();
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),re_code,"<code>  $1 </code>");
        std::regex_match(temp,sm,re_m);
        if(sm.size() > 0){
            needMathjax = 1;
        }
        temp2.erase(temp2.begin());
        content[i] = temp2;
    }
}

std::string languageCheck::preReplace(std::string s){
    if(status != NORMAL){
        std::string temp,temp2;
        std::regex re_lt("<");
        std::regex re_and("&");
        std::smatch sm;
        temp = s;
        temp2.clear();
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),re_and,"&amp;");
        temp.clear();
        std::regex_replace(std::back_inserter(temp),temp2.begin(),temp2.end(),re_lt,"&lt;");
        return temp;
    }
    return s;
}

int languageCheck::isBr(std::string s){
    std::regex re_br("^[\\s\\t]*$");
    std::smatch sm;
    std::regex_match(s,sm,re_br);
    if(sm.size() > 0){
        return 1;
    }else{
        return 0;
    }
}

languageCheck::languageCheck(){
    needMathjax = 0;
    status = NORMAL;
}

int languageCheck::handleCode(std::string s){
    std::regex re_Code("^\\s*```\\s*[a-zA-Z1-9\\+\\.]*\\s*$");
    std::regex re_s("\\s");
    std::regex re_t("\\t");
    std::smatch sm;
    std::regex_match(s, sm, re_Code);
    std::string temp,temp2;
    if(status == NORMAL && sm.size() > 0){
        status = INCODE;
        output.push_back("<code> <pre>");
        return 1;
    }else if(status == INCODE && sm.size() > 0){
        status = NORMAL;
        output.push_back("</pre></code>");
        return 1;
    }else if(status == INCODE){
        std::regex_replace(std::back_inserter(temp),s.begin(),s.end(),re_s,"&nbsp");
        std::regex_replace(std::back_inserter(temp2),temp.begin(),temp.end(),re_t,"&nbsp&nbsp&nbsp&nbsp");
        temp="<span>"+temp2+"</span><br />";
        output.push_back(temp);
        return 1;
    }else{
        return 0;
    }
}

int languageCheck::handleTable(std::string s){
    std::regex re_Table("\\|(.*)\\|");
    std::regex re_rep("\\|");
    std::smatch sm;
    std::regex_match(s,sm,re_Table);
    std::string result = "";
    std::vector<std::string> elem;
    if(status == NORMAL && sm.size() > 0){
        elem = getTableElem(s);
        result += "<table border=\"1\" align=\"center\" cellspacing=\"1\" cellpadding=\"5\">";
        result += "<tr>";
        for(auto e : elem){
            result += ("<th>" +  e + "</th>");
        }
        result += "</tr>";
        TableNumber = static_cast<int>(elem.size());
        status = TABLE_FMT;
        output.push_back(result);
        return 1;
    }else if(status == TABLE_FMT && sm.size() > 0){
        elem = getTableElem(s);
        for(auto ii = 0; ii < TableNumber; ++ii){
            std::regex_match(elem[ii],sm,std::regex("\\:\\-*"));
            if(sm.size() > 0) aligns[ii] = "left";
            std::regex_match(elem[ii],sm,std::regex("\\:\\-*\\:"));
            if(sm.size() > 0) aligns[ii] = "center";
            std::regex_match(elem[ii],sm,std::regex("\\-*\\:"));
            if(sm.size() > 0) aligns[ii] = "right" ;
        }
        status = INTABLE;
        return 1;
    }else if(status == INTABLE && sm.size() > 0){
        elem = getTableElem(s);
        result += "<tr>";
        for(auto ii = 0; ii < TableNumber; ++ii){
            result += ("<td align=\""+aligns[ii]+"\">" +elem[ii]+ "</td>");
        }
        result += "</tr>";
        output.push_back(result);
        return 1;
    }else if((status == INTABLE || status == TABLE_FMT) && sm.size() == 0){
        TableNumber = 0;
        status = NORMAL;
        result = "</table>"+ s;
        output.push_back(result);
        return 1;
    }
    return 0;
}

std::vector<std::string> languageCheck::getTableElem(std::string s){
    std::vector<std::string> result;
    std::string temp = "";
    for(auto c : s){
        if(c == '|'){
            result.push_back(temp);
            temp.clear();
        }else{
            temp += c;
        }
    }
    result.erase(result.begin());
    return result;
}

int languageCheck::isToc(std::string s){
    std::regex re_toc("\\[TOC\\]");
    std::smatch sm;
    std::regex_search(s,sm,re_toc);
    if(sm.size() > 0){
        needToc = 1;
        TocContent = s;
        return 1;
    }
    return 0;
}

void languageCheck::handleToc(){
    auto deep = 0;
    auto TocPosition = std::find(output.begin(),output.end(),TocContent);
    if(TocPosition == output.end()){
        return;
    }
    for(auto list : Tocs){
        if(list.deep > deep){
            while(deep < list.deep){
                TocString.push_back("<ol style = \"line-height:80%\">");
                deep ++;
            }
            TocString.push_back("<li> <a href=\"#"+list.name+"\">"+list.name+"</a> </li>");
        }else if(list.deep == deep){
            TocString.push_back("<li> <a href=\"#"+list.name+"\">"+list.name+"</a> </li>");
        }else{
            while(deep > list.deep){
                TocString.push_back("</ol>");
                deep --;
            }
            TocString.push_back("<li> <a href=\"#"+list.name+"\">"+list.name+"</a> </li>");
        }
    }
    while(deep > 0){
        TocString.push_back("</ol>");
        deep --;
    }
    for(int ii = static_cast<int>(TocString.size()) - 1; ii >= 0; ii--){
        output.insert(TocPosition + 1, TocString[ii]);
    }
}


void languageCheck::handleFlow(){
    auto flowNumber = 0;
    std::regex re_flow("^\\[flow\\]$");
    std::regex re_over("^\\[!flow\\]$");
    std::smatch sm;
    for(auto ii = 0; ii < content.size(); ++ii){
        std::regex_search(content[ii],sm,re_flow);
        if(sm.size() > 0){
            ++flowNumber;
            content[ii] = "<div id=\"flow"+std::to_string(flowNumber)+"\"></div>";
            auto jj = ii + 1;
            content[ii] += "<script>var diagram = flowchart.parse(";
            std::regex_match(content[jj],sm,re_over);
            while(jj < content.size()&& sm.size() == 0){
                content[ii] += ("'"+content[jj]+"\\n'+");
                content.erase(content.begin() + jj);
                std::regex_match(content[jj],sm,re_over);
            }
            content[ii] += "' ');diagram.drawSVG('flow"+std::to_string(flowNumber) +"');</script>";
            content.erase(content.begin() + jj);
        }
    }
}
